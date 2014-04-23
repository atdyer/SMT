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

#include "TextureManager.h"

TextureManager::TextureManager(int width, int height)
{
	this->width = width;
	this->height = height;
	numTextures = width*height;

	indices = new int[numTextures];
	loaded = new bool[numTextures];
	textureIDs = new GLuint[numTextures];

	for (int i=0; i<numTextures; ++i)
		indices[i] = i;

	if (textureIDs)
		glGenTextures(numTextures, textureIDs);

}


TextureManager::~TextureManager()
{
	if (indices)
		delete indices;

	if (loaded)
		delete loaded;

	if (textureIDs)
	{
		glDeleteTextures(numTextures, textureIDs);
		delete textureIDs;
	}

}


GLuint TextureManager::GetTexture(int x, int y)
{
	if (y*width + x < numTextures)
	{
		if (loaded[y*width + x])
		{
			return textureIDs[indices[y*width + x]];
		}
	}
	return 0;
}


void TextureManager::SetTexture(int x, int y, Tile *tile)
{
	int index = indices[y*width + x];

	if (index < numTextures && tile && tile->getData())
	{

		glBindTexture(GL_TEXTURE_2D, textureIDs[index]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RGBA,
			     tile->getWidth(),
			     tile->getHeight(),
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,
			     tile->getData());

		loaded[index] = true;
	}
}


void TextureManager::HideAll()
{
	for (int i=0; i<numTextures; ++i)
		loaded[i] = false;
}


void TextureManager::ShiftDown()
{
	for (int col=0; col<width; ++col)
	{
		int bottomIndex = indices[col];
		for (int row=0; row<height-1; ++row)
		{
			indices[row*width + col] = indices[(row+1)*width + col];
		}
		indices[(height-1)*width + col] = bottomIndex;
		loaded[bottomIndex] = false;
	}
}


void TextureManager::ShiftLeft()
{
	for (int row=0; row<height; ++row)
	{
		int leftIndex = indices[row*width];
		for (int col=0; col<width-1; ++col)
		{
			indices[row*width + col] = indices[row*width + col+1];
		}
		indices[row*width + width-1] = leftIndex;
		loaded[leftIndex] = false;
	}
}


void TextureManager::ShiftRight()
{
	for (int row=0; row<height; ++row)
	{
		int rightIndex = indices[row*width + width-1];
		for (int col=width-1; col>0; --col)
		{
			indices[row*width + col] = indices[row*width + col-1];
		}
		indices[row*width] = rightIndex;
		loaded[rightIndex] = false;
	}
}


void TextureManager::ShiftUp()
{
	for (int col=0; col<width; ++col)
	{
		int topIndex = indices[(height-1)*width + col];
		for (int row=height-1; row>0; --row)
		{
			indices[row*width+col] = indices[(row-1)*width + col];
		}
		indices[col] = topIndex;
		loaded[topIndex] = false;
	}
}


void TextureManager::PrintIndices()
{
	for (int row=height-1; row>=0; --row)
	{
		for (int col=0; col<width; ++col)
		{
			std::cout << indices[row*width + col] << "\t";
		}
		std::cout << std::endl;
	}
}
