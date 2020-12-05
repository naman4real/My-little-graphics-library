/* CS580 Homework 5 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#include<algorithm>

#define PI (float) 3.14159265358979323846

short normal_matLevel;


int pushMatrixToStack(short &matlevel, GzMatrix matrix, GzMatrix* stack) {

	if (matlevel >= MATLEVELS)
		return GZ_FAILURE;

	if (matlevel == -1) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (i == j)
					stack[0][i][j] = 1;
				else
					stack[0][i][j] = 0;
			}
		}
		matlevel++;
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			stack[matlevel + 1][i][j] = 0;
			for (int k = 0; k < 4; k++) {
				stack[matlevel + 1][i][j] += stack[matlevel][i][k] * matrix[k][j];
			}
		}
	}
	matlevel++;
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

float dotProduct(float vecA[], float vecB[]) {
	return vecA[0] * vecB[0] + vecA[1] * vecB[1] + vecA[2] * vecB[2];
}


int GzRender::GzRotXMat(float degree, GzMatrix mat)
{
	/* HW 3.1
	// Create rotate matrix : rotate along x axis
	// Pass back the matrix using mat value
	*/

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	double radians = degree * PI / 180;

	mat[0][0] = 1;
	mat[1][1] = cos(radians);
	mat[1][2] = -1 * sin(radians);
	mat[2][1] = sin(radians);
	mat[2][2] = cos(radians);
	mat[3][3] = 1;


	return GZ_SUCCESS;
}

int GzRender::GzRotYMat(float degree, GzMatrix mat)
{
	/* HW 3.2
	// Create rotate matrix : rotate along y axis
	// Pass back the matrix using mat value
	*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	double radians = degree * PI / 180;

	mat[1][1] = 1;
	mat[0][0] = cos(radians);
	mat[2][0] = -1 * sin(radians);
	mat[0][2] = sin(radians);
	mat[2][2] = cos(radians);
	mat[3][3] = 1;

	return GZ_SUCCESS;
}

int GzRender::GzRotZMat(float degree, GzMatrix mat)
{
	/* HW 3.3
	// Create rotate matrix : rotate along z axis
	// Pass back the matrix using mat value
	*/
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i][j] = 0;
		}
	}
	double radians = degree * PI / 180;

	mat[2][2] = 1;
	mat[0][0] = cos(radians);
	mat[0][1] = -1 * sin(radians);
	mat[1][0] = sin(radians);
	mat[1][1] = cos(radians);
	mat[3][3] = 1;
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
				mat[i][j] = 1;
			}
			else if (i == j) {
				mat[i][j] = scale[i];
			}
		}
	}
	return GZ_SUCCESS;
}


GzRender::GzRender(int xRes, int yRes)
{
	/* HW1.1 create a framebuffer for MS Windows display:
	 -- set display resolution
	 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
	 -- allocate memory for pixel buffer
	 */

	numlights = 0;
	matlevel = -1;
	normal_matLevel = -1;
	xres = (unsigned short)xRes;
	yres = (unsigned short)yRes;



	framebuffer = (char*)malloc(3 * sizeof(char) * xres * yres);
	pixelbuffer = (GzPixel*)malloc(sizeof(GzPixel) * xres * yres);

	/* HW 3.6
	- setup Xsp and anything only done once
	- init default camera
	*/

	// initiaize default camera
	
	m_camera.FOV = DEFAULT_FOV;
	m_camera.position[0] = DEFAULT_IM_X;
	m_camera.position[1] = DEFAULT_IM_Y;
	m_camera.position[2] = DEFAULT_IM_Z;
	for (int i = 0; i < 3; i++) {
		m_camera.lookat[i] = m_camera.worldup[i] = 0;
	}
	m_camera.worldup[1] = 1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				Ximage[0][i][j] = 1;
			else
				Ximage[0][i][j] = 0;

		}
	}

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			Xsp[i][j] = 0;
		}
	}
	Xsp[0][0] = xres / 2;
	Xsp[0][3] = xres / 2;
	Xsp[1][1] = -1 * yres / 2;
	Xsp[1][3] = yres / 2;
	Xsp[2][2] = MAXINT;
	Xsp[3][3] = 1;




}

