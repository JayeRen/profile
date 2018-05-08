// Application5.cpp: implementation of the Application5 class.
//
//////////////////////////////////////////////////////////////////////

/*
 * application test code for homework assignment #5
*/

#include "stdafx.h"
#include "CS580HW.h"
#include "Application5.h"
#include "Gz.h"
#include "rend.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INFILE  "ppot.asc"
#define OUTFILE "output.ppm"


extern int tex_fun(float u, float v, GzColor color); /* image texture function */
extern int ptex_fun(float u, float v, GzColor color); /* procedural texture function */
extern int GzFreeTexture();

void shade(GzCoord norm, GzCoord color);
void sumall(GzRender *render, GzRender *subrender[AAKERNEL_SIZE]);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*sub-sample*/

float AAFilter[AAKERNEL_SIZE][3] 		/* X-shift, Y-shift, weight */
{
	-0.52, 0.38, 0.128, 0.41, 0.56, 0.119, 0.27, 0.08, 0.294,
	-0.17, -0.29, 0.249, 0.58, -0.55, 0.104, -0.31, -0.71, 0.106
};


Application5::Application5()
{

}

Application5::~Application5()
{
	Clean();
}

int Application5::Initialize()
{
	GzCamera	camera;  
	int		    xRes, yRes;	/* display parameters */ 

	GzToken		nameListShader[9]; 	    /* shader attribute names */
	GzPointer   valueListShader[9];		/* shader attribute pointers */
	GzToken     nameListLights[10];		/* light info */
	GzPointer   valueListLights[10];
	int			shaderType, interpStyle;
	float		specpower;
	int		status; 
 
	status = 0; 

	/* 
	 * Allocate memory for user input
	 */
	m_pUserInput = new GzInput;

	/* 
	 * initialize the display and the renderer 
	 */ 
 	m_nWidth = 256;		// frame buffer and display width
	m_nHeight = 256;    // frame buffer and display height

	
		m_pRender = new GzRender(m_nWidth, m_nHeight);
		m_pRender->GzDefault();

		m_pFrameBuffer = m_pRender->framebuffer;

		/* Translation matrix */
		GzMatrix	scale =
		{
			3.25,	0.0,	0.0,	0.0,
			0.0,	3.25,	0.0,	-3.25,
			0.0,	0.0,	3.25,	3.5,
			0.0,	0.0,	0.0,	1.0
		};

		GzMatrix	rotateX =
		{
			1.0,	0.0,	0.0,	0.0,
			0.0,	.7071,	.7071,	0.0,
			0.0,	-.7071,	.7071,	0.0,
			0.0,	0.0,	0.0,	1.0
		};

		GzMatrix	rotateY =
		{
			.866,	0.0,	-0.5,	0.0,
			0.0,	1.0,	0.0,	0.0,
			0.5,	0.0,	.866,	0.0,
			0.0,	0.0,	0.0,	1.0
		};

		for(int i=0;i<AAKERNEL_SIZE;i++){
		m_subRender[i] = new GzRender(m_nWidth, m_nHeight);
		m_subRender[i]->GzDefault();

		m_subFrameBuffer[i] = m_subRender[i]->framebuffer;

#if 1 	/* set up app-defined camera if desired, else use camera defaults */
		camera.position[X] = -3;
		camera.position[Y] = -25;
		camera.position[Z] = -4;

		camera.lookat[X] = 7.8;
		camera.lookat[Y] = 0.7;
		camera.lookat[Z] = 6.5;

		camera.worldup[X] = -0.2;
		camera.worldup[Y] = 1.0;
		camera.worldup[Z] = 0.0;

		camera.FOV = 63.7;              /* degrees *              /* degrees */

		status |= m_subRender[i]->GzPutCamera(camera);
#endif 

		/* Start Renderer */
		status |= m_subRender[i]->GzBeginRender();

		/* Light */
		GzLight	light1 = { {-0.7071, 0.7071, 0}, {0.5, 0.5, 0.9} };
		GzLight	light2 = { {0, -0.7071, -0.7071}, {0.9, 0.2, 0.3} };
		GzLight	light3 = { {0.7071, 0.0, -0.7071}, {0.2, 0.7, 0.3} };
		GzLight	ambientlight = { {0, 0, 0}, {0.3, 0.3, 0.3} };

		/* Material property */
		GzColor specularCoefficient = { 0.3, 0.3, 0.3 };
		GzColor ambientCoefficient = { 0.1, 0.1, 0.1 };
		GzColor diffuseCoefficient = { 0.7, 0.7, 0.7 };

		/*
		  renderer is ready for frame --- define lights and shader at start of frame
		*/

		/*
		 * Tokens associated with light parameters
		 */
		nameListLights[0] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[0] = (GzPointer)&light1;
		nameListLights[1] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[1] = (GzPointer)&light2;
		nameListLights[2] = GZ_DIRECTIONAL_LIGHT;
		valueListLights[2] = (GzPointer)&light3;
		status |= m_subRender[i]->GzPutAttribute(3, nameListLights, valueListLights);

		nameListLights[0] = GZ_AMBIENT_LIGHT;
		valueListLights[0] = (GzPointer)&ambientlight;
		status |= m_subRender[i]->GzPutAttribute(1, nameListLights, valueListLights);

		/*
		 * Tokens associated with shading
		 */
		nameListShader[0] = GZ_DIFFUSE_COEFFICIENT;
		valueListShader[0] = (GzPointer)diffuseCoefficient;

		/*
		* Select either GZ_COLOR or GZ_NORMALS as interpolation mode
		*/
		nameListShader[1] = GZ_INTERPOLATE;
		//interpStyle = GZ_COLOR;         /* Gouraud shading */
		interpStyle = GZ_NORMALS;         /* Phong shading */
		valueListShader[1] = (GzPointer)&interpStyle;

		nameListShader[2] = GZ_AMBIENT_COEFFICIENT;
		valueListShader[2] = (GzPointer)ambientCoefficient;
		nameListShader[3] = GZ_SPECULAR_COEFFICIENT;
		valueListShader[3] = (GzPointer)specularCoefficient;
		nameListShader[4] = GZ_DISTRIBUTION_COEFFICIENT;
		specpower = 32;
		valueListShader[4] = (GzPointer)&specpower;

		nameListShader[5] = GZ_TEXTURE_MAP;
#if 0   /* set up null texture function or valid pointer */
		valueListShader[5] = (GzPointer)0;
#else
		valueListShader[5] = (GzPointer)(tex_fun);	/* or use ptex_fun */
#endif
		status |= m_subRender[i]->GzPutAttribute(6, nameListShader, valueListShader);

		nameListShader[0] = GZ_AASHIFTX;
		valueListShader[0] = (GzPointer)&AAFilter[i][0];
		nameListShader[1] = GZ_AASHIFTY;
		valueListShader[0] = (GzPointer)&AAFilter[i][1];
		status |= m_subRender[i]->GzPutAttribute(2, nameListShader, valueListShader);


		status |= m_subRender[i]->GzPushMatrix(scale);
		status |= m_subRender[i]->GzPushMatrix(rotateY);
		status |= m_subRender[i]->GzPushMatrix(rotateX);
	}
	if (status) exit(GZ_FAILURE); 

	if (status) 
		return(GZ_FAILURE); 
	else 
		return(GZ_SUCCESS); 
}

