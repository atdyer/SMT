#ifndef RECTANGLERENDERSURFACE_H
#define RECTANGLERENDERSURFACE_H

#include <QObject>
#include <QImage>

#include "OpenGL/GLData.h"
#include "OpenGL/Shaders/OverlayShader.h"

class RectangleRenderSurface : public QObject
{
		Q_OBJECT
	public:
		RectangleRenderSurface(QImage image, int windowWidth, int windowHeight);
		~RectangleRenderSurface();

		void	Render();
		void	SetViewportSize(int windowWidth, int windowHeight);

	private:

		int	width;
		int	height;
		int	windowWidth;
		int	windowHeight;

		int	numNodes;
		int	numElements;
		float*	nodes;
		int*	elements;

		QImage	imageData;

		GLuint	VAOId;
		GLuint	VBOId;
		GLuint	IBOId;
		GLuint	texID;

		OverlayShader*	shader;

		void	Initialize();
		void	InitializeGL();
		void	UpdateGL();




};

#endif // RECTANGLERENDERSURFACE_H