GzRender::~GzRender()
{
	/* HW1.2 clean up, free buffer memory */
	delete framebuffer;
	delete pixelbuffer;

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
	double fov = m_camera.FOV * PI / 360;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m_camera.Xpi[i][j] = 0;
		}
	}
	m_camera.Xpi[0][0] = 1;
	m_camera.Xpi[1][1] = 1;
	m_camera.Xpi[2][2] = tan(fov);
	m_camera.Xpi[3][2] = tan(fov);
	m_camera.Xpi[3][3] = 1;


	//##################  calculate Xiw   ###########################

	GzCoord cameraX, cameraY, cameraZ, up_prime, CL;
	for (int i = 0; i < 3; i++) {
		CL[i] = m_camera.lookat[i] - m_camera.position[i];
	}

	float CL_magnitude = sqrt(CL[0] * CL[0] + CL[1] * CL[1] + CL[2] * CL[2]);

	//camera Z-axis
	cameraZ[0] = CL[0] / CL_magnitude;
	cameraZ[1] = CL[1] / CL_magnitude;
	cameraZ[2] = CL[2] / CL_magnitude;

	// dot product of up-vector and Z-vector
	float up_dotProduct_Z = dotProduct(m_camera.worldup, cameraZ);


	// calculate up_prime-vector (normalized) which is the camera Y-axis
	for (int i = 0; i < 3; i++) {
		up_prime[i] = m_camera.worldup[i] - up_dotProduct_Z * cameraZ[i];
	}

	float UP_prime_magnitude = sqrt(up_prime[0] * up_prime[0] + up_prime[1] * up_prime[1] + up_prime[2] * up_prime[2]);
	//camera Y-axis
	for (int i = 0; i < 3; i++) {
		cameraY[i] = up_prime[i] / UP_prime_magnitude;
	}


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
	m_camera.Xiw[0][3] = -1 * dotProduct(cameraX, m_camera.position);
	m_camera.Xiw[1][3] = -1 * dotProduct(cameraY, m_camera.position);
	m_camera.Xiw[2][3] = -1 * dotProduct(cameraZ, m_camera.position);
	m_camera.Xiw[3][0] = 0;
	m_camera.Xiw[3][1] = 0;
	m_camera.Xiw[3][2] = 0;
	m_camera.Xiw[3][3] = 1;



	if (!GzPushMatrix(Xsp) && !GzPushMatrix(m_camera.Xpi) && !GzPushMatrix(m_camera.Xiw))
		return GZ_SUCCESS;
	return GZ_FAILURE;
}

int GzRender::GzPutCamera(GzCamera camera)
{
	/* HW 3.8
	/*- overwrite renderer camera structure with new camera definition
	*/
	m_camera.FOV = camera.FOV;
	for (int i = 0; i < 3; i++) {
		m_camera.worldup[i] = camera.worldup[i];
		m_camera.position[i] = camera.position[i];
		m_camera.lookat[i] = camera.lookat[i];
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
	int a = pushMatrixToStack(matlevel, matrix, Ximage);

	GzMatrix identityMatrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)
				identityMatrix[i][j] = 1;
			else
				identityMatrix[i][j] = 0;
		}
	}


	if (normal_matLevel < 2) {
		a |= pushMatrixToStack(normal_matLevel, identityMatrix, Xnorm);
	}

	else if (normal_matLevel >= 2) {
		// 0 out the translations
		matrix[0][3] = 0;
		matrix[1][3] = 0;
		matrix[2][3] = 0;
		// make the matrix unitary rotation by coputing scale factor k and dividing every element by k.
		float scaleFactor = sqrt(matrix[0][0] * matrix[0][0] + matrix[0][1] * matrix[0][1] + matrix[0][2] * matrix[0][2]);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				matrix[i][j] /= scaleFactor;
			}
		}
		a |= pushMatrixToStack(normal_matLevel, matrix, Xnorm);
	}


	if (a)
		return GZ_FAILURE;
	return GZ_SUCCESS;
}

