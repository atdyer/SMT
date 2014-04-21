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
}


TileServer::~TileServer()
{
	if (tileCache)
		delete tileCache;

	if (tileLoader)
		delete tileLoader;
}


void TileServer::RunTest()
{
	std::cout << "Starting TileServer Test" << std::endl;
	tileLoader->loadTile(StreetTile, 0, 0, 0);
}


void TileServer::tileLoaded(Tile *newTile)
{
	std::cout << "Tile Loaded: " <<
		     newTile->getTileX() << ", " <<
		     newTile->getTileY() << ", " <<
		     newTile->getZoom() << std::endl;

	delete newTile;

}
