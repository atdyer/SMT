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

#ifndef TILE_H
#define TILE_H

#include <QObject>
#include <QImage>

#include <OpenStreetMap/OpenStreetMapData.h>
#include <OpenStreetMap/Tiles/TileLoaderRunnable.h>

class Tile : public QObject
{
		Q_OBJECT
	public:
		Tile(TileType tileType, int tileX, int tileY, int tileZoom);
		~Tile();

		const uchar*		getData();
		TileLoaderRunnable*	getLoaderRunnable();
		int			getTileX();
		int			getTileY();
		TileType		getType();
		int			getZoom();

		void			setSkip(bool skip);

	private:

		QImage*			data;
		bool			isLoaded;
		TileLoaderRunnable*	loader;
		TileType		type;

		int			x;
		int			y;
		int			zoom;

		void	CleanupLoader();

	signals:

		void	loaded(bool isLoaded, Tile* tile);
		void	skip(bool isSkip);

	protected slots:

		void	threadFinished(QImage* newData);
};

#endif // TILE_H
