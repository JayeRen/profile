#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"

#define MAXINTENSITY 4095

GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	
	if (xRes > MAXXRES) {
		this->xres = MAXXRES;
	}
	else if (xRes < 0) {
		this->xres = 0;
	}
	else {
		this->xres = xRes;
	}

	if (yRes > MAXYRES) {
		this->yres = MAXYRES;
	}
	else if (yRes < 0) {
		this->yres = 0;
	}
	else {
		this->yres = yRes;
	}

	int resolution = xres*yres;
	framebuffer = new char [sizeof(char) * 3 * xres*yres];
	pixelbuffer= new GzPixel[resolution];
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
			pixelbuffer[ARRAY(i, j)].z = 0;
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
	if (i >= 0 && j >= 0 && i <= xres && j <= yres) {

		*a = pixelbuffer[ARRAY(i,j)].alpha;
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
	fprintf(outfile, "p6 %d %d 255\n",xres,yres);
	int i, j;
	for (i = 0; i < yres; i++) {
		for (j = 0; j < xres; j++) {
			char pb = pixelbuffer[ARRAY(j, i)].blue>>4;
			char pg = pixelbuffer[ARRAY(j, i)].green>>4;
			char pr = pixelbuffer[ARRAY(j, i)].red>>4;
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
	int i,j;
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