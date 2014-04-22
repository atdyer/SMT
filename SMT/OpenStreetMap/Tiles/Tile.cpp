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

#include "Tile.h"

Tile::Tile(TileType tileType, int tileX, int tileY, int tileZoom)
{
	data = 0;
	isLoaded = false;
	type = tileType;
	x = tileX;
	y = tileY;
	zoom = tileZoom;

	loader = new TileLoaderRunnable(type, x, y, zoom);

	connect(this, SIGNAL(skip(bool)), loader, SLOT(setSkip(bool)));
	connect(loader, SIGNAL(finished(QImage*)), this, SLOT(threadFinished(QImage*)));
}


Tile::~Tile()
{
//	std::cout << "Deleting Tile" << std::endl;
	if (data)
		delete data;

	CleanupLoader();
//	std::cout << "Deleted Tile" << std::endl;
}


const uchar* Tile::getData()
{
	if (!data)
		return 0;
	return data->bits();
}


int Tile::getHeight()
{
	if (data)
		return data->width();
	return 0;
}


TileLoaderRunnable* Tile::getLoaderRunnable()
{
	return loader;
}


int Tile::getTileX()
{
	return x;
}


int Tile::getTileY()
{
	return y;
}


TileType Tile::getType()
{
	return type;
}


int Tile::getWidth()
{
	if (data)
		return data->width();
	return 0;
}


int Tile::getZoom()
{
	return zoom;
}


void Tile::setSkip(bool isSkip)
{
	emit skip(isSkip);
}


void Tile::threadFinished(QImage *newData)
{
	data = newData;

	if (data)
		emit loaded(true, this);
	else
//		std::cout << "Tile Not Loaded: " << x << ", " << y << ", " << zoom << std::endl;
		emit loaded(false, this);

	CleanupLoader();
}


void Tile::CleanupLoader()
{
	if (loader)
	{
		delete loader;
		loader = 0;
	}
}
