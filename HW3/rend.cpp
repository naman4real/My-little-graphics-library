/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#include<algorithm>

#define PI (float) 3.14159265358979323846

int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/

	double radians = degree * PI / 180.0f;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1.0;
	mat[1][1] = (float)cos(radians);
	mat[1][2] = -1.0f * (float)sin(radians);
	mat[2][1] = (float)sin(radians);
	mat[2][2] = (float)cos(radians);
	mat[3][3] = 1.0;


	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	double radians = degree * PI / 180.0f;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[1][1] = 1.0;
	mat[0][0] = (float)cos(radians);
	mat[2][0] = -1.0f * (float)sin(radians);
	mat[0][2] = (float)sin(radians);
	mat[2][2] = (float)cos(radians);
	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	double radians = degree * PI / 180.0f;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	mat[2][2] = 1.0;
	mat[0][0] = (float)cos(radians);
	mat[0][1] = -1.0f * (float)sin(radians);
	mat[1][0] = (float)sin(radians);
	mat[1][1] = (float)cos(radians);
	mat[3][3] = 1.0;
	return GZ_SUCCESS;
}

int GzRender::GzTrxMat(GzCoord translate, GzMatrix mat)
{
	/* HW 3.4
	// Create translation matrix
	// Pass back the matrix using mat value
	*/
	for (int i = 0; i < 4; i++) { 
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
			if (j == 3)
				mat[i][j] = translate[i];
			if (i == j)
				mat[i][j] = 1;
			
		}
	}

	return GZ_SUCCESS;
}


int GzRender::GzScaleMat(GzCoord scale, GzMatrix mat)
{
	/* HW 3.5
	// Create scaling matrix
	// Pass back the matrix using mat value
	*/
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
			if (i == 3 && i == j) {
				mat[i][j] = 1.0f;
			}
			else if (i == j) {
				mat[i][j] = scale[i];
			}
		}
	}

	/*mat[0][0] = scale[0];
	mat[1][1] = scale[1];
	mat[2][2] = scale[2];
	mat[3][3] = 1.0;*/
	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */
	 //framebuffer = (char*) malloc (3 * sizeof(char) * xRes * yRes);
	xres = xRes;
	yres = yRes;
	//framebuffer = (char*)malloc(3 * sizeof(char) * xRes * yRes);
	//pixelbuffer = (GzPixel*)malloc(sizeof(GzPixel) * xRes * yRes);
	framebuffer = new char[3 * xres*yres];
	pixelbuffer = new GzPixel[xres*yres];

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/
	matlevel = -1;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Xsp[i][j] = 0;
		}
	}

	Xsp[0][0] = (float)xRes / 2;
	Xsp[0][3] = (float)xRes / 2;
	Xsp[1][1] = -1 * (float)yRes / 2;
	Xsp[1][3] = (float)yRes / 2;
	Xsp[2][2] = (float)MAXINT;
	Xsp[3][3] = 1.0;

	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;

	for (int i = 0; i < 3; i++) {
		if (i == 1)
			m_camera.worldup[i] = 1.0;
		else
			m_camera.worldup[i] = 0;
		m_camera.lookat[i] = 0;

	}

	m_camera.FOV = DEFAULT_FOV;


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
		pixelbuffer[i] = { 1000,1000,1000,1,MAXINT };
		framebuffer[3 * i] = (char)1000;
		framebuffer[3 * i + 1] = (char)1000;
		framebuffer[3 * i + 2] = (char)1000;
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

	//##################    calculate Xpi    ################## 
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xpi[i][j] = 0;
		}
	}
	m_camera.Xpi[0][0] = 1;
	m_camera.Xpi[1][1] = 1;
	m_camera.Xpi[2][2] = (float)tan((double)(m_camera.FOV * PI / 180.0) / 2.0);
	m_camera.Xpi[3][2] = (float)tan((double)(m_camera.FOV * PI / 180.0) / 2.0);
	m_camera.Xpi[3][3] = 1;


	//##################  calculate Xiw   ###########################

	GzCoord cameraX, cameraY, cameraZ, up_prime, CL;
	for (int i = 0; i < 3; i++) {
		CL[i] = m_camera.lookat[i] - m_camera.position[i];
	}

	float CL_magnitude = (float)sqrt((double)(CL[0] * CL[0] + CL[1] * CL[1] + CL[2] * CL[2]));

	//camera Z-axis
	cameraZ[0] = CL[0] / CL_magnitude;
	cameraZ[1] = CL[1] / CL_magnitude;
	cameraZ[2] = CL[2] / CL_magnitude;

	// dot product of up-vector and Z-vector
	float up_dotProduct_Z = m_camera.worldup[0] * cameraZ[0] + m_camera.worldup[1] * cameraZ[1] + m_camera.worldup[2] * cameraZ[2];


	// calculate up_prime-vector (normalized) which is the camera Y-axis
	up_prime[0] = m_camera.worldup[0] - up_dotProduct_Z * cameraZ[0];
	up_prime[1] = m_camera.worldup[1] - up_dotProduct_Z * cameraZ[1];
	up_prime[2] = m_camera.worldup[2] - up_dotProduct_Z * cameraZ[2];

	float UP_prime_magnitude = (float)sqrt((double)(up_prime[0] * up_prime[0] + up_prime[1] * up_prime[1] + up_prime[2] * up_prime[2]));
	//camera Y-axis
	cameraY[0] = up_prime[0] / UP_prime_magnitude;
	cameraY[1] = up_prime[1] / UP_prime_magnitude;
	cameraY[2] = up_prime[2] / UP_prime_magnitude;

	//camera X-axis. (the cross prodcut of Y and Z)
	cameraX[0] = cameraY[1] * cameraZ[2] - cameraY[2] * cameraZ[1];
	cameraX[1] = cameraY[2] * cameraZ[0] - cameraY[0] * cameraZ[2];
	cameraX[2] = cameraY[0] * cameraZ[1] - cameraY[1] * cameraZ[0];

	m_camera.Xiw[0][0] = cameraX[0];
	m_camera.Xiw[0][1] = cameraX[1];
	m_camera.Xiw[0][2] = cameraX[2];
	m_camera.Xiw[1][0] = cameraY[0];
	m_camera.Xiw[1][1] = cameraY[1];
	m_camera.Xiw[1][2] = cameraY[2];
	m_camera.Xiw[2][0] = cameraZ[0];
	m_camera.Xiw[2][1] = cameraZ[1];
	m_camera.Xiw[2][2] = cameraZ[2];
	m_camera.Xiw[0][3] = -1.0f * (cameraX[0] * m_camera.position[0] + cameraX[1] * m_camera.position[1] + cameraX[2] * m_camera.position[2]);
	m_camera.Xiw[1][3] = -1.0f * (cameraY[0] * m_camera.position[0] + cameraY[1] * m_camera.position[1] + cameraY[2] * m_camera.position[2]);
	m_camera.Xiw[2][3] = -1.0f * (cameraZ[0] * m_camera.position[0] + cameraZ[1] * m_camera.position[1] + cameraZ[2] * m_camera.position[2]);
	m_camera.Xiw[3][0] = 0;
	m_camera.Xiw[3][1] = 0;
	m_camera.Xiw[3][2] = 0;
	m_camera.Xiw[3][3] = 1;




	int status = 0;
	status |= GzPushMatrix(Xsp);

	status |= GzPushMatrix(m_camera.Xpi);
	status |= GzPushMatrix(m_camera.Xiw);

	if (status)
		return GZ_FAILURE;
	else
		return GZ_SUCCESS;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/
	m_camera.FOV = camera.FOV;
	for (int i = 0; i < 3; i++) {
		m_camera.position[i] = camera.position[i];
		m_camera.lookat[i] = camera.lookat[i];
		m_camera.worldup[i] = camera.worldup[i];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xpi[i][j] = camera.Xpi[i][j];
			m_camera.Xiw[i][j] = camera.Xiw[i][j];
		}
	}
	return GZ_SUCCESS;
}

