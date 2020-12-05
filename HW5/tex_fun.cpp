#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include<algorithm>

GzColor	*image = NULL;
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
		fd = fopen("texture", "rb");
		if (fd == NULL) {
			fprintf(stderr, "texture file not found\n");
			exit(-1);
		}
		fscanf(fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
		image = (GzColor*)malloc(sizeof(GzColor)*(xs + 1)*(ys + 1));
		if (image == NULL) {
			fprintf(stderr, "malloc for texture image failed\n");
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
	
	
	u = std::clamp(u,(float) 0, (float)1);
	v = std::clamp(v, (float)0, (float)1);
	u *= (xs - 1);
	v *= (ys - 1);

	/* determine texture cell corner values and perform bilinear interpolation */
	int  x1 = (int)floor(u) , x2 = (int)ceil(u) , y1 = (int)floor(v) , y2 = (int)ceil(v);

	float s = u - x1, t = v - y1;

	/* set color to interpolated GzColor value and return */
	float colorAtA[] = { image[y1*xs + x1][0],image[y1*xs + x1][1],image[y1*xs + x1][2] };
	float colorAtB[] = { image[y1*xs + x2][0],image[y1*xs + x2][1],image[y1*xs + x2][2] };
	float colorAtC[] = { image[y2*xs + x2][0],image[y2*xs + x2][1],image[y2*xs + x2][2] };
	float colorAtD[] = { image[y2*xs + x1][0],image[y2*xs + x1][1],image[y2*xs + x1][2] };

	for (int ind = 0; ind < 3; ind++) {
		color[ind] = s * t * colorAtC[ind] + (1.0f - s) * t * colorAtD[ind] + s * (1.0f - t) * colorAtB[ind] + (1.0f - s) * (1.0f - t) * colorAtA[ind];
	}

	return GZ_SUCCESS;
}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	//xs = 200;
	//ys = 200;
	u = std::clamp(u, (float)0, (float)1);
	v = std::clamp(v, (float)0, (float)1);

	//if (((int)floor(u * 6) % 2 == 0 && (int)floor(v * 6) % 2 == 0) || ((int)floor(u * 6) % 2 != 0 && (int)floor(v * 6) % 2 != 0)) {
	//	color[RED] = 0.24;
	//	color[GREEN] = 0.42f;
	//	color[BLUE] = 0.65f;
	//}

	//else {
	//	color[RED] = 0.213;
	//	color[GREEN] = 0.14f;
	//	color[BLUE] = 0.5;
	//}
	long r = u;
	long i = v;
	for (int i = 0; i < 100; i++) {
		r = r * r - v * v;
		i = 2 * r*i;
		if (r > 2 ^ 9 || i > 2 ^ 9) {
			break;
		}
	}

	long length = sqrt(r*r + i * i);
	int c = length % 10;

	if (c == 0) {
		color[RED] = 0.34;
		color[GREEN] = 0.52f;
		color[BLUE] = 0.75f;
	}
	else if (c == 1) {
		color[RED] = 0.14;
		color[GREEN] = 0.32f;
		color[BLUE] = 0.55f;
	}
	else if (c == 2) {
		color[RED] = 0.44;
		color[GREEN] = 0.62f;
		color[BLUE] = 0.75f;
	}
	else if (c == 3) {
		color[RED] = 0.24;
		color[GREEN] = 0.02f;
		color[BLUE] = 0.95f;
	}
	else if (c == 4) {
		color[RED] = 0.04;
		color[GREEN] = 0.12f;
		color[BLUE] = 0.75f;
	}
	else if (c == 5) {
		color[RED] = 0.74;
		color[GREEN] = 0.42f;
		color[BLUE] = 0.25f;
	}
	else if (c == 6) {
		color[RED] = 0.84;
		color[GREEN] = 0.12f;
		color[BLUE] = 0.85f;
	}
	else if (c == 7) {
		color[RED] = 0.94;
		color[GREEN] = 0.52f;
		color[BLUE] = 0.45f;
	}
	else if (c == 8) {
		color[RED] = 0.14;
		color[GREEN] = 0.72f;
		color[BLUE] = 0.55f;
	}
	else {
		color[RED] = 0.57;
		color[GREEN] = 0.51f;
		color[BLUE] = 0.4;
	}

	return GZ_SUCCESS;
}

/* Free texture memory */
int GzFreeTexture()
{
	if (image != NULL)
		free(image);
	return GZ_SUCCESS;
}