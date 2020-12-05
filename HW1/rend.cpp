#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"

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
	for (int i= 0; i < xres*yres; i++) {
		pixelbuffer[i] = { 1000,1000,1000,1,0 };
		framebuffer[3 * i] = (char)1000;
		framebuffer[3 * i + 1] = (char)1000;
		framebuffer[3 * i + 2] = (char)1000;
	}

	return GZ_SUCCESS;
}


int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	if(i>=0 && i<xres && j>=0 && j<yres)
	{
		int index = ARRAY(i, j);
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