int Application5::Render() 
{
	GzToken		nameListTriangle[3]; 	/* vertex attribute names */
	GzPointer	valueListTriangle[3]; 	/* vertex attribute pointers */
	GzCoord		vertexList[3];	/* vertex position coordinates */ 
	GzCoord		normalList[3];	/* vertex normals */ 
	GzTextureIndex  	uvList[3];		/* vertex texture map indices */ 
	char		dummy[256]; 
	int			status; 


	/* Initialize Display */
	for (int i = 0; i < AAKERNEL_SIZE; i++) {
		status |= m_subRender[i]->GzDefault();  /* init for new frame */
	}
	/* 
	* Tokens associated with triangle vertex values 
	*/ 
	nameListTriangle[0] = GZ_POSITION; 
	nameListTriangle[1] = GZ_NORMAL; 
	nameListTriangle[2] = GZ_TEXTURE_INDEX;  

	// I/O File open
	FILE *infile;
	if( (infile  = fopen( INFILE , "r" )) == NULL )
	{
         AfxMessageBox( "The input file was not opened\n" );
		 return GZ_FAILURE;
	}

	FILE *outfile;
	if( (outfile  = fopen( OUTFILE , "wb" )) == NULL )
	{
         AfxMessageBox( "The output file was not opened\n" );
		 return GZ_FAILURE;
	}

	/* 
	* Walk through the list of triangles, set color 
	* and render each triangle 
	*/ 
	while( fscanf(infile, "%s", dummy) == 1) { 	/* read in tri word */
	    fscanf(infile, "%f %f %f %f %f %f %f %f", 
		&(vertexList[0][0]), &(vertexList[0][1]),  
		&(vertexList[0][2]), 
		&(normalList[0][0]), &(normalList[0][1]), 	
		&(normalList[0][2]), 
		&(uvList[0][0]), &(uvList[0][1]) ); 
	    fscanf(infile, "%f %f %f %f %f %f %f %f", 
		&(vertexList[1][0]), &(vertexList[1][1]), 	
		&(vertexList[1][2]), 
		&(normalList[1][0]), &(normalList[1][1]), 	
		&(normalList[1][2]), 
		&(uvList[1][0]), &(uvList[1][1]) ); 
	    fscanf(infile, "%f %f %f %f %f %f %f %f", 
		&(vertexList[2][0]), &(vertexList[2][1]), 	
		&(vertexList[2][2]), 
		&(normalList[2][0]), &(normalList[2][1]), 	
		&(normalList[2][2]), 
		&(uvList[2][0]), &(uvList[2][1]) ); 

	    /* 
	     * Set the value pointers to the first vertex of the 	
	     * triangle, then feed it to the renderer 
	     * NOTE: this sequence matches the nameList token sequence
	     */ 
	     valueListTriangle[0] = (GzPointer)vertexList; 
		 valueListTriangle[1] = (GzPointer)normalList; 
		 valueListTriangle[2] = (GzPointer)uvList;
		for (int i = 0; i < AAKERNEL_SIZE; i++) {
			 m_subRender[i]->GzPutTriangle(3, nameListTriangle, valueListTriangle);
		 }
	} 
	sumall(m_pRender,m_subRender);
	m_pRender->GzFlushDisplay2File(outfile); 	/* write out or update display to file*/
	m_pRender->GzFlushDisplay2FrameBuffer();	// write out or update display to frame buffer

	/* 
	 * Close file
	 */ 

	if( fclose( infile ) )
      AfxMessageBox(_T( "The input file was not closed\n" ));

	if( fclose( outfile ) )
      AfxMessageBox(_T( "The output file was not closed\n" ));
 
	if (status) 
		return(GZ_FAILURE); 
	else 
		return(GZ_SUCCESS); 
}