int GzRender::GzPushMatrix(GzMatrix	matrix)
{
	/* HW 3.9
	- push a matrix onto the Ximage stack
	- check for stack overflow
	*/
	if (matlevel < MATLEVELS) {
		if (matlevel == -1) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					Ximage[0][i][j] = matrix[i][j];
				}
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					float sum = 0;
					for (int k = 0; k < 4; k++) {
						sum += Ximage[matlevel][i][k] * matrix[k][j];
					}
					Ximage[matlevel + 1][i][j] = sum;
				}
			}
		}
		matlevel++;
	}
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/

	if (matlevel > -1)
		matlevel--;
	else
		return GZ_FAILURE;

	return GZ_SUCCESS;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
	/* HW1.4 write pixel values into the buffer */
	if (i >= 0 && i < xres && j >= 0 && j < yres)
	{
		int index = ARRAY(i, j);
		if (z < pixelbuffer[index].z)
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

		tempRed = std::clamp(tempRed, (short)0, (short)4095);
		tempGreen = std::clamp(tempGreen, (short)0, (short)4095);
		tempBlue = std::clamp(tempBlue, (short)0, (short)4095);
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

		tempRed = std::clamp(tempRed, (short)0, (short)4095);
		tempGreen = std::clamp(tempGreen, (short)0, (short)4095);
		tempBlue= std::clamp(tempBlue, (short)0, (short)4095);
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
	return a * x1 + b * y1 + c;

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
	GzCoord* verticesList = (GzCoord*)valueList[0];


	float vertices4D[3][4];
	float transedVertices4D[3][4];
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			vertices4D[j][i] = verticesList[j][i];
		}
		vertices4D[j][3] = 1.0;
	}
	// Transformation M * Vertex Coord.
	for (int k = 0; k < 3; k++) {
		for (int j = 0; j < 4; j++) {
			transedVertices4D[k][j] = 0;
			for (int i = 0; i < 4; i++) {
				transedVertices4D[k][j] += Ximage[matlevel][j][i] * vertices4D[k][i];
			}
		}
	}
	// 4D => 3D
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			verticesList[j][i] = transedVertices4D[j][i] / transedVertices4D[j][3];
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

	float edge1[] = { v21 - v01, v00 - v20, v20 * v01 - v00 * v21 };
	float edge2[] = { v01 - v11, v10 - v00, v00 * v11 - v10 * v01 };
	float edge3[] = { v11 - v21, v20 - v10, v10 * v21 - v20 * v11 };

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

			float sat1 = satisfies(edge1[0], edge1[1], edge1[2], (float)i, (float)j);
			float sat2 = satisfies(edge2[0], edge2[1], edge2[2], (float)i, (float)j);
			float sat3 = satisfies(edge3[0], edge3[1], edge3[2], (float)i, (float)j);

			if ((sat1 > 0 && sat2 > 0 && sat3 > 0 && crossP[2] != 0) || (sat1 < 0 && sat2 < 0 && sat3 < 0 && crossP[2] != 0)
				|| sat1 == 0 || sat2 == 0 || sat3 == 0) {

				int z = (int)((-1 * D - A * (float)i - B * (float)j) / C + 0.5);
				GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, z);
			}
		}
	}
	return GZ_SUCCESS;
}

