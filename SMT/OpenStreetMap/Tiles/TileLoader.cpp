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

#include "TileLoader.h"

TileLoader::TileLoader()
{
	threadPool = QThreadPool::globalInstance();
	threadPool->setMaxThreadCount(1);
}


TileLoader::~TileLoader()
{
//	std::cout << "Deleting Tile Loader" << std::endl;
	if (threadPool)
		threadPool->waitForDone();
//	std::cout << "Deleted Tile Loader" << std::endl;
}


void TileLoader::loadTile(TileType type, int tileX, int tileY, int zoom)
{
	// First check tile pool to see if it's already in there
	int alreadyQueued = 0;
	TilePoolObject poolObj (tileX, tileY, zoom);
	if (type == SatelliteTile)
		alreadyQueued = satellitePool.count(poolObj);
	else if (type == StreetTile)
		alreadyQueued = streetPool.count(poolObj);
	else if (type == TerrainTile)
		alreadyQueued = terrainPool.count(poolObj);
	else
		return;

	if (alreadyQueued != 1)
	{
		// Tile isn't there, so create a new one

		// Create the new tile
		Tile *newTile = new Tile(type, tileX, tileY, zoom);

		// Connect signals and slots with the tile
		connect(newTile, SIGNAL(loaded(bool,Tile*)), this, SLOT(loaded(bool,Tile*)));

		// Add to the tile pool
		if (type == SatelliteTile)
			satellitePool[poolObj] = newTile;
		else if (type == StreetTile)
			streetPool[poolObj] = newTile;
		else if (type == TerrainTile)
			terrainPool[poolObj] = newTile;

//		std::cout << "Main Thread: " << QThread::currentThread() << std::endl;
		threadPool->start(newTile->getLoaderRunnable());

	} else {

		// Tile is there, make sure it's going to be loaded
		Tile* oldTile = 0;
		if (type == SatelliteTile)
			oldTile = satellitePool[poolObj];
		else if (type == StreetTile)
			oldTile = streetPool[poolObj];
		else if (type == TerrainTile)
			oldTile = terrainPool[poolObj];
		if (oldTile)
			oldTile->setSkip(false);
	}
}


void TileLoader::skipAll(TileType type)
{
	if (type == SatelliteTile)
	{
		for (std::map<TilePoolObject, Tile*>::iterator it = satellitePool.begin();
		     it != satellitePool.end(); ++it)
			(it->second)->setSkip(true);
	}
	else if (type == StreetTile)
	{
		for (std::map<TilePoolObject, Tile*>::iterator it = streetPool.begin();
		     it != streetPool.end(); ++it)
			(it->second)->setSkip(true);
	}
	else if (type == TerrainTile)
	{
		for (std::map<TilePoolObject, Tile*>::iterator it = terrainPool.begin();
		     it != terrainPool.end(); ++it)
			(it->second)->setSkip(true);
	}
}


void TileLoader::skipTile(TileType type, int tileX, int tileY, int zoom)
{
	TilePoolObject poolObj (tileX, tileY, zoom);
	Tile* oldTile = 0;

	// Look for the target tile (and avoid adding any empty slot
	// if it isn't there).
	if (type == SatelliteTile && satellitePool.count(poolObj))
		oldTile = satellitePool[poolObj];
	else if (type == StreetTile && streetPool.count(poolObj))
		oldTile = streetPool[poolObj];
	else if (type == TerrainTile && terrainPool.count(poolObj))
		oldTile = terrainPool[poolObj];

	// If it's in there, skip it
	if (oldTile)
		oldTile->setSkip(true);
}


void TileLoader::loaded(bool isLoaded, Tile *tile)
{
	if (tile)
	{
		// Remove from the tile pool
		TileType type = tile->getType();
		TilePoolObject poolObj (tile->getTileX(), tile->getTileY(), tile->getZoom());

		if (type == SatelliteTile)
			satellitePool.erase(poolObj);
		else if (type == StreetTile)
			streetPool.erase(poolObj);
		else if (type == TerrainTile)
			terrainPool.erase(poolObj);


		if (isLoaded)
		{
			// The tile was loaded, send it to be cached
			emit tileLoaded(tile);
		} else {
			// The tile wasn't loaded, get rid of it
			delete tile;
		}
	}
}
