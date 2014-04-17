#include "GLCamera.h"

// Initialize static members
unsigned int	GLCamera::cameraCount = 0;
unsigned int	GLCamera::nextID = 1;

GLCamera::GLCamera()
{
	// ID initialization
	++cameraCount;
	cameraID = nextID;
	++nextID;

	// All matrices begin as the identity matrix
	MVPMatrix = IDENTITY_MATRIX;
	ModelMatrix = IDENTITY_MATRIX;
	ViewMatrix = IDENTITY_MATRIX;
	ProjectionMatrix = IDENTITY_MATRIX;

	// Set pan locations to zero and zoom levels on the order of lat/long granularity
	panX = 0.0;
	panY = 0.0;
	zoomLevel = 1.0;
	zoomScale = 1.1;

	// Initial window values
	window[0] = -1.0;
	window[1] = 1.0;
	window[2] = -1.0;
	window[3] = 1.0;
	window[4] = 100000000.0;
	window[5] = -100000000.0;

	// Get the size of the viewport from the GL context
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportX = viewport[2];
	viewportY = viewport[3];

	// Calculate ratio of window width to viewport width
	pixelToViewRatio = ((window[1]-window[0]/2.0)/viewportX);
}


GLCamera::~GLCamera()
{
	--cameraCount;
}


/**
 * @brief Returns the unique ID associated with the Camera object
 * @return The unique ID associated with the Camera object
 */
unsigned int GLCamera::GetID()
{
	return cameraID;
}


/**
 * @brief Returns the total number of cameras the currently exist
 * @return The total number of cameras that currently exist
 */
unsigned int GLCamera::GetNumCameras()
{
	return cameraCount;
}


/**
 * @brief Pan the camera in the x-y plane
 * @param dx Change in x (pixels)
 * @param dy Change in y (pixels)
 */
void GLCamera::Pan(float dx, float dy)
{
	panX += pixelToViewRatio*dx/zoomLevel;
	panY -= pixelToViewRatio*dy/zoomLevel;
	UpdateModel();
}


/**
 * @brief Zoom the view by moving the camera up or down along the z-axis
 *
 * The magnitude of the value zoomAmount has no effect on how much the camera will zoom.
 * The sign of the value determines which way to zoom, and the zoom level is set by
 * multiplying the current zoom level by the zoomScale value.
 *
 * @param zoomAmount A positive value for zooming in, a negative value for zooming out
 */
void GLCamera::Zoom(float zoomAmount)
{
	if (zoomAmount > 0)
	{
		zoomLevel *= zoomScale;
	} else {
		zoomLevel /= zoomScale;
	}
	UpdateProjection();
}


/**
 * @brief Call this function when the window size changes in order to update the MVP matrices
 * @param l Left side of the viewport
 * @param r Right side of the viewport
 * @param b Bottom of the viewport
 * @param t Top of the viewport
 * @param n Near z-value
 * @param f Far z-value
 */
void GLCamera::SetWindowSize(float l, float r, float b, float t, float n, float f)
{
	window[0] = l;
	window[1] = r;
	window[2] = b;
	window[3] = t;
	window[4] = n;
	window[5] = f;

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	viewportX = viewport[2];
	viewportY = viewport[3];

	pixelToViewRatio = (window[1]-window[0])/viewportX;

	UpdateProjection();
}


/**
 * @brief Resets all pan/zoom levels to the default values
 */
void GLCamera::ResetView()
{
	panX = 0.0;
	panY = 0.0;
	zoomLevel = 1.0/EARTH_RADIUS;

	UpdateModel();
	UpdateProjection();
}


/**
 * @brief Gets the x- and y-coordinates in model space given a point on the window
 * @param x The x-coordinate on the window
 * @param y The y-coordinate on the window
 * @param resultX Pointer to value that will store the x result
 * @param resultY Pointer to value that will store the y result
 */
void GLCamera::GetUnprojectedPoint(float x, float y, float *resultX, float *resultY)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	y = (float)viewport[3] - y;

	GLdouble posX = 0.0;
	GLdouble posY = 0.0;
	GLdouble posZ = 0.0;
	gluUnProject((GLdouble)x, (GLdouble)y, (GLdouble)0.0, modelview, projection, viewport, &posX, &posY, &posZ);

//	float aspectRatio = viewportX/viewportY;

	*resultX = posX/zoomLevel - panX;
	*resultY = posY/zoomLevel - panY;
}


/**
 * @brief Gets the x- and y-coordinates on the window given a point in model space
 * @param x The x-coordinate in model space
 * @param y The y-coordinate in model space
 * @param resultX Pointer to the value that will store the x result
 * @param resultY Pointer to the value that will store the y result
 */
