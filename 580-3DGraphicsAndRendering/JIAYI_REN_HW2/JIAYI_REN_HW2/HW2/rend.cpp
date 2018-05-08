#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#include<vector>
using std::vector;

#define MAXINTENSITY 4095
/***********************************************/
/* HW1 methods: copy here the methods from HW1 */

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
	framebuffer = new char[sizeof(char) * 3 * xres*yres];
	pixelbuffer = new GzPixel[resolution];
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
	fprintf(outfile, "p6 %d %d 255\n", xres, yres);
	int i, j;
	for (i = 0; i < yres; i++) {
		for (j = 0; j < xres; j++) {
			char pb = pixelbuffer[ARRAY(j, i)].blue >> 4;
			char pg = pixelbuffer[ARRAY(j, i)].green >> 4;
			char pr = pixelbuffer[ARRAY(j, i)].red >> 4;
			fprintf(outfile, "%c%c%c", pb, pg, pr);
			//fwrite(framebuffer, xres*yres, 3, outfile);
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
		//gzrgbcolor
		if (nameList[i] == GZ_RGB_COLOR) {
			GzColor *tempcolor = (GzColor*)valueList[i];
			flatcolor[0] = (*tempcolor)[0];
			flatcolor[1] = (*tempcolor)[1];
			flatcolor[2] = (*tempcolor)[2];
		}
	}
	return GZ_SUCCESS;
}

