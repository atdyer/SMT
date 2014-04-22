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

#include "TileServer.h"

TileServer::TileServer()
{
	tileCache = new TileCache();
	tileLoader = new TileLoader();

	connect(tileLoader, SIGNAL(tileLoaded(Tile*)), this, SLOT(tileLoaded(Tile*)));

	initialized = false;
	type = StreetTile;
	bottom = 0;
	height = 0;
	left = 0;
	width = 0;
	zoom = 0;
}


TileServer::~TileServer()
{
	if (tileCache)
		delete tileCache;

	if (tileLoader)
		delete tileLoader;
}


void TileServer::setBoundingBox(TileType tileType, int tileLeft, int tileBottom, int tileZoom)
{

	if (tileType == type && tileZoom == zoom && initialized)
	{

		// Check bounds

		int dx = left - tileLeft;
		int dy = bottom - tileBottom;

		if (dx != 0 || dy != 0)
			shiftBoundingBox(dx, dy);

	} else {

		// Zoom or type has changed (or we haven't been
		// initialized yet, clear tiles)
		type = tileType;
		left = tileLeft;
		bottom = tileBottom;
		zoom = tileZoom;

		newBoundingBox();

		initialized = true;

	}
}


void TileServer::setGridSize(int tilesWide, int tilesHigh)
{
	width = tilesWide;
	height = tilesHigh;

	if (initialized)
		newBoundingBox();
}


void TileServer::loadTile(TileType type, int x, int y, int zoom)
{
	bool inserted = (currentPool.insert(TilePoolObject(x, y, zoom))).second;

	if (inserted)
	{
		Tile *tile = 0;

		// Check cache first
		if (tileCache)
			tile = tileCache->GetTile(type, x, y, zoom);

		// It's in the cache, send it off to be rendered
		if (tile)
			emit tileAvailable(tile);

		// It's not in cache, send it off to be loaded from OSM
		else if (tileLoader)
			tileLoader->loadTile(type, x, y, zoom);
	}

}


void TileServer::newBoundingBox()
{
	currentPool.clear();

	if (tileLoader)
		tileLoader->skipAll(type);

	for (int i=left; i<width; ++i)
	{
		for (int j=bottom; j<height; ++j)
		{
			loadTile(type, i, j, zoom);
		}
	}
}


void TileServer::removeTile(TileType type, int x, int y, int zoom)
{
	int numRemoved = currentPool.erase(TilePoolObject(x, y, zoom));

	if (numRemoved > 0)
	{
		tileLoader->skipTile(type, x, y, zoom);
	}
}


void TileServer::shiftBoundingBox(int dx, int dy)
{
	if (dx < 0)
	{
		removeColumns(left+width+dx, left+width);
		addColumns(left+dx, left);
	}
	else if (dx > 0)
	{
		removeColumns(left, left+dx);
		addColumns(left+width, left+width+dx);
	}

	left += dx;

	if (dy < 0)
	{
		removeRows(bottom+height+dy, bottom+height);
		addRows(bottom+dy, bottom);
	}
	else if (dy > 0)
	{
		removeRows(bottom, bottom+dy);
		addRows(bottom+height, bottom+height+dy);
	}

	bottom += dy;
}


void TileServer::skipTile(TileType type, int x, int y, int zoom)
{
	if (tileLoader)
	{
		tileLoader->skipTile(type, x, y, zoom);
	}
}


void TileServer::addColumns(int l, int r)
{
	for (int x=l; x<l+r; ++x)
	{
		for (int y=bottom; y<bottom+height; ++y)
		{
			loadTile(type, x, y, zoom);
		}
	}
}


void TileServer::addRows(int b, int t)
{
	for (int x=left; x<left+width; ++x)
	{
		for (int y=b; y<b+t; ++y)
		{
			loadTile(type, x, y, zoom);
		}
	}
}


void TileServer::removeColumns(int l, int r)
{
	for (std::set<TilePoolObject>::iterator it = currentPool.begin();
	     it != currentPool.end();)
	{
		int x = (*it).x;
		if (l <= x && x < r)
		{
			currentPool.erase(it++);
			skipTile(type, x, (*it).y, zoom);
		}
		else
			++it;
	}
}


void TileServer::removeRows(int b, int t)
{
	for (std::set<TilePoolObject>::iterator it = currentPool.begin();
	     it != currentPool.end();)
	{
		int y = (*it).y;
		if (b <= y && y < t)
		{
			currentPool.erase(it++);
			skipTile(type, (*it).x, y, zoom);
		}
		else
			++it;
	}
}


void TileServer::tileLoaded(Tile *newTile)
{
	std::cout << "Tile Loaded: " <<
		     newTile->getTileX() << ", " <<
		     newTile->getTileY() << ", " <<
		     newTile->getZoom() << std::endl;

	if (tileCache)
		tileCache->AddTile(newTile);

	emit tileAvailable(newTile);

}