int GzRender::GzPopMatrix()
{
	/* HW 3.10
	- pop a matrix off the Ximage stack
	- check for stack underflow
	*/

	if (matlevel < 0)
		return GZ_FAILURE;
	matlevel--;
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
		int position = ARRAY(i, j);
		*r = pixelbuffer[position].red;
		*g = pixelbuffer[position].green;
		*b = pixelbuffer[position].blue;
		*a = pixelbuffer[position].alpha;
		*z = pixelbuffer[position].z;
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
		tempBlue = std::clamp(tempBlue, (short)0, (short)4095);
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
	for (int position = 0; position < numAttributes; position++) {
		GzToken currentToken = nameList[position];
		if (currentToken == GZ_RGB_COLOR) {
			float* color = (float*)valueList[position];
			flatcolor[0] = color[0];
			flatcolor[1] = color[1];
			flatcolor[2] = color[2];
		}
		else if (currentToken == GZ_SPECULAR_COEFFICIENT) {
			float* specular = (float*)valueList[position];
			Ks[0] = specular[0];
			Ks[1] = specular[1];
			Ks[2] = specular[2];
		}
		else if (currentToken == GZ_DIFFUSE_COEFFICIENT) {
			float* diffuse = (float*)valueList[position];
			Kd[0] = diffuse[0];
			Kd[1] = diffuse[1];
			Kd[2] = diffuse[2];
		}
		else if (currentToken == GZ_AMBIENT_COEFFICIENT) {
			float* ambient = (float*)valueList[position];
			Ka[0] = ambient[0];
			Ka[1] = ambient[1];
			Ka[2] = ambient[2];
		}
		else if (currentToken == GZ_DISTRIBUTION_COEFFICIENT) {
			spec = *(float*)valueList[position];
		}
		else if (currentToken == GZ_DIRECTIONAL_LIGHT) {
			GzLight* dirLight = (GzLight*)valueList[position];
			for (int i = 0; i < 3; i++) {
				lights[position].direction[i] = dirLight->direction[i];
				lights[position].color[i] = dirLight->color[i];
			}
			numlights++;

		}
		else if (currentToken == GZ_AMBIENT_LIGHT) {
			GzLight* ambLight = (GzLight*)valueList[position];
			for (int i = 0; i < 3; i++) {
				ambientlight.direction[i] = ambLight->direction[i];
				ambientlight.color[i] = ambLight->color[i];
			}

		}
		else if (currentToken == GZ_INTERPOLATE) {
			interp_mode = *(int*)valueList[position];
		}
		else if (currentToken == GZ_TEXTURE_MAP) {
			if ((GzTexture)valueList[position]) {
				tex_fun = (GzTexture)valueList[position];
			}

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


	float modelVertices4D[3][4], modelNormals4D[3][4];
	GzCoord* verticesList = (GzCoord*)valueList[0];
	GzCoord* normalsList = (GzCoord*)valueList[1];
	GzTextureIndex* uvList = (GzTextureIndex*)valueList[2];

	for (int k = 0; k < 3; k++) {
		for (int j = 0; j < 4; j++) {
			modelVertices4D[k][j] = modelNormals4D[k][j] = 0;
			for (int i = 0; i < 4; i++) {
				if (i <= 2) {
					modelVertices4D[k][j] += Ximage[matlevel][j][i] * verticesList[k][i];
					modelNormals4D[k][j] += Xnorm[normal_matLevel][j][i] * normalsList[k][i];
				}
				else {
					modelVertices4D[k][j] += Ximage[matlevel][j][i];
					modelNormals4D[k][j] += Xnorm[normal_matLevel][j][i];
				}

			}
		}

	}


	for (int i = 0; i < 3; i++) {
		float i_V = modelVertices4D[i][3];
		float i_N = modelNormals4D[i][3];
		for (int j = 0; j < 3; j++) {
			verticesList[i][j] = modelVertices4D[i][j] / i_V;
			normalsList[i][j] = modelNormals4D[i][j] / i_N;
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



	GzColor specularIntensity[3], diffuseIntensity[3], finalIntensity[3];
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			specularIntensity[j][i] = diffuseIntensity[j][i] = finalIntensity[j][i] = 0;
		}
	}
	GzColor ambientINtensity[3];

	if (interp_mode == GZ_COLOR) {
		if (tex_fun) {
			Ka[0] = Ka[1] = Ka[2] = 1;
			Kd[0] = Kd[1] = Kd[2] = 1;
			Ks[0] = Ks[1] = Ks[2] = 1;
		}
	}
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			ambientINtensity[j][k] = Ka[k] * ambientlight.color[k];
		}
		for (int n = 0; n < numlights; n++) {
			GzCoord E = { 0,0,-1 };
			GzCoord R;

			float N_DotP_L = dotProduct(normalsList[j], lights[n].direction);
			float N_DotP_E = dotProduct(normalsList[j], E);


			if ((N_DotP_E > 0 && N_DotP_L > 0) || (N_DotP_E < 0 && N_DotP_L < 0)) {
				for (int r = 0; r < 3; r++) {
					R[r] = 2 * N_DotP_L * normalsList[j][r] - lights[n].direction[r];
				}
				float R_magnitude = sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);
				for (int r = 0; r < 3; r++) {
					R[r] /= R_magnitude;
				}
				float R_DotP_E = dotProduct(R, E);
				R_DotP_E = std::clamp(R_DotP_E, (float)0, (float)1);
				

					for (int r = 0; r < 3; r++) {
						specularIntensity[j][r] += lights[n].color[r] * Ks[r] * pow(R_DotP_E, spec);
						float value = dotProduct(normalsList[j], lights[n].direction);
						if (N_DotP_L > 0 && N_DotP_E > 0) {
							diffuseIntensity[j][r] += lights[n].color[r] * Kd[r] * value;
						}
						else {
							diffuseIntensity[j][r] += lights[n].color[r] * Kd[r] * -1 * value;
						}
					}
			}
		}

	}
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			finalIntensity[j][i] = specularIntensity[j][i] + diffuseIntensity[j][i] + ambientINtensity[j][i];
			finalIntensity[j][i] = std::clamp(finalIntensity[j][i], (float)0, (float)1);
		}
	}

	// Interpolate Red, Green and Blue colors from using finalIntensity:
	float vectorRed1[] = { v10 - v00,v11 - v01,finalIntensity[1][0] - finalIntensity[0][0] };
	float vectorRed2[] = { v20 - v00,v21 - v01,finalIntensity[2][0] - finalIntensity[0][0] };
	float* crossPRed = crossProduct(vectorRed1, vectorRed2);
	float A_red = crossPRed[0];
	float B_red = crossPRed[1];
	float C_red = crossPRed[2];
	float D_red = -1 * (A_red * v00 + B_red * v01 + C_red * finalIntensity[0][0]);

	float vectorGreen1[] = { v10 - v00,v11 - v01,finalIntensity[1][1] - finalIntensity[0][1] };
	float vectorGreen2[] = { v20 - v00,v21 - v01,finalIntensity[2][1] - finalIntensity[0][1] };
	float* crossPGreen = crossProduct(vectorGreen1, vectorGreen2);
	float A_green = crossPGreen[0];
	float B_green = crossPGreen[1];
	float C_green = crossPGreen[2];
	float D_green = -1 * (A_green * v00 + B_green * v01 + C_green * finalIntensity[0][1]);

	float vectorBlue1[] = { v10 - v00,v11 - v01,finalIntensity[1][2] - finalIntensity[0][2] };
	float vectorBlue2[] = { v20 - v00,v21 - v01,finalIntensity[2][2] - finalIntensity[0][2] };
	float* crossPBlue = crossProduct(vectorBlue1, vectorBlue2);
	float A_blue = crossPBlue[0];
	float B_blue = crossPBlue[1];
	float C_blue = crossPBlue[2];
	float D_blue = -1 * (A_blue * v00 + B_blue * v01 + C_blue * finalIntensity[0][2]);

	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Interpolate normals X,Y and Z:
	float vectorNormalX1[] = { v10 - v00,v11 - v01,normalsList[1][0] - normalsList[0][0] };
	float vectorNormalX2[] = { v20 - v00,v21 - v01,normalsList[2][0] - normalsList[0][0] };
	float* crossPNormalX = crossProduct(vectorNormalX1, vectorNormalX2);
	float Ax = crossPNormalX[0];
	float Bx = crossPNormalX[1];
	float Cx = crossPNormalX[2];
	float Dx = -1 * (Ax * v00 + Bx * v01 + Cx * normalsList[0][0]);

	float vectorNormalY1[] = { v10 - v00,v11 - v01,normalsList[1][1] - normalsList[0][1] };
	float vectorNormalY2[] = { v20 - v00,v21 - v01,normalsList[2][1] - normalsList[0][1] };
	float* crossPNormalY = crossProduct(vectorNormalY1, vectorNormalY2);
	float Ay = crossPNormalY[0];
	float By = crossPNormalY[1];
	float Cy = crossPNormalY[2];
	float Dy = -1 * (Ay * v00 + By * v01 + Cy * normalsList[0][1]);

	float vectorNormalZ1[] = { v10 - v00,v11 - v01,normalsList[1][2] - normalsList[0][2] };
	float vectorNormalZ2[] = { v20 - v00,v21 - v01,normalsList[2][2] - normalsList[0][2] };
	float* crossPNormalZ = crossProduct(vectorNormalZ1, vectorNormalZ2);
	float Az = crossPNormalZ[0];
	float Bz = crossPNormalZ[1];
	float Cz = crossPNormalZ[2];
	float Dz = -1 * (Az * v00 + Bz * v01 + Cz * normalsList[0][2]);

	// Interpolate U and V

	//First apply perspective correction

	for (int z = 0; z < 3; z++) {
		float Vz_prime = verticesList[z][2] / (MAXINT - verticesList[z][2]);
		uvList[z][0]/=(Vz_prime + 1);
		uvList[z][1]/= (Vz_prime + 1);
	}
	//U
	float vector_U_1[] = { v10 - v00,v11 - v01,uvList[1][0] - uvList[0][0] };
	float vector_U_2[] = { v20 - v00,v21 - v01,uvList[2][0] - uvList[0][0] };
	float* crossP_U = crossProduct(vector_U_1, vector_U_2);
	float Au = crossP_U[0];
	float Bu = crossP_U[1];
	float Cu = crossP_U[2];
	float Du = -1 * (Au * v00 + Bu * v01 + Cu * uvList[0][0]);

	//V
	float vector_V_1[] = { v10 - v00,v11 - v01,uvList[1][1] - uvList[0][1] };
	float vector_V_2[] = { v20 - v00,v21 - v01,uvList[2][1] - uvList[0][1] };
	float* crossP_V = crossProduct(vector_V_1, vector_V_2);
	float Av = crossP_V[0];
	float Bv = crossP_V[1];
	float Cv = crossP_V[2];
	float Dv = -1 * (Av * v00 + Bv * v01 + Cv * uvList[0][1]);



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

	// calculate bounding-box:
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
				if (z >= 0) {


					// calculate texture color
					GzColor color;
					float uInterpolated = -1 * (Au * i + Bu * j + Du) / Cu;
					float vInterpolated = -1 * (Av * i + Bv * j + Dv) / Cv;
					float zPrimeInterpolated = (float)z / (MAXINT - z);
					uInterpolated *= (zPrimeInterpolated + 1);
					vInterpolated *= (zPrimeInterpolated + 1);
					tex_fun(uInterpolated, vInterpolated, color);

					if (interp_mode == GZ_FLAT) {
						GzPut(i, j, ctoi(flatcolor[0]), ctoi(flatcolor[1]), ctoi(flatcolor[2]), 1, z);
					}

					else if (interp_mode == GZ_COLOR) {
						GzColor finalIntensity;
						finalIntensity[0] = -1 * (A_red * (float)i + B_red * (float)j + D_red) / C_red;
						finalIntensity[1] = -1 * (A_green * (float)i + B_green * (float)j + D_green) / C_green;
						finalIntensity[2] = -1 * (A_blue * (float)i + B_blue * (float)j + D_blue) / C_blue;
						GzPut(i, j, ctoi(finalIntensity[0] * color[0]), ctoi(finalIntensity[1] * color[1]), ctoi(finalIntensity[2] * color[2]), 1, z);

					}
					else if (interp_mode == GZ_NORMALS) {


						GzCoord interpNormal = { (Dx + Ax * (float)i + Bx * (float)j) / -Cx,
												  (Dy + Ay * (float)i + By * (float)j) / -Cy,
												  (Dz + Az * (float)i + Bz * (float)j) / -Cz };

						for (int i = 0; i < 3; i++) {
							Kd[i] = Ka[i] = color[i];
						}

						GzColor finalIntensity, specularIntensity, diffuseIntensity, ambientIntensity;
						for (int m = 0; m < 3; m++) {
							ambientIntensity[m] = Ka[m] * ambientlight.color[m];
							specularIntensity[m] = diffuseIntensity[m] = 0;
						}

						for (int n = 0; n < numlights; n++) {
							GzCoord E = { 0,0,-1 };
							GzCoord R;
							float N_DotP_L = dotProduct(interpNormal, lights[n].direction);
							float N_DotP_E = dotProduct(interpNormal, E);

							if ((N_DotP_E > 0 && N_DotP_L > 0) || (N_DotP_E < 0 && N_DotP_L < 0)) {
								//calculate reflection vector
								for (int r = 0; r < 3; r++) {
									R[r] = 2 * N_DotP_L * interpNormal[r] - lights[n].direction[r];
								}
								//normalize R
								float R_magnitude = sqrt(R[0] * R[0] + R[1] * R[1] + R[2] * R[2]);
								for (int k = 0; k < 3; k++) {
									R[k] /= R_magnitude;
								}
								//clamp R between [0,1]
								float R_DotP_E = R[0] * E[0] + R[1] * E[1] + R[2] * E[2];
								R_DotP_E = std::clamp(R_DotP_E, (float)0, (float)1);

								//calculate specular and diffuse intensities
								for (int r = 0; r < 3; r++) {
									specularIntensity[r] += lights[n].color[r] * Ks[r] * pow(R_DotP_E, spec);
									float value = dotProduct(interpNormal, lights[n].direction);
									if (N_DotP_L > 0 && N_DotP_E > 0) {
										diffuseIntensity[r] += lights[n].color[r] * Kd[r] * value;
									}
									else {
										diffuseIntensity[r] += lights[n].color[r] * Kd[r] * -1 * value;
									}

								}
							}
						}

						for (int m = 0; m < 3; m++) {
							finalIntensity[m] = specularIntensity[m] + diffuseIntensity[m] + ambientIntensity[m];
							finalIntensity[m] = std::clamp(finalIntensity[m], (float)0, (float)1);
						}

						GzPut(i, j, ctoi(finalIntensity[0]), ctoi(finalIntensity[1]), ctoi(finalIntensity[2]), 1, z);
					}
				}
			}
		}
	}
	return GZ_SUCCESS;
}

