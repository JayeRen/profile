/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
  if (u < 0) { u = 0; }
  else if (u > 1) { u = 1; }
  if (v < 0) { v = 0; }
  else if (v > 1) { v = 1; }
/* determine texture cell corner values and perform bilinear interpolation */
  float pu, pv;
  float s, t;
  pu = (xs-1)*u;
  pv =(ys-1)*v;
  s =pu- floor(pu);
  t =pv- floor(pv);
/* set color to interpolated GzColor value and return 
	A(floor floor) B(ceil floor)
	D(floor ceil) C(ceil ceil)
*/
  GzColor ColorA, ColorB, ColorC, ColorD, ColorSum;
  for (int i = 0; i < 3; i++) {
	  ColorA[i] = image[xs*(int)floor(pv)+(int)floor(pu)][i];
	  ColorB[i]= image[xs*(int)floor(pv) + (int)ceil(pu)][i];
	  ColorC[i]= image[xs*(int)ceil(pv) + (int)ceil(pu)][i];
	  ColorD[i]= image[xs*(int)ceil(pv) + (int)floor(pu)][i];

	  ColorSum[i] = s*t*ColorC[i] + (1 - s)*t*ColorD[i] +
		  s*(1 - t)*ColorB[i] + (1 - s)*(1 - t)*ColorA[i];
  }
  for (int j = 0; j < 3; j++) {
	  color[j] = ColorSum[j];
  }
	return GZ_SUCCESS;
}
#ifndef COMPLEXNUM
#define COMPLEXNUM
typedef struct ComplexNum
{
	float Xi;
	float Xr;

} ComplexNum;
#endif
/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	int N = 350;
	float lenX;
	
	ComplexNum x,x2,C;
	float temp;
	//initially
	int j;
	C.Xr = -0.012;
	C.Xi = 0.74;

	x.Xr = u*u-v*v+0.1 ;
	x.Xi = 2*u*v-0.6 ;
	for (j = 0; j < N; j++) {
		
		x2.Xr = x.Xr*x.Xr - x.Xi*x.Xi+C.Xr;
		x2.Xi = x.Xr*x.Xi + x.Xi*x.Xr+C.Xi;

		float temp = sqrt(pow(x2.Xr, 2) + pow(x2.Xi, 2));
		if (temp > 2.0) {
			break;
		}
		x.Xr = x2.Xr;
		x.Xi = x2.Xi;
	}
	lenX = sqrt(pow(x.Xr, 2) + pow(x.Xi, 2));
	
	if (j == N) {
			color[0] = color[1] = color[2] = lenX / 2;
		}
	else {
			//make a < 1;
			float a = ((float)j) / N;
			color[0] = 5 * a;
			color[1] = 5 * a;
			color[2] = 6 * a;
		}
	
	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}

