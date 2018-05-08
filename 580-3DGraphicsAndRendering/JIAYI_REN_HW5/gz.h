/*
 * Gz.h - include file for the cs580 rendering library
 */

/*
 * universal constants
 */
#define GZ_SUCCESS      0
#define GZ_FAILURE      1

/*
 * name list tokens
 */
#define GZ_NULL_TOKEN			0    /* triangle vert attributes */
#define GZ_POSITION             1
#define GZ_NORMAL               2
#define GZ_TEXTURE_INDEX        3

/* renderer-state default pixel color */
#define GZ_RGB_COLOR            99	

#define GZ_INTERPOLATE			95		/* interpolation mode */

#define GZ_DIRECTIONAL_LIGHT	79	/* directional light */
#define GZ_AMBIENT_LIGHT		78	/* ambient light type */

#define GZ_AMBIENT_COEFFICIENT		1001	/* Ka material property */
#define GZ_DIFFUSE_COEFFICIENT		1002	/* Kd material property */
#define GZ_SPECULAR_COEFFICIENT		1003	/* Ks material property */
#define GZ_DISTRIBUTION_COEFFICIENT	1004	/* specular power of material */

#define	GZ_TEXTURE_MAP	1010		/* texture function ptr */

/*
 * value-list attributes
 */

/* select interpolation mode of the shader */
#define GZ_FLAT			0	/* do flat shading with GZ_RBG_COLOR */
#define	GZ_COLOR		1	/* interpolate vertex color */
#define	GZ_NORMALS		2	/* interpolate normals */

typedef int     GzToken;
typedef void    *GzPointer;
typedef float   GzColor[3];
typedef short   GzIntensity;	/* 0-4095 in lower 12-bits for RGBA */
typedef float	GzCoord[3];
typedef float	GzTextureIndex[2];
typedef float	GzMatrix[4][4];
typedef int	GzDepth;		/* signed z for clipping */

typedef	int	(*GzTexture)(float u, float v, GzColor color);	/* pointer to texture lookup method */
/* u,v parameters [0,1] are defined tex_fun(float u, float v, GzColor color) */

/*
 * Gz camera definition
 */
#ifndef GZCAMERA
#define GZCAMERA
typedef struct  GzCamera
{
  GzMatrix			Xiw;  	/* xform from world to image space */
  GzMatrix			Xpi;     /* perspective projection xform */
  GzCoord			position;  /* position of image plane origin */
  GzCoord			lookat;         /* position of look-at-point */
  GzCoord			worldup;   /* world up-vector (almost screen up) */
  float				FOV;            /* horizontal field of view */
} GzCamera;
#endif

#ifndef GZLIGHT
#define GZLIGHT
typedef struct  GzLight
{
  GzCoord        direction;    /* vector from surface to light */
  GzColor        color;		/* light color intensity */
} GzLight;
#endif

#ifndef GZINPUT
#define GZINPUT
typedef struct  GzInput
{
	GzCoord         rotation;       /* object rotation */
	GzCoord			translation;	/* object translation */
	GzCoord			scale;			/* object scaling */
	GzCamera		camera;			/* camera */
} GzInput;
#endif

#define RED     0         /* array indicies for color vector */
#define GREEN   1
#define BLUE    2

#define X       0      /* array indicies for position vector */
#define Y       1
#define Z       2

#define U       0       /* array indicies for texture coords */
#define V       1


#ifndef GZ_PIXEL
typedef	struct {
  GzIntensity    red;	
  GzIntensity    green;
  GzIntensity    blue;
  GzIntensity    alpha;
  GzDepth	 z;
} GzPixel;
#define GZ_PIXEL
#endif;

#define	MAXXRES	1024	/* put some bounds on size in case of error */
#define	MAXYRES	1024

#ifndef VERTEX
typedef	struct {
	float x;
	float y;
	float z;
	float nx;
	float ny;
	float nz;
	float u, v;
	
} Vertex;
#define VERTEX
#endif;

#ifndef EDGE
class Edge {
public:
	Vertex start;
	Vertex end;
	float slopex;
	float slopez;
	//add color slope param
	float slopecr, slopecg, slopecb;
	//uv slope
	float slopeu,slopev;
	Vertex current;

	Edge() {}
	void setEdge(const Vertex &s, const Vertex &e) {
		start = s;
		end = e;
	}
	float slopeX() {
		slopex = (end.x - start.x) / (end.y - start.y);
		return slopex;
	}
	float slopeZ() {
		slopez = (end.z - start.z) / (end.y - start.y);
		return slopez;
	}
	float SlopeCR()
	{
		slopecr = (end.nx - start.nx) / (end.y - start.y);
		return  slopecr;
	}
	float SlopeCG()
	{
		slopecg = (end.ny - start.ny) / (end.y - start.y);
		return slopecg;
	}
	float SlopeCB()
	{
		slopecb = (end.nz - start.nz) / (end.y - start.y);
		return slopecb;
	}
	float SlopeU() {
		slopeu = (end.u - start.u) / (end.y - start.y);
		return slopeu;
	}
	float SlopeV() {
		slopev = (end.v - start.v) / (end.y - start.y);
		return slopev;
	}
};
#define EDGE
#endif;

#ifndef SPAN
class Span {
public:
	Vertex start;
	Vertex end;
	float slopez, slopezr, slopezg, slopezb;
	float slopezu, slopezv;
	Vertex current;

	Span() {}
	void setEdge(Vertex s, Vertex e) {
		start = s;
		end = e;
	}

	float slopeZ() {
		slopez = (end.z - start.z) / (end.x - start.x);
		return slopez;
	}
	float SlopeZR()
	{
		slopezr = (end.nx - start.nx) / (end.x - start.x);
		return slopezr;
	}
	float SlopeZG()
	{
		slopezg = (end.ny - start.ny) / (end.x - start.x);
		return slopezg;
	}
	float SlopeZB()
	{
		slopezb = (end.nz - start.nz) / (end.x - start.x);
		return slopezb;
	}
	float SlopeZU() {
		slopezu = (end.u - start.u) / (end.x - start.x);
		return slopezu;
	}
	float SlopeZV() {
		slopezv = (end.v - start.v) / (end.x - start.x);
		return slopezv;
	}

};
#define SPAN
#endif;

#ifndef AXIS_IW
class Axis_iw {
public:

	float x;
	float y;
	float z;

	float unit_x;
	float unit_y;
	float unit_z;

	float iw_m;
	//origin vector
	void vector_iw(GzCoord c, GzCoord l) {
		if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
			x = l[0];
			y = l[1];
			z = l[2];
		}
		else {
			x = l[0] - c[0];
			y = l[1] - c[1];
			z = l[2] - c[2];
		}
		//sqrt mod of vector
		iw_m = (float)sqrt(pow(x, 2) +
			pow(y, 2) +
			pow(z, 2)
		);
		//unit vector
		unit_x = x / iw_m;
		unit_y = y / iw_m;
		unit_z = z / iw_m;

	}
	Axis_iw() {}
};
#define AXIS_IW
#endif;
