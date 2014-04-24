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

#include "TileCache.h"

TileCache::TileCache()
{
	satellitePool.clear();
	streetPool.clear();
	terrainPool.clear();
}


TileCache::~TileCache()
{
	std::cout << "Deleting Tile Cache" << std::endl;
	std::cout << "   Satellite Tiles: " << satellitePool.size() << std::endl;
	for (std::map<TilePoolObject, Tile*>::iterator it = satellitePool.begin();
	     it != satellitePool.end(); ++it)
		delete it->second;

	std::cout << "   Street Tiles: " << streetPool.size() << std::endl;
	for (std::map<TilePoolObject, Tile*>::iterator it = streetPool.begin();
	     it != streetPool.end(); ++it)
		delete it->second;

	std::cout << "   Terrain Tiles: " << terrainPool.size() << std::endl;
	for (std::map<TilePoolObject, Tile*>::iterator it = terrainPool.begin();
	     it != terrainPool.end(); ++it)
		delete it->second;
//	std::cout << "Deleted Tile Cache" << std::endl;
}


void TileCache::AddTile(Tile *newTile)
{
	if (newTile)
	{
//		std::cout << "Adding tile to cache: " << newTile->getTileX() << ", " <<
//			     newTile->getTileY() << ", " << newTile->getZoom() << std::endl;

		TilePoolObject poolObj (newTile->getTileX(), newTile->getTileY(), newTile->getZoom());

		if (newTile->getType() == SatelliteTile)
		{
			if (satellitePool.count(poolObj))
				delete newTile;
			else
				satellitePool[poolObj] = newTile;
		}
		else if (newTile->getType() == StreetTile)
		{
			if (streetPool.count(poolObj))
				delete newTile;
			else
				streetPool[poolObj] = newTile;
		}
		else if (newTile->getType() == TerrainTile)
		{
			if (terrainPool.count(poolObj))
				delete newTile;
			else
				terrainPool[poolObj] = newTile;
		}
	}
}


Tile* TileCache::GetTile(TileType type, int tileX, int tileY, int zoom)
{
	TilePoolObject poolObj (tileX, tileY, zoom);

	if (type == SatelliteTile)
	{
		if (satellitePool.count(poolObj))
			return satellitePool[poolObj];
		else
			return 0;
	}
	else if (type == StreetTile)
	{
		if (streetPool.count(poolObj))
			return streetPool[poolObj];
		else
			return 0;
	}
	else if (type == TerrainTile)
	{
		if (terrainPool.count(poolObj))
			return terrainPool[poolObj];
		else
			return 0;
	}

	return 0;
}
