#include "RectangleRenderSurface.h"

RectangleRenderSurface::RectangleRenderSurface(QImage image, int windowWidth, int windowHeight)
{
	width = image.width();
	height = image.height();
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	numNodes = 4;
	numElements = 2;
	nodes = new float[numNodes*6];
	elements = new int[numElements*3];

	image.convertToFormat(QImage::Format_ARGB32);
	imageData = QGLWidget::convertToGLFormat(image);

	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	texID = 0;

	shader = 0;

	Initialize();
	InitializeGL();
}


RectangleRenderSurface::~RectangleRenderSurface()
{
	if (nodes)
		delete nodes;
	if (elements)
		delete elements;

	if (shader)
		delete shader;

	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);
	if (texID)
		glDeleteTextures(1, &texID);
}


void RectangleRenderSurface::Render()
{
	glBindVertexArray(VAOId);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
	if (shader && shader->Use() && texID)
	{
		glBindTexture(GL_TEXTURE_2D, texID);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}


void RectangleRenderSurface::SetViewportSize(int windowWidth, int windowHeight)
{
	float glWidth = ((float)width/(float)windowWidth) * 2.0;
	float glHeight = ((float)height/(float)windowHeight) * 2.0;

	float xLeft = 1.0 - glWidth;
	float xRight = 1.0;
	float yBottom = -1.0;
	float yTop = -1.0 + glHeight;

	int counter = 0;
	nodes[counter++] = xLeft;
	nodes[counter++] = yBottom;
	counter += 4;

	nodes[counter++] = xRight;
	nodes[counter++] = yBottom;
	counter += 4;

	nodes[counter++] = xLeft;
	nodes[counter++] = yTop;
	counter += 4;

	nodes[counter++] = xRight;
	nodes[counter++] = yTop;
	counter += 4;

	UpdateGL();
}


void RectangleRenderSurface::Initialize()
{
	// Create dimensions and connectivity
	float glWidth = ((float)width/(float)windowWidth) * 2.0;
	float glHeight = ((float)height/(float)windowHeight) * 2.0;

	float xLeft = 1.0 - glWidth;
	float xRight = 1.0;
	float yBottom = -1.0;
	float yTop = -1.0 + glHeight;

	int counter = 0;
	nodes[counter++] = xLeft;
	nodes[counter++] = yBottom;
	nodes[counter++] = 0.0;
	nodes[counter++] = 1.0;
	nodes[counter++] = 0.0;
	nodes[counter++] = 0.0;

	nodes[counter++] = xRight;
	nodes[counter++] = yBottom;
	nodes[counter++] = 0.0;
	nodes[counter++] = 1.0;
	nodes[counter++] = 1.0;
	nodes[counter++] = 0.0;

	nodes[counter++] = xLeft;
	nodes[counter++] = yTop;
	nodes[counter++] = 0.0;
	nodes[counter++] = 1.0;
	nodes[counter++] = 0.0;
	nodes[counter++] = 1.0;

	nodes[counter++] = xRight;
	nodes[counter++] = yTop;
	nodes[counter++] = 0.0;
	nodes[counter++] = 1.0;
	nodes[counter++] = 1.0;
	nodes[counter++] = 1.0;

	elements[0] = 0;
	elements[1] = 1;
	elements[2] = 3;
	elements[3] = 0;
	elements[4] = 2;
	elements[5] = 3;
}


void RectangleRenderSurface::InitializeGL()
{
	// Generate VAO and buffers
	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
	glGenBuffers(1, &IBOId);

	// Bind the VAO
	glBindVertexArray(VAOId);

	// Send the vertex data
	const size_t vboSize = 6*sizeof(GLfloat)*numNodes;
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *)(0 + 4*sizeof(GLfloat)));
	glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_STATIC_DRAW);
	GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData)
	{
		for (int i=0; i<numNodes; i++)
		{
			glNodeData[6*i+0] = (GLfloat)nodes[6*i+0];
			glNodeData[6*i+1] = (GLfloat)nodes[6*i+1];
			glNodeData[6*i+2] = (GLfloat)nodes[6*i+2];
			glNodeData[6*i+3] = (GLfloat)nodes[6*i+3];
			glNodeData[6*i+4] = (GLfloat)nodes[6*i+4];
			glNodeData[6*i+5] = (GLfloat)nodes[6*i+5];
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			std::cout << "Error unmapping buffer" << std::endl;
		}

	} else {
		std::cout << "Error loading map render surface nodes" << std::endl;
		return;
	}

	// Send index data
	const size_t iboSize = 3*sizeof(GLuint)*numElements;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, NULL, GL_STATIC_DRAW);
	GLuint* glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glElementData)
	{
		for (int i=0; i<numElements; i++)
		{
			glElementData[3*i+0] = (GLuint)elements[3*i+0];
			glElementData[3*i+1] = (GLuint)elements[3*i+1];
			glElementData[3*i+2] = (GLuint)elements[3*i+2];
		}

		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
		{
			std::cout << "Error unmapping buffer" << std::endl;
		}

	} else {
		std::cout << "Error loading map render surface elements" << std::endl;
		return;
	}

	// Finish up
	glBindVertexArray(0);

	// Load the texture
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D,
		     0,
		     GL_RGBA,
		     imageData.width(),
		     imageData.height(),
		     0,
		     GL_RGBA,
		     GL_UNSIGNED_BYTE,
		     imageData.bits());

	// Load the shader
	shader = new OverlayShader();
}


void RectangleRenderSurface::UpdateGL()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);

	GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData)
	{
		for (int i=0; i<numNodes; i++)
		{
			glNodeData[6*i+0] = (GLfloat)nodes[6*i+0];
			glNodeData[6*i+1] = (GLfloat)nodes[6*i+1];
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			std::cout << "Error unmapping buffer" << std::endl;
		}

	} else {
		std::cout << "Error loading map render surface nodes" << std::endl;
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}























