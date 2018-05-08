/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include <vector>
using std::vector;

#define MAXINTENSITY 4095
#define PI (float) 3.14159265358979323846

//extrafunc state hw4
void computeColor(GzRender *render, GzCoord color, GzLight lights[MAX_LIGHTS], Vertex v, GzCoord R, float spec);

//extrafunc hw2
vector<Vertex> sortVertY(vector<Vertex> v, vector<Vertex> v_sort) {
	//0-1-2 from top to bottom
	int max = 0, min = 0, mid = 0;
	for (int i = 0; i < 3; i++) {
		if (v[i].y >= v[max].y) {
			if (v[i].y > v[max].y || (v[i].y == v[max].y && v[i].x >= v[max].x)) {
				max = i;
			}
		}
		if (v[i].y <= v[min].y) {
			if (v[i].y < v[min].y || (v[i].y == v[min].y && v[i].x >= v[min].x)) {
				min = i;
			}
		}
	}
	for (int m = 0; m < 3; m++) {
		if (m != min && m != max) {
			mid = m;
		}
	}
	v_sort.push_back(v[min]);
	v_sort.push_back(v[mid]);
	v_sort.push_back(v[max]);
	return v_sort;
}
inline int CCDirection(vector<Vertex> vertex) {
	float d, z, x2;
	
	
	
	if (vertex.empty()) {
		return GZ_FAILURE;
	}
	else {
		d = (vertex[0].y - vertex[2].y) / (vertex[0].x - vertex[2].x);
		z = vertex[0].y - d * vertex[0].x;
		x2 = (vertex[1].y - z) / d;
		//bottom hori
		if ((float)(vertex[2].y - vertex[1].y) == 0) {
			return 1;//bottom
		}//top hori
		else if ((float)(vertex[1].y - vertex[0].y) == 0) {
			return 2;//top
		}
		else if (x2 > vertex[1].x) {
			return 3;//left
		}
		else if (x2 < vertex[1].x) {
			return 4;//right
		}
		else {
			return GZ_FAILURE;
		}
	}
}
//set triangle 
//top 1 0 | 1 0  bottom 0   | 0   left    0    |right    0
//      2 | 2           1 2 | 1 2      1       |             1
//                                          2  |       2
//e e[0] 0-1 e[1] 1-2  e[2] 0-2
int setTri(GzRender* render,int orient, vector<Edge>&e, vector<Vertex>& v) {
	GzColor color;
	GzCoord R;
	//gau
	if (render->interp_mode == GZ_COLOR)
	{
		//compute color at 3 vertices
		for (int i = 0; i < 3; i++)
		{
			computeColor(render, color, render->lights, v[i], R, render->spec);
			//record color using nx, ny, and nz
			v[i].nx = color[0];
			v[i].ny = color[1];
			v[i].nz = color[2];
		}
	}//phong
	else if (render->interp_mode == GZ_NORMALS)
	{
		//do nothing, keep nx, ny, nz as the normal
	}
	
	if (v.empty() ||e.empty()) {
		return GZ_FAILURE;
	}
	else {
		e[0].setEdge(v[0], v[1]);
		e[1].setEdge(v[1], v[2]);
		e[2].setEdge(v[0], v[2]);
	}
	return GZ_SUCCESS;
}
int scan(GzRender *render, int orient, vector<Edge>& e, vector<Vertex>& v_sort);

//extra func hw3
//matrix multipulation
void Matrix_d4(GzMatrix x, GzMatrix y, GzMatrix z) {
	float temp;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp = 0.0;
			for (int k = 0; k < 4; k++) {
				temp = temp + x[i][k] * y[k][j];
			}
			z[i][j] = temp;
		}
	}
}
//trans screen-v to model-v
Vertex trs_v(Vertex v, GzMatrix ximg, Vertex v_m) {
	//trans v to matrix 4*1;
	float temp_v[4], temp_vm[4] = { 0.0, 0.0, .0, .0 };
	temp_v[0] = v.x;
	temp_v[1] = v.y;
	temp_v[2] = v.z;
	temp_v[3] = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp_vm[i] += ximg[i][j] * temp_v[j];
		}
	}
	v_m.x = temp_vm[0] / temp_vm[3];
	v_m.y = temp_vm[1] / temp_vm[3];
	v_m.z = temp_vm[2] / temp_vm[3];
	return v_m;
}
//check triangle
int cut_tri(vector<Vertex> v, int xres, int yres) {
	int temp = 0, count = 0;
	for (int i = 0; i<3; i++) {
		if (v[i].z< 0) {
			temp = 1;
			break;
		}
		else if (v[i].x<0 || v[i].x>xres ||
			v[i].y<0 || v[i].y>yres) {
			count++;
		}
	}
	if (count == 3) {
		temp = 1;
	}
	return temp;

}

