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

#ifndef TILELOADER_H
#define TILELOADER_H

#include <QObject>
#include <QThreadPool>

#include <OpenStreetMap/OpenStreetMapData.h>
#include <OpenStreetMap/Tiles/Tile.h>

#include <map>
#include <iostream>

class TileLoader : public QObject
{
		Q_OBJECT
	public:
		TileLoader();
		~TileLoader();

		void	loadTile(TileType type, int tileX, int tileY, int zoom);
		void	skipAll(TileType type);
		void	skipTile(TileType type, int tileX, int tileY, int zoom);


	private:

		QThreadPool*	threadPool;

		std::map<TilePoolObject, Tile*>	satellitePool;
		std::map<TilePoolObject, Tile*>	streetPool;
		std::map<TilePoolObject, Tile*>	terrainPool;

	signals:

		void	tileLoaded(Tile* tile);

	protected slots:

		void	loaded(bool isLoaded, Tile* tile);
};

#endif // TILELOADER_H