void GLCamera::GetProjectedPoint(float x, float y, float *resultX, float *resultY)
{
//	float xtemp = (x + panX) * zoomLevel;
//	float ytemp = (y + panY) * zoomLevel;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble modelview[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLdouble posX = 0.0;
	GLdouble posY = 0.0;
	GLdouble posZ = 0.0;

	if (gluProject((GLdouble)x, (GLdouble)y, (GLdouble)0.0, modelview, projection, viewport, &posX, &posY, &posZ) == GLU_FALSE)
		DEBUG("Nope");

//	posY = posY - (float)viewport[3];

	*resultX = (float)posX;
	*resultY = (float)posY;
}


float GLCamera::GetViewportWidth()
{
	return viewportX;
}


float GLCamera::GetViewportHeight()
{
	return viewportY;
}


CameraSettings GLCamera::GetCameraSettings()
{
	return CameraSettings(panX, panY, zoomLevel);
}


void GLCamera::SetCameraSettings(CameraSettings newSettings)
{
	ResetView();
	panX = newSettings.panX;
	panY = newSettings.panY;
	UpdateModel();
	zoomLevel = newSettings.zoomLevel;
	UpdateProjection();
}


/**
 * @brief Updates the Model Matrix, which is responsible for panning
 */
void GLCamera::UpdateModel()
{
	ModelMatrix = IDENTITY_MATRIX;

	TranslateMatrix(&ModelMatrix, panX, panY, 0);

	UpdateMVP();
}


/**
 * @brief Updates the Projection Matrix, which is responsible for zooming
 */
void GLCamera::UpdateProjection()
{
	ProjectionMatrix = IDENTITY_MATRIX;

	ProjectionMatrix.m[0] = 2.0/(window[1]-window[0]);
	ProjectionMatrix.m[3] = -(window[1]+window[0])/(window[1]-window[0]);
	ProjectionMatrix.m[5] = 2.0/(window[3]-window[2]);
	ProjectionMatrix.m[7] = -(window[3]+window[2])/(window[3]-window[2]);
	ProjectionMatrix.m[10] = -2.0/(window[5]-window[4]);
	ProjectionMatrix.m[11] = -(window[5]+window[4])/(window[5]-window[4]);

	ScaleMatrix(&ProjectionMatrix, zoomLevel, zoomLevel, zoomLevel);

	UpdateMVP();
}


/**
 * @brief Performs multiplication of Model, View, and Projection matrices in order to
 * create the MVP Matrix
 */
void GLCamera::UpdateMVP()
{
	Matrix VP = MultiplyMatrices(&ViewMatrix, &ProjectionMatrix);
	memcpy(&MVPMatrix.m, MultiplyMatrices(&ModelMatrix, &VP).m, sizeof(MVPMatrix.m));
}


/**
 * @brief Multiplies to matrices and returns the result
 * @param m1 Pointer to the first matrix
 * @param m2 Pointer to the second matrix
 * @return The resulting matrix
 */
Matrix GLCamera::MultiplyMatrices(const Matrix *m1, const Matrix *m2)
{
	Matrix out = IDENTITY_MATRIX;
	unsigned int row, column, row_offset;

	for (row=0, row_offset=row*4; row<4; ++row, row_offset=row*4)
	{
		for (column=0; column<4; ++column)
		{
			out.m[row_offset + column] =
					(m1->m[row_offset + 0] * m2->m[column+0]) +
					(m1->m[row_offset + 1] * m2->m[column+4]) +
					(m1->m[row_offset + 2] * m2->m[column+8]) +
					(m1->m[row_offset + 3] * m2->m[column+12]);
		}
	}

	return out;
}


/**
 * @brief Matrix manipulation that achieves the zooming effect
 * @param m The matrix to be scaled
 * @param x Amount to scale in the x-direction
 * @param y Amount to scale in the y-direction
 * @param z Amount to scale in the z-direction
 */
void GLCamera::ScaleMatrix(Matrix *m, float x, float y, float z)
{
	Matrix scale = IDENTITY_MATRIX;

	scale.m[0] = x;
	scale.m[5] = y;
	scale.m[10] = z;

	memcpy(m->m, MultiplyMatrices(m, &scale).m, sizeof(m->m));
}


/**
 * @brief Matrix manipulation that achieves the panning effect
 * @param m The matrix to be translated
 * @param x Amount to translate in x-direction
 * @param y Amount to translate in y-direction
 * @param z Amount to translate in z-direction
 */
void GLCamera::TranslateMatrix(Matrix *m, float x, float y, float z)
{
	Matrix translation = IDENTITY_MATRIX;

	translation.m[12] = x;
	translation.m[13] = y;
	translation.m[14] = z;

	memcpy(m->m, MultiplyMatrices(m, &translation).m, sizeof(m->m));
}