//extra func hw4
//trans norm to N with Xnorm
Vertex trs_n(Vertex v, GzMatrix xnorm, Vertex v_m) {
	//trans v to matrix 4*1;
	float temp_v[4], temp_vm[4] = { 0.0, 0.0, .0, .0 };
	temp_v[0] = v.x;
	temp_v[1] = v.y;
	temp_v[2] = v.z;
	temp_v[3] = 1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			temp_vm[i] += xnorm[i][j] * temp_v[j];
		}
	}
	v_m.x = temp_vm[0] / temp_vm[3];
	v_m.y = temp_vm[1] / temp_vm[3];
	v_m.z = temp_vm[2] / temp_vm[3];
	return v_m;
}
//compute A¡¤B(cos theta) with unit vector
float pointCos(GzCoord A, GzCoord B) {
	Axis_iw a, b;
	float cos;
	GzCoord temp = { 0, 0, 0};
	a.vector_iw(temp, A);
	b.vector_iw(temp, B);
	cos = (a.unit_x * b.unit_x + a.unit_y * b.unit_y + a.unit_z * b.unit_z) / a.iw_m*b.iw_m;
	return cos;
}
//compute A¡¤B 
float pointMul(GzCoord A, GzCoord B) {
	float result;
	result = A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
	return result;
}
//compute Reflection val
int computeR(Vertex v,GzLight light, GzCoord R) {
	GzCoord N, L;
	GzCoord temp = { 0,0,0 };
	N[0] = v.nx;
	N[1] = v.ny;
	N[2] = v.nz;
	L[0] = light.direction[0];
	L[1] = light.direction[1];
	L[2] = light.direction[2];
	
	Axis_iw n, l;
	n.vector_iw(temp, N);
	l.vector_iw(temp, L);

	float cosNL = pointCos(N, L);
	R[0] = 2 * cosNL*n.unit_x - l.unit_x;
	R[1] = 2 * cosNL*n.unit_y - l.unit_y;
	R[2] = 2 * cosNL*n.unit_z - l.unit_z;

		return GZ_SUCCESS;
}
//compute shading color with gotten shading data
void computeColor(GzRender *render, GzCoord color,GzLight lights[MAX_LIGHTS], Vertex v, GzCoord R, float spec) {
	GzCoord sumS = { 0,0,0 },
		sumD = { 0,0,0 },
		sumA = { 0,0,0 },
		E = { 0,0,-1 },
		N, L;
		N[0] = v.nx;
		N[1] = v.ny;
		N[2] = v.nz;
		float RE, NL,NE;
		//check nl,ne
		NE=	pointMul(N, E);
		for (int i = 0; i < render->numlights; i++) {
			//LightDirection L for each light
			L[0] = lights[i].direction[0];
			L[1] = lights[i].direction[1];
			L[2] = lights[i].direction[2];
			NL = pointMul(N, L);
			if(NL*NE<0){
				continue;
			}
			else if (NL*NE > 0 && NL < 0) {
				N[0] = -N[0];
				N[1] = -N[1];
				N[2] = -N[2];
				NL = -NL;
			}
			//check RE within[0,1]
			computeR(v, lights[i], R);
			RE = pointMul(R, E);
			if (RE > 1) { RE = 1; }
			else if (RE < 0) { RE = 0; }
			//sum
			sumS[0] +=(lights[i].color[0])*pow(RE,spec);
			sumS[1]+= (lights[i].color[1])*pow(RE, spec);
			sumS[2]+=(lights[i].color[2])*pow(RE, spec);
			sumD[0] += (lights[i].color[0])*NL;
			sumD[1] += (lights[i].color[1])*NL;
			sumD[2] += (lights[i].color[2])*NL;
	}
		color[0] = (render->Ks[0])*sumS[0] + (render->Kd[0])*sumD[0] + (render->Ka[0])*render->ambientlight.color[0];
		color[1] = (render->Ks[1])*sumS[1] + (render->Kd[1])*sumD[1] + (render->Ka[1])*render->ambientlight.color[1];
		color[2] = (render->Ks[2])*sumS[2] + (render->Kd[2])*sumD[2] + (render->Ka[2])*render->ambientlight.color[2];
}
int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
/* HW 3.1
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
*/
	float deg = (degree / 180)*PI;

	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = cos(deg);
	mat[1][2] = -sin(deg);
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = sin(deg);
	mat[2][2] = cos(deg);
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;


	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
/* HW 3.2
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
*/
	float deg = (degree / 180)*PI;

	mat[0][0] = cos(deg);
	mat[0][1] = 0;
	mat[0][2] = sin(deg);
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = -sin(deg);
	mat[2][1] = 0;
	mat[2][2] = cos(deg);
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
/* HW 3.3
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
*/
	float deg = (degree / 180)*PI;

	mat[0][0] = cos(deg);
	mat[0][1] = -sin(deg);
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = sin(deg);
	mat[1][1] = cos(deg);
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
/* HW 3.4
// Create translation matrix
// Pass back the matrix using mat value
*/
	mat[0][0] = 1;
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = translate[0];

	mat[1][0] = 0;
	mat[1][1] = 1;
	mat[1][2] = 0;
	mat[1][3] = translate[1];

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
	mat[2][3] = translate[2];

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;

	return GZ_SUCCESS;
}

int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
/* HW 3.5
// Create scaling matrix
// Pass back the matrix using mat value
*/
	mat[0][0] = scale[0];
	mat[0][1] = 0;
	mat[0][2] = 0;
	mat[0][3] = 0;

	mat[1][0] = 0;
	mat[1][1] = scale[1];
	mat[1][2] = 0;
	mat[1][3] = 0;

	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = scale[2];
	mat[2][3] = 0;

	mat[3][0] = 0;
	mat[3][1] = 0;
	mat[3][2] = 0;
	mat[3][3] = 1;

	return GZ_SUCCESS;
}

GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	if (xRes > MAXXRES) {
		xres = MAXXRES;
	}
	else if (xRes < 0) {
		xres = 0;
	}
	else {
		xres = xRes;
	}

	if (yRes > MAXYRES) {
		yres = MAXYRES;
	}
	else if (yRes < 0) {
		yres = 0;
	}
	else {
		yres = yRes;
	}

	int resolution = xres*yres;
	//framebuffer = new char[sizeof(char) * 3 * xres*yres];
	framebuffer = (char*) malloc (3 * sizeof(char) * xRes * yRes);
	pixelbuffer = new GzPixel[resolution];
/* HW 3.6
- setup Xsp and anything only done once 
- init default camera 
*/ 
	matlevel = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Xsp[i][j] = 0;
			m_camera.Xpi[i][j] = 0;
			m_camera.Xiw[i][j] = 0;
		}
	}

	m_camera.FOV = DEFAULT_FOV;
	m_camera.lookat[0] = 0.0;
	m_camera.lookat[1] = 0.0;
	m_camera.lookat[2] = 0.0;
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	m_camera.worldup[0] = 0.0;
	m_camera.worldup[1] = 1.0;
	m_camera.worldup[2] = 0.0;
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	delete pixelbuffer;
	delete framebuffer;
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	if (framebuffer == NULL) {
		return GZ_FAILURE;
	}
	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			pixelbuffer[ARRAY(i, j)].alpha = 1;
			pixelbuffer[ARRAY(i, j)].blue = 2550;
			pixelbuffer[ARRAY(i, j)].green = 2550;
			pixelbuffer[ARRAY(i, j)].red = 2550;
			pixelbuffer[ARRAY(i, j)].z = INT32_MAX;
		}

	}
	return GZ_SUCCESS;
}

