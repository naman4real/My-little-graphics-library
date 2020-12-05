#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	xres = xRes;
	yres = yRes;
	framebuffer = new char[3 * xres*yres];
	pixelbuffer = new GzPixel[xres*yres];
}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete[] framebuffer;
	delete[] pixelbuffer;

}


int GzRender::GzDefault()
{
	/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (int i = 0; i < xres*yres; i++) {
		pixelbuffer[i] = { 1000,1000,1000,1,MAXINT};
		framebuffer[3 * i] = (char)1000;
		framebuffer[3 * i + 1] = (char)1000;
		framebuffer[3 * i + 2] = (char)1000;
	}
	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres)
	{
		int index = ARRAY(i, j);
		if(z<pixelbuffer[index].z)
			pixelbuffer[index] = { r,g,b,a,z };
	}

	return GZ_SUCCESS;
}


int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* HW1.5 retrieve a pixel information from the pixel buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres)
	{
		int index = ARRAY(i, j);
		GzPixel pixel = pixelbuffer[index];
		*r = pixel.red;
		*g = pixel.green;
		*b = pixel.blue;
		*a = pixel.alpha;
		*z = pixel.z;
	}


	return GZ_SUCCESS;
}


int GzRender::GzFlushDisplay2File(FILE* outfile)
{
	/* HW1.6 write image to ppm file -- "P6 %d %d 255\r" */
	fprintf(outfile, "P6 %d %d 255\n", xres, yres);
	for (int i = 0; i < xres*yres; i++) {
		GzPixel pixel = pixelbuffer[i];
		GzIntensity tempRed = pixel.red;
		GzIntensity tempGreen = pixel.green;
		GzIntensity tempBlue = pixel.blue;

		if (tempRed < 0)
			tempRed = 0;
		if (tempRed > 4095)
			tempRed = 4095;
		if (tempGreen < 0)
			tempGreen = 0;
		if (tempGreen > 4095)
			tempGreen = 4095;
		if (tempBlue < 0)
			tempBlue = 0;
		if (tempBlue > 4095)
			tempBlue = 4095;

		tempRed = tempRed >> 4;
		tempGreen = tempGreen >> 4;
		tempBlue = tempBlue >> 4;

		char color[3] = { (char)(tempRed & 0b11111111),(char)(tempGreen & 0b11111111),(char)(tempBlue & 0b11111111) };
		fwrite(color, 1, 3, outfile);
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
	for (int i = 0; i < xres*yres; i++) {
		GzPixel pixel = pixelbuffer[i];
		GzIntensity tempRed = pixel.red;
		GzIntensity tempGreen = pixel.green;
		GzIntensity tempBlue = pixel.blue;

		if (tempRed < 0)
			tempRed = 0;
		if (tempRed > 4095)
			tempRed = 4095;
		if (tempGreen < 0)
			tempGreen = 0;
		if (tempGreen > 4095)
			tempGreen = 4095;
		if (tempBlue < 0)
			tempBlue = 0;
		if (tempBlue > 4095)
			tempBlue = 4095;

		tempRed = tempRed >> 4;
		tempGreen = tempGreen >> 4;
		tempBlue = tempBlue >> 4;


		framebuffer[3 * i] = (char)(tempBlue & 0b11111111);
		framebuffer[3 * i + 1] = (char)(tempGreen & 0b11111111);
		framebuffer[3 * i + 2] = (char)(tempRed & 0b11111111);

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
	if (numAttributes == 1) {
		float* color = (float*)valueList[0];
		flatcolor[0] = color[0];
		flatcolor[1] = color[1];
		flatcolor[2] = color[2];
	}
	return GZ_SUCCESS;
}


float* crossProduct(float vecA[], float vecB[]) {
	float crossP[3];

	crossP[0] = vecA[1] * vecB[2] - vecA[2] * vecB[1];
	crossP[1] = vecA[2] * vecB[0] - vecA[0] * vecB[2];
	crossP[2] = vecA[0] * vecB[1] - vecA[1] * vecB[0];
	return crossP;

}

float satisfies(float a, float b, float c, float x1, float y1) {
	return a * x1 + b * y1 + c ;

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
	GzCoord* verticesList = (GzCoord*)valueList[0];



	if (verticesList[0][1] > verticesList[1][1]) {
		for (int i = 0; i < 3; i++) {
			std::swap(verticesList[0][i], verticesList[1][i]);
		}
	}
	if (verticesList[0][1] > verticesList[2][1]) {
		for (int i = 0; i < 3; i++) {
			std::swap(verticesList[0][i], verticesList[2][i]);
		}
	}
	if (verticesList[1][1] > verticesList[2][1]) {
		for (int i = 0; i < 3; i++) {
			std::swap(verticesList[1][i], verticesList[2][i]);
		}
	}


	if ((int)(verticesList[0][1] + 0.5) == (int)(verticesList[1][1] + 0.5)) {
		if (verticesList[0][0] > verticesList[1][0]) {
			for (int i = 0; i < 3; i++) {
				std::swap(verticesList[1][i], verticesList[2][i]);
			}
		}
	}
	else if ((int)(verticesList[1][1] + 0.5) == (int)(verticesList[2][1] + 0.5)) {
		if (verticesList[2][0] > verticesList[1][0]) {
			for (int i = 0; i < 3; i++) {
				std::swap(verticesList[1][i], verticesList[2][i]);
			}
		}
	}
	else {
		float  midX, longEdgeSlope;
		if ((int)(verticesList[0][0] + 0.5) == (int)(verticesList[2][0] + 0.5)) {
			midX = verticesList[0][0];
		}
		else {
			longEdgeSlope = (verticesList[0][1] - verticesList[2][1]) / (verticesList[0][0] - verticesList[2][0]);
			midX = (verticesList[1][1] - verticesList[0][1]) / longEdgeSlope + verticesList[0][0];
		}

		if (midX > verticesList[1][0]) {
			for (int i = 0; i < 3; i++) {
				std::swap(verticesList[1][i], verticesList[2][i]);
			}
		}
	}


	float v00 = verticesList[0][0];
	float v01 = verticesList[0][1];
	float v02 = verticesList[0][2];
	float v10 = verticesList[1][0];
	float v11 = verticesList[1][1];
	float v12 = verticesList[1][2];
	float v20 = verticesList[2][0];
	float v21 = verticesList[2][1];
	float v22 = verticesList[2][2];

	float edge1[] =   { v21 - v01, v00 - v20, v20 * v01 - v00 * v21};
	float edge2[] = { v01 - v11, v10 - v00, v00 * v11 - v10 * v01};
	float edge3[] = { v11 - v21, v20 - v10, v10 * v21 - v20 * v11};

	//calculate edge vectors
	float vectorA[] = { v10 - v00, v11 - v01, v12 - v02 };
	float vectorB[] = { v20 - v00, v21 - v01, v22 - v02 };

	//calculate 'D' for the plane equation
	float* crossP = crossProduct(vectorA, vectorB);
	float A = crossP[0];
	float B = crossP[1];
	float C = crossP[2];
	float D = -1 * (A * v00 + B * v01 + C * v02);

	//Get Bounding box
	int minx = (int)(min(min(verticesList[0][0], verticesList[1][0]), verticesList[2][0]) + 0.5);
	int maxx = (int)(max(max(verticesList[0][0], verticesList[1][0]), verticesList[2][0]) + 0.5);
	int miny = (int)(min(min(verticesList[0][1], verticesList[1][1]), verticesList[2][1]) + 0.5);
	int maxy = (int)(max(max(verticesList[0][1], verticesList[1][1]), verticesList[2][1]) + 0.5);


	for (int i = minx; i <= maxx; i++) {
		for (int j = miny; j <= maxy; j++) {

			float sat1 = satisfies(edge1[0], edge1[1], edge1[2], (float)i, (float)j) ;
			float sat2 = satisfies(edge2[0], edge2[1], edge2[2], (float)i, (float)j);
			float sat3 = satisfies(edge3[0], edge3[1], edge3[2], (float)i, (float)j);

			if ((sat1 > 0 && sat2 > 0 && sat3 > 0 && crossP[2]!=0) || (sat1 < 0 && sat2 < 0 && sat3 < 0 && crossP[2] != 0)
				|| sat1==0 || sat2==0 || sat3==0) {

				int z =(int) ((-1 * D - A * (float)i - B * (float)j) / C + 0.5);
				GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, z);
			}			
		}
	}





	return GZ_SUCCESS;
}

