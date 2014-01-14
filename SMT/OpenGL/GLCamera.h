#ifndef GLCAMERA_H
#define GLCAMERA_H

#include "GLData.h"
#include "adcData.h"
#include "string.h"


/**
 * @brief This class is used to represent a camera within the OpenGL context.
 *
 * The GLCamera class represents a camera within the OpenGL context. It keeps track
 * of all three matrices (model, view, and projection) used to draw object in the
 * context, as well as the values for panning/zooming that will be changed interactively
 * by the user.
 *
 * The camera looks down the z-axis on the ADCIRC layers from above, and is only capable
 * of panning in the x-y plane. It can also translate up and down on the z-axis, but cannot
 * rotate about any axis.
 *
 */
class GLCamera
{
	public:

		/* Constructor/Destructor */
		GLCamera();
		~GLCamera();

		// The Model, View, and Projection matrices, as well as the MVP matrix
		//	- Matrix multiplication is performed once on the CPU, as opposed to it
		//	  being performed continuously in the shader program
		Matrix	MVPMatrix;
		Matrix	ModelMatrix;
		Matrix	ViewMatrix;
		Matrix	ProjectionMatrix;

		// Pan and zoom variables
		float	panX;
		float	panY;
		float	zoomLevel;
		float	zoomScale;

		// Window/viewport variables
		float	window[6];
		float	viewportX;
		float	viewportY;
		float	pixelToViewRatio;

		// Public functions
		void	Pan(float dx, float dy);
		void	Zoom(float zoomAmount);
		void	SetWindowSize(float l, float r, float b, float t, float n, float f);
		void	ResetView();
		void	GetUnprojectedPoint(float x, float y, float *resultX, float *resultY);
		void	GetProjectedPoint(float x, float y, float *resultX, float *resultY);
		float	GetViewportWidth();
		float	GetViewportHeight();

		unsigned int		GetID();
		static unsigned int	GetNumCameras();

	private:

		static unsigned int	cameraCount;	/**< A running count of the number of GLCamera objects */
		static unsigned int	nextID;		/**< The next available cameraID */
		unsigned int		cameraID;	/**< A unique integer that identifies this GLCamera */

		// Functions used to update various matrices
		void	UpdateModel();
		void	UpdateProjection();
		void	UpdateMVP();

		// Matrix Math functions
		Matrix	MultiplyMatrices(const Matrix *m1, const Matrix *m2);
		void	ScaleMatrix(Matrix *m, float x, float y, float z);
		void	TranslateMatrix(Matrix *m, float x, float y, float z);
};

#endif // GLCAMERA_H
