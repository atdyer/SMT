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

#ifndef SQUARERENDERSURFACE_H
#define SQUARERENDERSURFACE_H

#include <QObject>

#include "OpenGL/GLData.h"
#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/OpenStreetMapShader.h"

#include "OpenStreetMap/Tiles/Tile.h"

#include <iostream>

class SquareRenderSurface : public QObject
{
		Q_OBJECT
	public:
		SquareRenderSurface(int tilesDimension);
		~SquareRenderSurface();

		void	ClearTextures();
		void	Render();
		void	SetCamera(GLCamera *newCamera);
		void	SetTextureData(int x, int y, Tile *tile);
		void	UpdateSurfacePosition(float x, float  y, float width, float height);

	private:

		int	dimension;
		int	numTiles;
		int	numNodes;
		int	numElements;

		float*	nodes;
		int*	elements;

		// Vertex Array Object
		GLuint	VAOId;
		GLuint	VBOId;
		GLuint	IBOId;

		// Textures
		GLuint*	textureIDs;
		bool*	textureLoaded;

		// Shader
		OpenStreetMapShader*	shader;

		// Camera
		GLCamera*	camera;

		void	Initialize();
		void	InitializeGL();

	signals:

		void	updateGL();

};

#endif // SQUARERENDERSURFACE_H