int GzRender::GzBeginRender()
{
/* HW 3.7 
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 

//GzDefault();
	float degree = (m_camera.FOV / 180) * PI;
	float d = 1 / tan(degree / 2);

	Xsp[0][0] = Xsp[0][3] = xres / 2;
	Xsp[1][1] = -yres / 2;
	Xsp[1][3] = yres / 2;
	Xsp[2][2] = MAXINT;
	Xsp[3][3] = 1;
	Xsp[0][1] = Xsp[0][2] =
		Xsp[1][0] = Xsp[1][2] =
		Xsp[2][0] = Xsp[2][1] = Xsp[2][3] =
		Xsp[3][0] = Xsp[3][1] = Xsp[3][2] = 0;


	m_camera.Xpi[0][0] = 1.0;
	m_camera.Xpi[1][1] = 1.0;
	m_camera.Xpi[2][2] = 1 / d;
	m_camera.Xpi[3][2] = 1 / d;
	m_camera.Xpi[3][3] = 1.0;
	m_camera.Xpi[0][1] = m_camera.Xpi[0][2] = m_camera.Xpi[0][3] =
		m_camera.Xpi[1][0] = m_camera.Xpi[1][2] = m_camera.Xpi[1][3] =
		m_camera.Xpi[2][0] = m_camera.Xpi[2][1] = m_camera.Xpi[2][3] =
		m_camera.Xpi[3][0] = m_camera.Xpi[3][1] = 0;

	//setup Z axis,include z-origin and z-unit
	Axis_iw set_Z;
	set_Z.vector_iw(m_camera.position, m_camera.lookat);
	float mod_Z = set_Z.iw_m;

	//setup Up vector and mod
	Axis_iw set_up;
	GzCoord temp_set = { 0,0,0 };
	set_up.vector_iw(temp_set, m_camera.worldup);
	float mod_Up = set_up.iw_m;
	float cos_uz = (set_Z.x*set_up.x +
		set_Z.y*set_up.y +
		set_Z.z*set_up.z) / (mod_Z*mod_Up);
	GzCoord up_p;
	//get up prime
	up_p[0] = set_up.x - set_Z.unit_x*cos_uz;
	up_p[1] = set_up.y - set_Z.unit_y*cos_uz;
	up_p[2] = set_up.z - set_Z.unit_z*cos_uz;

	//set up Y axis
	Axis_iw set_Y;
	set_Y.vector_iw(temp_set, up_p);
	float mod_Y = set_up.iw_m;

	/*set up X axis
	 X = (Y x Z) */
	GzCoord axix_X;
	axix_X[0] = set_Y.unit_y*set_Z.unit_z -
		set_Y.unit_z*set_Z.unit_y;
	axix_X[1] = set_Y.unit_z*set_Z.unit_x -
		set_Y.unit_x*set_Z.unit_z;
	axix_X[2] = set_Y.unit_x*set_Z.unit_y -
		set_Y.unit_y*set_Z.unit_x;

	Axis_iw set_X;
	set_X.vector_iw(temp_set, axix_X);
	float mod_X = set_X.iw_m;
	float XC, YC, ZC;
	XC = set_X.unit_x *m_camera.position[0] +
		set_X.unit_y*m_camera.position[1] +
		set_X.unit_z*m_camera.position[2];
	YC = set_Y.unit_x*m_camera.position[0] +
		set_Y.unit_y*m_camera.position[1] +
		set_Y.unit_z*m_camera.position[2];
	ZC = set_Z.unit_x*m_camera.position[0] +
		set_Z.unit_y*m_camera.position[1] +
		set_Z.unit_z*m_camera.position[2];

	//set Xiw
	m_camera.Xiw[0][0] = set_X.unit_x;
	m_camera.Xiw[0][1] = set_X.unit_y;
	m_camera.Xiw[0][2] = set_X.unit_z;
	m_camera.Xiw[0][3] = -XC;
	m_camera.Xiw[1][0] = set_Y.unit_x;
	m_camera.Xiw[1][1] = set_Y.unit_y;
	m_camera.Xiw[1][2] = set_Y.unit_z;
	m_camera.Xiw[1][3] = -YC;
	m_camera.Xiw[2][0] = set_Z.unit_x;
	m_camera.Xiw[2][1] = set_Z.unit_y;
	m_camera.Xiw[2][2] = set_Z.unit_z;
	m_camera.Xiw[2][3] = -ZC;
	m_camera.Xiw[3][0] = 0.0;
	m_camera.Xiw[3][1] = 0.0;
	m_camera.Xiw[3][2] = 0.0;
	m_camera.Xiw[3][3] = 1.0;
	//push matrix into Xnorm and Ximage as initial stack
	GzPushMatrix(Xsp);
	GzPushMatrix(m_camera.Xpi);
	GzPushMatrix(m_camera.Xiw);
	return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
/* HW 3.8 
/*- overwrite renderer camera structure with new camera definition
*/
	m_camera.FOV = camera.FOV;
	m_camera.lookat[0] = camera.lookat[0];
	m_camera.lookat[1] = camera.lookat[1];
	m_camera.lookat[2] = camera.lookat[2];
	m_camera.position[0] = camera.position[0];
	m_camera.position[1] = camera.position[1];
	m_camera.position[2] = camera.position[2];
	m_camera.worldup[0] = camera.worldup[0];
	m_camera.worldup[1] = camera.worldup[1];
	m_camera.worldup[2] = camera.worldup[2];

	return GZ_SUCCESS;	
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
/* HW 3.9 
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	if (matlevel < MATLEVELS) {
		if (matlevel == 0) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Ximage[matlevel][i][j] = Xsp[i][j];
					Xnorm[matlevel][i][j] = 0;
				}
			}
			Xnorm[matlevel][0][0] = 1;
			Xnorm[matlevel][1][1] = 1;
			Xnorm[matlevel][2][2] = 1;
			Xnorm[matlevel][3][3] = 1;
		}
		else {
			
			Matrix_d4(Ximage[matlevel-1], matrix, Ximage[matlevel]);
			if (matlevel == 1) {
				//xnorm1
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						Xnorm[matlevel][i][j] = 0;
					}
				}
				Xnorm[matlevel][0][0] = 1;
				Xnorm[matlevel][1][1] = 1;
				Xnorm[matlevel][2][2] = 1;
				Xnorm[matlevel][3][3] = 1;
			}
			else {
				float temp;
				GzMatrix T;
				temp= 1/sqrt(matrix[0][0]*matrix[0][0]+matrix[1][0] * matrix[1][0] + matrix[2][0] * matrix[2][0]);
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++) {
						T[i][j] = matrix[i][j] * temp;
					}
					T[i][3] = 0;
					T[3][i] = 0;
				}
				T[3][3] = 1;
				Matrix_d4(Xnorm[matlevel - 1], T, Xnorm[matlevel]);
			}
		}
		matlevel = matlevel + 1;
	}
	else {
		return GZ_FAILURE;
	}
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
/* HW 3.10
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	if (matlevel > 0) {
		matlevel = matlevel - 1;
	}
	else {
		return GZ_FAILURE;
	}
	return GZ_SUCCESS;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	if (framebuffer == NULL) {
		return GZ_FAILURE;
	}
	if (i >= 0 && j >= 0 && i < xres && j < yres) {

		//blue
		if (b > MAXINTENSITY) {
			(pixelbuffer[ARRAY(i, j)]).blue = MAXINTENSITY;
		}
		else if (b < 0) {
			(pixelbuffer[ARRAY(i, j)]).blue = 0;
		}
		else {
			(pixelbuffer[ARRAY(i, j)]).blue = b;
		}
		//green
		if (g > MAXINTENSITY) {
			(pixelbuffer[ARRAY(i, j)]).green = MAXINTENSITY;
		}
		else if (g < 0) {
			(pixelbuffer[ARRAY(i, j)]).green = 0;
		}
		else {
			(pixelbuffer[ARRAY(i, j)]).green = g;
		}
		//red
		if (r > MAXINTENSITY) {
			(pixelbuffer[ARRAY(i, j)]).red = MAXINTENSITY;
		}
		else if (r < 0) {
			(pixelbuffer[ARRAY(i, j)]).red = 0;
		}
		else {
			(pixelbuffer[ARRAY(i, j)]).red = r;
		}

		(pixelbuffer[ARRAY(i, j)]).alpha = a;
		(pixelbuffer[ARRAY(i, j)]).z = z;

		return GZ_SUCCESS;
	}
	else {
		return GZ_FAILURE;
	}
}

int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	if (i >= 0 && j >= 0 && i < xres && j < yres) {

		*a = pixelbuffer[ARRAY(i, j)].alpha;
		*r = pixelbuffer[ARRAY(i, j)].red;
		*g = pixelbuffer[ARRAY(i, j)].green;
		*b = pixelbuffer[ARRAY(i, j)].blue;
		*z = pixelbuffer[ARRAY(i, j)].z;
		return GZ_SUCCESS;
	}
	else {
		return GZ_FAILURE;
	}
}

int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	if (outfile == NULL) {
		printf("File error!\n");
	}
	fprintf(outfile, "P6 %d %d 255\r", xres, yres);
	int i, j;
	for (i = 0; i < yres; i++) {
		for (j = 0; j < xres; j++) {
			char pb = pixelbuffer[ARRAY(j, i)].blue >> 4;
			char pg = pixelbuffer[ARRAY(j, i)].green >> 4;
			char pr = pixelbuffer[ARRAY(j, i)].red >> 4;
			fprintf(outfile, "%c%c%c", pr, pg, pb);
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	int i, j;
	for (i = 0; i < yres; i++) {
		for (j = 0; j < xres; j++) {
			char pbb = pixelbuffer[ARRAY(j, i)].blue >> 4;
			char pbg = pixelbuffer[ARRAY(j, i)].green >> 4;
			char pbr = pixelbuffer[ARRAY(j, i)].red >> 4;
			framebuffer[3 * ARRAY(j, i)] = pbb;
			framebuffer[3 * ARRAY(j, i) + 1] = pbg;
			framebuffer[3 * ARRAY(j, i) + 2] = pbr;
		}

	}
	return GZ_SUCCESS;
}

/***********************************************/
/* HW2 methods: implement from here */

int GzRender::GzPutAttribute(int numAttributes, GzToken	*nameList, GzPointer *valueList) 
{
/* HW 2.1
-- Set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
-- In later homeworks set shaders, interpolaters, texture maps, and lights
*/
	for (int i = 0; i < numAttributes; i++) {
		//gzrgbcolor flat shading
		if (nameList[i] == GZ_RGB_COLOR) {
			GzColor *tempcolor = (GzColor*)valueList[i];
			flatcolor[0] = (*tempcolor)[0];
			flatcolor[1] = (*tempcolor)[1];
			flatcolor[2] = (*tempcolor)[2];

		}
		//light: directional light: L:direction and le:color
		else if(nameList[i]==GZ_DIRECTIONAL_LIGHT){
			GzLight *templight = (GzLight*)valueList[i];
			if ( numlights >= MAX_LIGHTS) {
				return GZ_FAILURE;
			}
			lights[i].direction[0] = (*templight).direction[0];
			lights[i].direction[1] = (*templight).direction[1];
			lights[i].direction[2] = (*templight).direction[2];
			lights[i].color[0] = (*templight).color[0];
			lights[i].color[1] = (*templight).color[1];
			lights[i].color[2] = (*templight).color[2];
			numlights++;
		}
		else if (nameList[i] == GZ_AMBIENT_LIGHT) {
			GzLight *templight = (GzLight*)valueList[i];
			ambientlight.direction[0] = (*templight).direction[0];
			ambientlight.direction[1] = (*templight).direction[1];
			ambientlight.direction[2] = (*templight).direction[2];
			ambientlight.color[0] = (*templight).color[0];
			ambientlight.color[1] = (*templight).color[1];
			ambientlight.color[2] = (*templight).color[2];
		}
		//color
		else if(nameList[i] == GZ_INTERPOLATE){
		//gouraud or phong? tell interp_mode
			int *temp = (int*)valueList[i];
			interp_mode = *temp;
		}
		//coefficient Kd Ka Ks when i=0,2,3
		else if (nameList[i] == GZ_DIFFUSE_COEFFICIENT) {
			GzColor *tempcolor = (GzColor*)valueList[i];
			Kd[0] = (*tempcolor)[0];
			Kd[1] = (*tempcolor)[1];
			Kd[2] = (*tempcolor)[2];
		}
		else if(nameList[i]==GZ_AMBIENT_COEFFICIENT){
			GzColor *tempcolor = (GzColor*)valueList[i];
			Ka[0] = (*tempcolor)[0];
			Ka[1] = (*tempcolor)[1];
			Ka[2] = (*tempcolor)[2];
		}
		else if(nameList[i]==GZ_SPECULAR_COEFFICIENT){
			GzColor *tempcolor = (GzColor*)valueList[i];
			Ks[0] = (*tempcolor)[0];
			Ks[1] = (*tempcolor)[1];
			Ks[2] = (*tempcolor)[2];
		}
		else if (nameList[i] == GZ_DISTRIBUTION_COEFFICIENT) {
			float *tempcolor = (float*)valueList[i];
			spec = *tempcolor;
		}
		
	}
	return GZ_SUCCESS;
}

int GzRender::GzPutTriangle(int numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
/* HW 2.2
-- Pass in a triangle description with tokens and values corresponding to
      GZ_NULL_TOKEN:		do nothing - no values
      GZ_POSITION:		3 vert positions in model space
-- Invoke the rastrizer/scanline framework
-- Return error code
*/
	vector<Vertex> v(3);
	vector<Edge> e(3);
	for (int count = 0; count < numParts; count++) {
		//gzposition
		if (nameList[count] == GZ_POSITION) {
		
			//setup triangle
			GzCoord *CoTemp = (GzCoord*)valueList[count];
			Vertex t_v, t_vm;
			t_vm.x = 0;
			t_vm.y = 0;
			t_vm.z = 0;
			v.clear();

			for (int k = 0; k < 3; k++) {
				t_v.x = (*(CoTemp + k))[X];
				t_v.y = (*(CoTemp + k))[Y];
				t_v.z = (*(CoTemp + k))[Z];
				//trs_V to model
				t_vm = trs_v(t_v, Ximage[matlevel - 1], t_vm);
				//cut triangle out of screen;
				v.push_back(t_vm);
			}

			if (cut_tri(v, xres, yres) == 1) {
				continue;
			}
		}//normal pos
		else if(nameList[count] == GZ_NORMAL){
			//setup triangle normal
			GzCoord *CoTemp = (GzCoord*)valueList[count];
			Vertex t_nv, t_nvm;
			t_nvm.x = 0;
			t_nvm.y = 0;
			t_nvm.z = 0;
			//nomal coord
			for (int k = 0; k < 3; k++) {
				t_nv.x = (*(CoTemp + k))[X];
				t_nv.y = (*(CoTemp + k))[Y];
				t_nv.z = (*(CoTemp + k))[Z];
				//trs_V to model
				t_nvm = trs_n(t_nv, Xnorm[matlevel - 1], t_nvm);
				v[k].nx = t_nvm.x;
				v[k].ny = t_nvm.y;
				v[k].nz = t_nvm.z;
			}
		}
		else if (nameList[count] == GZ_NULL_TOKEN) {
			//do nothing
		}
	}	
	
	//sort, get direct, set Tris
	vector<Vertex> v_sort;
	v_sort = sortVertY(v, v_sort);
	int orient = CCDirection(v_sort);
	setTri(this,orient, e, v_sort);

	//scanLine method
	scan(this,orient, e, v_sort);

	return GZ_SUCCESS;
}

int scan(GzRender *render,int orient, vector<Edge>& e, vector<Vertex>& v_sort) {

	//default value
	Span span;
	float deltaY0, deltaY1;
	float deltaX;
	float deltaZ0, deltaZ1, deltaZ2;
	float slopeX0, slopeX1, slopeX2;
	float slopeZ0, slopeZ1, slopeZ2, slopeZ3; //Z3 for span
	//slope for new color;
	float slopeZ = 0;
	float slopeZR, slopeZG, slopeZB;

	int i = 0;
	int j = 0;
	GzIntensity red;
	GzIntensity green;
	GzIntensity blue;
	GzIntensity alpha;
	GzDepth depthZ;

	deltaX = 0;
	deltaY0 = ceil(v_sort[0].y) - v_sort[0].y;
	deltaY1 = ceil(v_sort[1].y) - v_sort[1].y;

	if (e.empty()) { return GZ_FAILURE; }
	else {

		//color slope r g b
		float slopecR0 = e[0].SlopeCR(); 
		float slopecR1 = e[1].SlopeCR();
		float slopecR2 = e[2].SlopeCR();

		float slopecG0 = e[0].SlopeCG(); 
		float slopecG1 = e[1].SlopeCG(); 
		float slopecG2 = e[2].SlopeCG(); 

		float slopecB0 = e[0].SlopeCB(); 
		float slopecB1 = e[1].SlopeCB(); 
		float slopecB2 = e[2].SlopeCB(); 
															
		//e0:
		slopeX0 = e[0].slopeX();
		slopeZ0 = e[0].slopeZ();
		e[0].current.x = e[0].start.x + slopeX0 * deltaY0;
		e[0].current.y = e[0].start.y + deltaY0;
		e[0].current.z = e[0].start.z + slopeZ0 * deltaY0;
		e[0].current.nx =e[0].start.nx + slopecR0 * deltaY0;
		e[0].current.ny =e[0].start.ny + slopecG0 * deltaY0;
		e[0].current.nz =e[0].start.nz + slopecB0 * deltaY0;

		//e2:
		slopeX2 = e[2].slopeX();
		slopeZ2 = e[2].slopeZ();
		e[2].current.x = e[2].start.x + slopeX2 * deltaY0;
		e[2].current.y = e[2].start.y + deltaY0;
		e[2].current.z = e[2].start.z + slopeZ2 * deltaY0;
		e[2].current.nx =e[2].start.nx + slopecR2 * deltaY0;
		e[2].current.ny =e[2].start.ny + slopecG2 * deltaY0;
		e[2].current.nz =e[2].start.nz + slopecB2 * deltaY0;
		
		//top half side : v[1]:mid point 1 3 + 4
		while (e[0].current.y < e[0].end.y) {
			if (orient == 1 || orient == 3) { //bottom and left l:E0 r:E2
				//x-line
				span.setEdge(e[0].current, e[2].current);
				deltaX = ceil(e[0].current.x) - e[0].current.x;
				//span slope
				slopeZ3 = span.slopeZ();
				slopeZR = span.SlopeZR();
				slopeZG = span.SlopeZG();
				slopeZB = span.SlopeZB();
				//x-line current loc
				span.current.x = e[0].current.x + deltaX;
				span.current.y = e[0].current.y;
				span.current.z = e[0].current.z + slopeZ3 * deltaX;
				span.current.nx =e[0].current.nx + slopeZR * deltaX ;
				span.current.ny =e[0].current.ny + slopeZG * deltaX  ;
				span.current.nz =e[0].current.nz + slopeZB * deltaX  ;

				while (span.current.x < e[2].current.x) {
					if (span.current.z < 0) {
						continue;
					}
					//render
					i = (int)span.current.x;
					j = (int)span.current.y;
					render->GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);
					if (depthZ == 0 || span.current.z < depthZ) {
						//phong: compute color at each pixel loc(i,j).
						if (render->interp_mode == GZ_NORMALS) {
							GzCoord color = { 0,0,0 };
							GzCoord R = { 0,0,0 };
							computeColor(render, color, render->lights, span.current, R, render->spec);
							render->GzPut(i, j, render->ctoi(color[0]), render->ctoi(color[1]), render->ctoi(color[2]), 0, (GzDepth)span.current.z);
						}
						//gouraud: use normal set at vertice as color
						else if (render->interp_mode == GZ_COLOR) {
							render->GzPut(i, j, render->ctoi(span.current.nx), render->ctoi(span.current.ny), render->ctoi(span.current.nz), 0, (GzDepth)span.current.z);
						}
						//flat render
						else {
							render->GzPut(i, j, render->ctoi(render->flatcolor[0]), render->ctoi(render->flatcolor[1]), render->ctoi(render->flatcolor[2]), 0, (GzDepth)span.current.z);
						}
					}
					span.current.x = span.current.x + 1;
					span.current.z = span.current.z + slopeZ3;
					span.current.nx = span.current.nx + slopeZR;
					span.current.ny = span.current.ny + slopeZG;
					span.current.nz = span.current.nz + slopeZB;
				}
			}
			else if (orient == 4) {//right l:E2 r:E0
				//x-line
				span.setEdge(e[2].current, e[0].current);
				deltaX = ceil(e[2].current.x) - e[2].current.x;
				slopeZ3 = span.slopeZ();
				slopeZR = span.SlopeZR();
				slopeZG = span.SlopeZG();
				slopeZB = span.SlopeZB();
				//set span cursor
				span.current.x = e[2].current.x + deltaX;
				span.current.y = e[2].current.y;
				span.current.z = e[2].current.z + slopeZ3 * deltaX;
				span.current.nx = e[2].current.nx + slopeZR * deltaX;
				span.current.ny = e[2].current.ny + slopeZG * deltaX;
				span.current.nz = e[2].current.nz + slopeZB * deltaX;

				while (span.current.x < e[0].current.x) {
					if (span.current.z < 0) {
						continue;
					}

					//render
					i = (int)span.current.x;
					j = (int)span.current.y;
					render->GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);
					if (depthZ == 0 || span.current.z < depthZ) {
						//phong
						if (render->interp_mode == GZ_NORMALS) {
							GzCoord color = {0,0,0};
							GzCoord R = {0,0,0};
							computeColor(render, color, render->lights, span.current, R, render->spec);
							render->GzPut(i, j, render->ctoi(color[0]), render->ctoi(color[1]), render->ctoi(color[2]), 0, (GzDepth)span.current.z);
						}
						//gouraud
						else if (render->interp_mode == GZ_COLOR) {
							render->GzPut(i, j, render->ctoi(span.current.nx), render->ctoi(span.current.ny), render->ctoi(span.current.nz), 0, (GzDepth)span.current.z);
						}
						//flat
						else{
							render->GzPut(i, j, render->ctoi(render->flatcolor[0]), render->ctoi(render->flatcolor[1]), render->ctoi(render->flatcolor[2]), 0, (GzDepth)span.current.z);
						}
					}
						
					span.current.x = span.current.x + 1;
					span.current.z = span.current.z + slopeZ3;
					span.current.nx = span.current.nx + slopeZR;
					span.current.ny = span.current.ny + slopeZG;
					span.current.nz = span.current.nz + slopeZB;
				}
			}
			else if (orient == 2) {
				break;
			}
			else {
				return GZ_FAILURE;
			}

			e[0].current.x = e[0].current.x + slopeX0;
			e[0].current.y = e[0].current.y + 1;
			e[0].current.z = e[0].current.z + slopeZ0;
			e[2].current.x = e[2].current.x + slopeX2;
			e[2].current.y = e[2].current.y + 1;
			e[2].current.z = e[2].current.z + slopeZ2;

			e[0].current.nx = e[0].current.nx + slopecR0;
			e[0].current.ny = e[0].current.ny + slopecG0;
			e[0].current.nz = e[0].current.nz + slopecB0;
			e[2].current.nx = e[2].current.nx + slopecR2;
			e[2].current.ny = e[2].current.ny + slopecG2;
			e[2].current.nz = e[2].current.nz + slopecB2;

		}

		//preparing for low half
		//e 1:
		slopeX1 = e[1].slopeX();
		slopeZ1 = e[1].slopeZ();
		e[1].current.x = e[1].start.x + slopeX1 * deltaY1;
		e[1].current.y = e[1].start.y + deltaY1;
		e[1].current.z = e[1].start.z + slopeZ1 * deltaY1;
		e[1].current.nx = e[1].start.nx + slopecR1 * deltaY1;
		e[1].current.ny = e[1].start.ny + slopecG1 * deltaY1;
		e[1].current.nz = e[1].start.nz + slopecB1 * deltaY1;

		//low half side:v[1]:mid point 2 3 +4
		while (e[1].current.y < e[1].end.y) {
			if (orient == 2 || orient == 3) { //bottom and left l:E1 r:E2
				//x-line
				span.setEdge(e[1].current, e[2].current);
				deltaX = ceil(e[1].current.x) - e[1].current.x;
				slopeZ3 = span.slopeZ();
				slopeZR = span.SlopeZR();
				slopeZG = span.SlopeZG();
				slopeZB = span.SlopeZB();
				//span cursor
				span.current.x = e[1].current.x + deltaX;
				span.current.y = e[1].current.y;
				span.current.z = e[1].current.z + slopeZ3 * deltaX;
				span.current.nx = e[1].current.nx + slopeZR * deltaX;
				span.current.ny = e[1].current.ny + slopeZG * deltaX;
				span.current.nz = e[1].current.nz + slopeZB * deltaX;

				while (span.current.x < e[2].current.x) {
					if (span.current.z < 0) {
						continue;
					}
					//render
					i = (int)span.current.x;
					j = (int)span.current.y; 
					render->GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);

					if (depthZ == 0 || span.current.z < depthZ) {
						//phong
						if (render->interp_mode == GZ_NORMALS) {
							GzCoord color = { 0,0,0 };
							GzCoord R = { 0,0,0 };
							computeColor(render, color, render->lights, span.current, R, render->spec);
							render->GzPut(i, j, render->ctoi(color[0]), render->ctoi(color[1]), render->ctoi(color[2]), 0, (GzDepth)span.current.z);
						}
						//gouraud
						else if (render->interp_mode == GZ_COLOR) {
							render->GzPut(i, j, render->ctoi(span.current.nx), render->ctoi(span.current.ny), render->ctoi(span.current.nz), 0, (GzDepth)span.current.z);
						}
						//flat
						else {
							render->GzPut(i, j, render->ctoi(render->flatcolor[0]), render->ctoi(render->flatcolor[1]), render->ctoi(render->flatcolor[2]), 0, (GzDepth)span.current.z);
						}
					}

					span.current.x = span.current.x + 1;
					span.current.z = span.current.z + slopeZ3;
					span.current.nx = span.current.nx + slopeZR;
					span.current.ny = span.current.ny + slopeZG;
					span.current.nz = span.current.nz + slopeZB;
				}
			}
			else if (orient == 4) {//right l:E2 r:E1
				//x-line
				span.setEdge(e[2].current, e[1].current);
				deltaX = ceil(e[2].current.x) - e[2].current.x;
				//span slope
				slopeZ3 = span.slopeZ();
				slopeZR = span.SlopeZR();
				slopeZG = span.SlopeZG();
				slopeZB = span.SlopeZB();
				//span cursor
				span.current.x = e[2].current.x + deltaX;
				span.current.y = e[2].current.y;
				span.current.z = e[2].current.z + slopeZ3 * deltaX;
				span.current.nx = e[2].current.nx + slopeZR * deltaX;
				span.current.ny = e[2].current.ny + slopeZG * deltaX;
				span.current.nz = e[2].current.nz + slopeZB * deltaX;

				while (span.current.x < e[1].current.x) {
					if (span.current.z < 0) {
						continue;
					}
					//render
					i = (int)span.current.x;
					j = (int)span.current.y;
					render->GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);

					if (depthZ == 0 || span.current.z < depthZ) {
						//phong
						if (render->interp_mode == GZ_NORMALS) {
							GzCoord color = { 0,0,0 };
							GzCoord R = { 0,0,0 };
							computeColor(render, color, render->lights, span.current, R, render->spec);
							render->GzPut(i, j, render->ctoi(color[0]), render->ctoi(color[1]), render->ctoi(color[2]), 0, (GzDepth)span.current.z);
						}
						//gouraud
						else if (render->interp_mode == GZ_COLOR) {
							render->GzPut(i, j, render->ctoi(span.current.nx), render->ctoi(span.current.ny), render->ctoi(span.current.nz), 0, (GzDepth)span.current.z);
						}
						//flat
						else {
							render->GzPut(i, j, render->ctoi(render->flatcolor[0]), render->ctoi(render->flatcolor[1]), render->ctoi(render->flatcolor[2]), 0, (GzDepth)span.current.z);
						}
					}
					
					span.current.x = span.current.x + 1;
					span.current.z = span.current.z + slopeZ3;
					span.current.nx = span.current.nx + slopeZR;
					span.current.ny = span.current.ny + slopeZG;
					span.current.nz = span.current.nz + slopeZB;
				}
			}
			else if (orient == 1) {
				break;
			}
			else {
				return GZ_FAILURE;
			}

			e[1].current.x = e[1].current.x + slopeX1;
			e[1].current.y = e[1].current.y + 1;
			e[1].current.z = e[1].current.z + slopeZ1;
			e[2].current.x = e[2].current.x + slopeX2;
			e[2].current.y = e[2].current.y + 1;
			e[2].current.z = e[2].current.z + slopeZ2;
			
			e[1].current.nx = e[1].current.nx + slopecR1;
			e[1].current.ny = e[1].current.ny + slopecG1;
			e[1].current.nz = e[1].current.nz + slopecB1;
			e[2].current.nx = e[2].current.nx + slopecR2;
			e[2].current.ny = e[2].current.ny + slopecG2;
			e[2].current.nz = e[2].current.nz + slopecB2;

		}
	}
}