void sortVertY(vector<Vertex> vertex, vector<Vertex> temp) {
	int max = 0, min=0,mid = 0;
	for (int i = 0; i < 3; i++) {
		if (vertex[i].y > vertex[max].y) {
			max = i;
		}
		else if (vertex[i].y < vertex[min].y) {
			min = i;
		}
	}
	for (int m = 0; m < 3; m++) {
		if (m != min && m != max) {
			mid = m;
		}
	}
	temp.push_back(vertex[max]);
	temp.push_back(vertex[mid]);
	temp.push_back(vertex[min]);
}
inline int CCDirection(vector<Vertex> vertex) {
	float d, z, x2;
	if (vertex.empty()) {
		return GZ_FAILURE;
	}
	else {
		d = (vertex[0].y - vertex[2].y) / (vertex[0].x - vertex[2].x);
		z = vertex[0].y - d * vertex[0].x;
		x2 = (vertex[1].y-z) / d;
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
//edge e[0] 0-1 e[1] 1-2  e[2] 0-2
int setTri(int orient, vector<Edge>& edge, vector<Vertex>& v) {
	if (v.empty()||edge.empty()) {
		return GZ_FAILURE;
	}
	else {
			edge[0].setEdge(v[0], v[1]);
			edge[1].setEdge(v[1], v[2]);
			edge[2].setEdge(v[0], v[2]);
	}	
	return GZ_SUCCESS;
}
int GzRender::GzPutTriangle(int	numParts, GzToken *nameList, GzPointer *valueList)
/* numParts - how many names and values */
{
	/* HW 2.2
	-- Pass in a triangle description with tokens and values corresponding to
		  GZ_NULL_TOKEN:		do nothing - no values
		  GZ_POSITION:		3 vert positions in model space
	-- Invoke the rastrizer/scanline framework
	-- Return error code
	*/
	for (int count = 0; count < numParts; count++) {
		//gzposition
		vector<Vertex> v(3);
		vector<Edge> e(3);
		if (nameList[count] == GZ_POSITION) {

			//setup triangle
			GzCoord *CoTemp = (GzCoord*)valueList[count];
			Vertex t_v;
			v.clear();
			//e.clear();
			for (int k = 0; k < 3; k++) {
				t_v.x = (*(CoTemp + k))[X];
				t_v.y = (*(CoTemp + k))[Y];
				t_v.z = (*(CoTemp + k))[Z];
				v.push_back(t_v);
			}

			//sort
			vector<Vertex> v_sort;
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

			int orient = CCDirection(v_sort);
			setTri(orient, e, v_sort);

			//scan
			//default value
			Span span;
			float deltaY0,deltaY1;
			float deltaX;
			float deltaZ0,deltaZ1, deltaZ2;
			float slopeX0, slopeX1, slopeX2;
			float slopeZ0, slopeZ1, slopeZ2, slopeZ3;//Z3 for span

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
				
				//edge 0:
				slopeX0 = e[0].slopeX();
				slopeZ0 = e[0].slopeZ();
				e[0].current.x = e[0].start.x + slopeX0*deltaY0;
				e[0].current.y = e[0].start.y + deltaY0;
				e[0].current.z = e[0].start.z + slopeZ0*deltaY0;
				
				//edge 2:
				slopeX2 = e[2].slopeX();
				slopeZ2 = e[2].slopeZ();
				e[2].current.x = e[2].start.x + slopeX2*deltaY0;
				e[2].current.y = e[2].start.y + deltaY0;
				e[2].current.z = e[2].start.z + slopeZ2*deltaY0;

				

				//top half side : v[1]:mid point 1 3+4
				while (e[0].current.y < e[0].end.y) {
					if (orient == 1 || orient == 3) { //bottom and left l:E0 r:E2
						//x-line
						span.setEdge(e[0].current, e[2].current);
						deltaX = ceil(e[0].current.x) - e[0].current.x;
						slopeZ3 = (e[2].current.z - e[0].current.z) / (e[2].current.x - e[0].current.x);

						span.current.x = e[0].current.x + deltaX;
						span.current.y = e[0].current.y;
						span.current.z = e[0].current.z + deltaX*slopeZ3;
						while (span.current.x < e[2].current.x) {
							if (span.current.z < 0) {
								continue;
							}
							//render
							i = (int)span.current.x;
							j = (int)span.current.y;

							GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);
							if (depthZ == 0 || span.current.z < depthZ) {
								
								GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 0, (GzDepth)span.current.z);
							}
							span.current.x= span.current.x + 1;
							span.current.z = span.current.z + slopeZ3;
						}
					}
					else if (orient == 4) {//right l:E2 r:E0
										   //x-line
						span.setEdge(e[2].current, e[0].current);
						deltaX = ceil(e[2].current.x) - e[2].current.x;
						slopeZ3 = (e[0].current.z - e[2].current.z) / (e[0].current.x - e[2].current.x);

						span.current.x = e[2].current.x + deltaX;
						span.current.y = e[2].current.y;
						span.current.z = e[2].current.z + deltaX*slopeZ3;
						while (span.current.x < e[0].current.x) {
							if (span.current.z < 0) {
								continue;
							}
							//render
							i = (int)span.current.x;
							j = (int)span.current.y;

							GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);
							if (depthZ == 0 || span.current.z < depthZ) {
						
								GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 0, (GzDepth)span.current.z);
							}
							span.current.x = span.current.x + 1;
							span.current.z = span.current.z + slopeZ3;
						}
					}
					else if (orient == 2) {
						break;
					}
					else {
						return GZ_FAILURE;
					}
					
					e[0].current.x = e[0].current.x + slopeX0;
					e[0].current.y = e[0].current.y +1;
					e[0].current.z = e[0].current.z + slopeZ0;
					e[2].current.x = e[2].current.x + slopeX2;
					e[2].current.y = e[2].current.y +1;
					e[2].current.z= e[2].current.z + slopeZ2;
							
				}

				//edge 1:
				slopeX1 = e[1].slopeX();
				slopeZ1 = e[1].slopeZ();
				e[1].current.x = e[1].start.x + slopeX1*deltaY1;
				e[1].current.y = e[1].start.y + deltaY1;
				e[1].current.z = e[1].start.z + slopeZ1*deltaY1;
				
				//low half side:v[1]:mid point 2 3 +4
				while (e[1].current.y < e[1].end.y) {
					if (orient == 2 || orient == 3) { //bottom and left l:E1 r:E2
													  //x-line
						
						span.setEdge(e[1].current, e[2].current);
						deltaX = ceil(e[1].current.x) - e[1].current.x;
						slopeZ3 = (e[2].current.z - e[1].current.z) / (e[2].current.x - e[1].current.x);

						span.current.x = e[1].current.x + deltaX;
						span.current.y = e[1].current.y;
						span.current.z = e[1].current.z + deltaX*slopeZ3;
						while (span.current.x < e[2].current.x) {
							if (span.current.z < 0) {
								continue;
							}
							//render
							i = (int)span.current.x;
							j = (int)span.current.y; //choose curve

							GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);
							if (depthZ == 0 || span.current.z < depthZ) {
								
								GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 0,(GzDepth)span.current.z);
							}
							span.current.x = span.current.x +1;
							span.current.z = span.current.z +slopeZ3;
						}
					}
					else if (orient == 4) {//right l:E2 r:E1
										   //x-line
						span.setEdge(e[2].current, e[1].current);
						deltaX = ceil(e[2].current.x) - e[2].current.x;
						slopeZ3 = (e[1].current.z - e[2].current.z) / (e[1].current.x - e[2].current.x);

						span.current.x = e[2].current.x + deltaX;
						span.current.y = e[2].current.y;
						span.current.z = e[2].current.z + deltaX*slopeZ3;
						while (span.current.x < e[1].current.x) {
							if (span.current.z < 0) {
								continue;
							}
							//render
							i = (int)span.current.x;
							j = (int)span.current.y;

							GzGet(i, j, &red, &green, &blue, &alpha, &depthZ);
							if (depthZ == 0 || span.current.z < depthZ) {
								GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 0,(GzDepth)span.current.z);
							}
							span.current.x= span.current.x +1;
							span.current.z = span.current.z +slopeZ3;
						}
					}
					else if (orient == 1) {
						break;
					}
					else {
						return GZ_FAILURE;
					}

					
					e[1].current.x = e[1].current.x +slopeX1;
					e[1].current.y= e[1].current.y +1;
					e[1].current.z = e[1].current.z + slopeZ1;
					e[2].current.x = e[2].current.x + slopeX2;
					e[2].current.y= e[2].current.y +1;
					e[2].current.z = e[2].current.z + slopeZ2;

				}

			}
			}//gznulltoken
		else if (nameList[count] == GZ_NULL_TOKEN) {
			//do nothing
		}
		}
	
	return GZ_SUCCESS;
	
}


