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

#ifndef OSMTILELAYER_H
#define OSMTILELAYER_H

#include <QObject>

#include "Project/Files/Fort14.h"
#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/OpenStreetMapShader.h"
#include "OpenStreetMap/Tiles/TileServer.h"
#include "OpenStreetMap/Surfaces/SquareRenderSurface.h"

#include <math.h>
#include <iostream>

class OSMTileLayer : public QObject
{
		Q_OBJECT
	public:
		OSMTileLayer();
		~OSMTileLayer();

		void	Draw();
		void	SetCamera(GLCamera *newCamera);
		void	SetFort14(Fort14 *newFort14);

		void	ToggleSatellite();
		void	ToggleStreet();
		void	ToggleTerrain();

	private:

		// The fort.14 file and camera
		Fort14*		fort14;
		GLCamera*	camera;

		// The tile server
		TileServer*	tileServer;

		// The map type and visibility flag
		MapType	currentType;
		bool	isVisible;

		// The rendering surface
		SquareRenderSurface*	renderSurface;
		int			surfaceDimension;

		// The currently visible tiles
		int	zoom;
		int	x;
		int	y;


		int	CalculateZoomLevel(int tileDim, float padding);
		void	RefreshTiles();

		int	lonToTileX(float lon, int z);
		int	latToTileY(float lat, int z);
		float	tileXToLon(int x, int z);
		float	tileYToLat(int y, int z);

	protected slots:

		void	tileAvailable(Tile* newTile);

	signals:

		void	updateGL();
};

#endif // OSMTILELAYER_H
