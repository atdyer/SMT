/************************************************************************************************
* Copyright (C) 2014 by Tristan Dyer								*
*												*
* This file is part of SMT.									*
*												*
* SMT is free software: you can redistribute it and/or modify it under the terms of the		*
* GNU General Public License as published by the Free Software Foundation, either		*
* version 3 of the License, or (at your option) any later version.				*
*												*
* SMT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without	*
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	*
* GNU General Public License for more details.							*
*												*
* You should have received a copy of the GNU General Public License				*
* along with SMT.  If not, see <http://www.gnu.org/licenses/>.					*
*												*
************************************************************************************************/

#include "SquareRenderSurface.h"

SquareRenderSurface::SquareRenderSurface(int tilesDimension)
{
	dimension = tilesDimension;
	numTiles = dimension*dimension;
	numNodes = numTiles*4;
	numElements = numTiles*2;

	nodes = new float[numNodes*6];
	elements = new int[numElements*3];

	positionSet = false;
	currX = 0.0;
	currY = 0.0;

	camera = 0;

	Initialize();
	InitializeGL();
}


SquareRenderSurface::~SquareRenderSurface()
{
//	std::cout << "Deleting Render Surface" << std::endl;

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

	if (texManager)
		delete texManager;

//	std::cout << "Deleted Render Surface" << std::endl;
}


void SquareRenderSurface::ClearTextures()
{
	if (texManager)
		texManager->HideAll();
}


void SquareRenderSurface::Render()
{
	glBindVertexArray(VAOId);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glActiveTexture(GL_TEXTURE0);
	if (shader && shader->Use() && texManager)
	{
		int counter = 0;
		for (int i=0; i<dimension; ++i)
		{
			for (int j=0; j<dimension; ++j)
			{
				int texID = texManager->GetTexture(i, j);
				if (texID)
				{
					glBindTexture(GL_TEXTURE_2D, texID);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(6*(counter++)*sizeof(GLuint)));
				}
//				int index = counter++;
//				if (textureLoaded[i])
//				{
//					glBindTexture(GL_TEXTURE_2D, textureIDs[index]);
//					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(6*index*sizeof(GLuint)));
//				}
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}


void SquareRenderSurface::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;

	if (shader)
		shader->SetCamera(camera);
}


void SquareRenderSurface::SetTextureData(int x, int y, Tile *tile)
{
	if (texManager)
		texManager->SetTexture(x, y, tile);

	emit updateGL();
//	int index = dimension*x + y;

//	if (index < numTiles && tile && tile->getData())
//	{
//		glBindTexture(GL_TEXTURE_2D, textureIDs[index]);

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//		glTexImage2D(GL_TEXTURE_2D,
//			     0,
//			     GL_RGBA,
//			     tile->getWidth(),
//			     tile->getHeight(),
//			     0,
//			     GL_RGBA,
//			     GL_UNSIGNED_BYTE,
//			     tile->getData());

//		GLenum errorCheck = glGetError();
//		if (errorCheck != GL_NO_ERROR)
//		{
//			std::cout << "Error loading texture" << std::endl;
//		}

//		textureLoaded[index] = true;
//		emit updateGL();
//	}
}


void SquareRenderSurface::UpdateSurfacePosition(float x, float y, float width, float height)
{

	if (!positionSet)
	{
		currX = x;
		currY = y;
		positionSet = true;
	}
	else if (texManager)
	{

		if (x < currX)
			texManager->ShiftRight();
		else if (x > currX)
			texManager->ShiftLeft();
		if (y < currY)
			texManager->ShiftDown();
		else if (y > currY)
			texManager->ShiftUp();

		currX = x;
		currY = y;

	}

	int counter = 0;

	for (int i=0; i<dimension; ++i)
	{
		for (int j=0; j<dimension; ++j)
		{
			float xLeft = x + width*i;
			float xRight = x + width*(i+1);
			float yBottom = y + height*j;
			float yTop = y + height*(j+1);

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

		}
	}

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

//	emit updateGL();
}


void SquareRenderSurface::Initialize()
{
	int counter = 0;

	float stride = 2.0/dimension;

	for (int i=0; i<dimension; ++i)
	{
		for (int j=0; j<dimension; ++j)
		{
			float xLeft = -1.0 + stride*j;
			float xRight = -1.0 + stride*(j+1);
			float yBottom = -1.0 + stride*i;
			float yTop = -1.0 + stride*(i+1);

			nodes[counter++] = xLeft;
			nodes[counter++] = yBottom;
			nodes[counter++] = 0.0;
			nodes[counter++] = 1.0;
			nodes[counter++] = 0.0;
			nodes[counter++] = 1.0;

			nodes[counter++] = xRight;
			nodes[counter++] = yBottom;
			nodes[counter++] = 0.0;
			nodes[counter++] = 1.0;
			nodes[counter++] = 1.0;
			nodes[counter++] = 1.0;

			nodes[counter++] = xLeft;
			nodes[counter++] = yTop;
			nodes[counter++] = 0.0;
			nodes[counter++] = 1.0;
			nodes[counter++] = 0.0;
			nodes[counter++] = 0.0;

			nodes[counter++] = xRight;
			nodes[counter++] = yTop;
			nodes[counter++] = 0.0;
			nodes[counter++] = 1.0;
			nodes[counter++] = 1.0;
			nodes[counter++] = 0.0;
		}
	}

	counter = 0;

	for (int i=0, tile=0; i<numTiles; ++i, tile += 4)
	{
		elements[counter++] = tile;
		elements[counter++] = tile+1;
		elements[counter++] = tile+3;

		elements[counter++] = tile;
		elements[counter++] = tile+2;
		elements[counter++] = tile+3;
	}
}


void SquareRenderSurface::InitializeGL()
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


	// Build the shader
	shader = new OpenStreetMapShader();
	if (camera)
		shader->SetCamera(camera);


	// Create the textures
//	textureIDs = new GLuint[numTiles];
//	textureLoaded = new bool[numTiles];

//	if (textureIDs)
//		glGenTextures(numTiles, textureIDs);
//	ClearTextures();

	texManager = new TextureManager(dimension, dimension);
}