int Application5::Clean()
{
	/* 
	 * Clean up and exit 
	 */ 
	int	status = 0; 
	for (int i = 0; i < AAKERNEL_SIZE; i++) {
		free(m_subRender[i]);
	}
	
	free(m_pRender);
	status |= GzFreeTexture();
	
	if (status) 
		return(GZ_FAILURE); 
	else 
		return(GZ_SUCCESS);
}

void sumall(GzRender *render, GzRender *subrender[AAKERNEL_SIZE]) {
	for (int i = 0; i < render->xres; i++) {
		for (int j = 0; j < render->yres; j++) {
			render->pixelbuffer[render->ARRAY(i, j)].red = 0;
			render->pixelbuffer[render->ARRAY(i, j)].green = 0;
			render->pixelbuffer[render->ARRAY(i, j)].blue = 0;
			render->pixelbuffer[render->ARRAY(i, j)].alpha = 0;
			render->pixelbuffer[render->ARRAY(i, j)].z = 0;
			for (int k = 0; k < AAKERNEL_SIZE; k++) {
				render->pixelbuffer[render->ARRAY(i, j)].red = render->pixelbuffer[render->ARRAY(i, j)].red+
					subrender[k]->pixelbuffer[subrender[k]->ARRAY(i, j)].red*AAFilter[k][2];
				render->pixelbuffer[render->ARRAY(i, j)].green= render->pixelbuffer[render->ARRAY(i, j)].green +
					subrender[k]->pixelbuffer[subrender[k]->ARRAY(i, j)].green*AAFilter[k][2];
				render->pixelbuffer[render->ARRAY(i, j)].blue = render->pixelbuffer[render->ARRAY(i, j)].blue +
					subrender[k]->pixelbuffer[subrender[k]->ARRAY(i, j)].blue*AAFilter[k][2];
				render->pixelbuffer[render->ARRAY(i, j)].alpha = render->pixelbuffer[render->ARRAY(i, j)].alpha +
					subrender[k]->pixelbuffer[subrender[k]->ARRAY(i, j)].alpha*AAFilter[k][2];
				render->pixelbuffer[render->ARRAY(i, j)].z = render->pixelbuffer[render->ARRAY(i, j)].z +
					subrender[k]->pixelbuffer[subrender[k]->ARRAY(i, j)].z*AAFilter[k][2];
			}
		}
	}
	
}


