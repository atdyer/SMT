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

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "OpenGL/GLData.h"
#include "OpenStreetMap/Tiles/Tile.h"

#include <iostream>

class TextureManager
{
	public:
		TextureManager(int width, int height);
		~TextureManager();

		GLuint	GetTexture(int x, int y);
		void	SetTexture(int x, int y, Tile *tile);

		void	HideAll();
		void	ShiftDown();
		void	ShiftLeft();
		void	ShiftRight();
		void	ShiftUp();

		void	PrintIndices();

	private:

		int	width;
		int	height;
		int	numTextures;

		int*	indices;
		bool*	loaded;
		GLuint*	textureIDs;
};

#endif // TEXTUREMANAGER_H
