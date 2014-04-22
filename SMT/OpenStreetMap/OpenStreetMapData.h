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

#ifndef OPENSTREETMAPDATA_H
#define OPENSTREETMAPDATA_H

#include <QString>

enum TileType {StreetTile, SatelliteTile, TerrainTile};

static const QString satelliteURL1 = "http://otile1.mqcdn.com/tiles/1.0.0/sat/";
static const QString satelliteURL2 = "http://otile2.mqcdn.com/tiles/1.0.0/sat/";
static const QString satelliteURL3 = "http://otile3.mqcdn.com/tiles/1.0.0/sat/";
static const QString satelliteURL4 = "http://otile4.mqcdn.com/tiles/1.0.0/sat/";
static const QString streetURLa = "http://a.tile.openstreetmap.org/";
static const QString streetURLb = "http://b.tile.openstreetmap.org/";
static const QString streetURLc = "http://c.tile.openstreetmap.org/";
static const QString terrainURL = "http://tile.stamen.com/terrain-background/";

// Data structure used to keep track of
// which tiles are in the thread pool
struct	TilePoolObject {
		int x;
		int y;
		int z;

		TilePoolObject(int x, int y, int z) :
			x(x), y(y), z(z)
		{}

		bool operator <(const TilePoolObject & t) const {
			if (t.z == this->z) {
				if (t.x == this->x) {
					return t.y < this->y;
				}
				return t.x < this->x;
			}
			return t.z < this->z;
		}

		bool operator ==(const TilePoolObject & t) const {
			return t.x == this->x && t.y == this->y && t.z == this->z;
		}
};

#endif // OPENSTREETMAPDATA_H
