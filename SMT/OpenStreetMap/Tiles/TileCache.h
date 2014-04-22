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

#ifndef TILECACHE_H
#define TILECACHE_H

#include "OpenStreetMap/OpenStreetMapData.h"
#include "OpenStreetMap/Tiles/Tile.h"

#include <map>

class TileCache
{
	public:
		TileCache();
		~TileCache();

		void	AddTile(Tile* newTile);
		Tile*	GetTile(TileType type, int tileX, int tileY, int zoom);

	private:

		std::map<TilePoolObject, Tile*>	satellitePool;
		std::map<TilePoolObject, Tile*>	streetPool;
		std::map<TilePoolObject, Tile*>	terrainPool;

};

#endif // TILECACHE_H
