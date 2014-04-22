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
#include <set>

class TileServer : public QObject
{
		Q_OBJECT
	public:
		TileServer();
		~TileServer();

		void	setBoundingBox(TileType tileType, int tileLeft, int tileBottom, int tileZoom);
		void	setGridSize(int tilesWide, int tilesHigh);

	private:

		bool		initialized;
		TileType	type;
		int		bottom;
		int		height;
		int		left;
		int		width;
		int		zoom;

		TileCache*	tileCache;
		TileLoader*	tileLoader;

		std::set<TilePoolObject>	currentPool;


		void	loadTile(TileType type, int x, int y, int zoom);
		void	newBoundingBox();
		void	removeTile(TileType type, int x, int y, int zoom);
		void	shiftBoundingBox(int dx, int dy);
		void	skipTile(TileType type, int x, int y, int zoom);

		void	addColumns(int l, int r);
		void	addRows(int b, int t);
		void	removeColumns(int l, int r);
		void	removeRows(int b, int t);



	signals:

		void	tileAvailable(Tile* newTile);

	protected slots:

		void	tileLoaded(Tile* newTile);
};

#endif // TILESERVER_H
