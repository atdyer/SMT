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

#ifndef TILESERVER_H
#define TILESERVER_H

#include <QObject>

#include "OpenStreetMap/Tiles/TileCache.h"
#include "OpenStreetMap/Tiles/TileLoader.h"

#include <iostream>

class TileServer : public QObject
{
		Q_OBJECT
	public:
		TileServer();
		~TileServer();

		void	RunTest();

	private:

		TileCache*	tileCache;
		TileLoader*	tileLoader;

	protected slots:

		void	tileLoaded(Tile* newTile);
};

#endif // TILESERVER_H
