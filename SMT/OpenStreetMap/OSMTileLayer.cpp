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

#include "OSMTileLayer.h"

OSMTileLayer::OSMTileLayer()
{
	surfaceDimension = 4;

	tileServer = new TileServer();
	tileServer->setGridSize(surfaceDimension, surfaceDimension);

	renderSurface = new SquareRenderSurface(surfaceDimension);

	connect(tileServer, SIGNAL(tileAvailable(Tile*)), this, SLOT(tileAvailable(Tile*)));
	connect(renderSurface, SIGNAL(updateGL()), this, SIGNAL(updateGL()));

	fort14 = 0;
	camera = 0;

	currentType = StreetMap;
	isVisible = false;

	zoom = 0;
	x = 0;
	y = 0;
}


OSMTileLayer::~OSMTileLayer()
{
//	std::cout << "Deleting Tile Layer" << std::endl;
	if (tileServer)
		delete tileServer;
	if (renderSurface)
		delete renderSurface;
//	std::cout << "Deleted Tile Layer" << std::endl;
}


void OSMTileLayer::Draw()
{
	if (isVisible)
	{
		// Before drawing, see if we're still looking at the same thing
		RefreshTiles();

		// Okay, now draw
		if (renderSurface)
			renderSurface->Render();
	}
}


void OSMTileLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;
	if (renderSurface)
		renderSurface->SetCamera(camera);
}


void OSMTileLayer::SetFort14(Fort14 *newFort14)
{
	fort14 = newFort14;
}


void OSMTileLayer::ToggleSatellite()
{
	if (!isVisible)
	{
		currentType = SatelliteMap;
		isVisible = true;

		RefreshTiles();
	} else if (currentType == SatelliteMap) {
		isVisible = false;
	} else {
		currentType = SatelliteMap;

		RefreshTiles();
	}
}


void OSMTileLayer::ToggleStreet()
{
	if (!isVisible)
	{
		currentType = StreetMap;
		isVisible = true;

		RefreshTiles();


	} else if (currentType == StreetMap) {
		isVisible = false;
	} else {
		currentType = StreetMap;

		RefreshTiles();
	}
}


void OSMTileLayer::ToggleTerrain()
{
	if (!isVisible)
	{
		currentType = TerrainMap;
		isVisible = true;

		RefreshTiles();

	} else if (currentType == TerrainMap) {
		isVisible = false;
	} else {
		currentType = TerrainMap;
		RefreshTiles();
	}
}


int OSMTileLayer::CalculateZoomLevel(int tileDim, float padding)
{
	if (camera && fort14)
	{
		// Convert the screen coordinates into normalized coordinates
		float screenLeft, screenRight, screenBottom, screenTop;
		camera->GetUnprojectedPoint(0.0, 0.0, &screenLeft, &screenTop);
		camera->GetUnprojectedPoint(camera->GetViewportWidth(), camera->GetViewportHeight(), &screenRight, &screenBottom);

		// Convert normalized coords into lat/long
		float geoLeft = fort14->GetUnprojectedX(screenLeft);
		float geoRight = fort14->GetUnprojectedX(screenRight);

		// Get width in degrees
		float geoWidth = geoRight - geoLeft;

		// Calculate zoom level
		int newZoom = (int)floor(log2((padding*tileDim*256.0) / geoWidth));

		if (newZoom > 19)
			newZoom = 19;
		if (newZoom < 0)
			newZoom = 0;

		return newZoom;
	}

	return 0;
}


void OSMTileLayer::RefreshTiles()
{
	if (tileServer && renderSurface && camera && fort14)
	{
		int newZoom = CalculateZoomLevel(surfaceDimension, 1.2);

		if (newZoom != zoom)
		{
			renderSurface->ClearTextures();
		}

		zoom = newZoom;

		// Convert screen coordinates to normalized coordinates
		float normX, normY;
		camera->GetUnprojectedPoint(0, 0, &normX, &normY);

		// Convert normalized coordiantes to lat/long
		float geoX = fort14->GetUnprojectedX(normX);
		float geoY = fort14->GetUnprojectedY(normY);

		// Get the x, y tile coordinates
		int newX = lonToTileX(geoX, zoom);
		int newY = latToTileY(geoY, zoom);

		if (newX != x || newY != y)
		{
			// Convert tile coordinates to lat/long
			float geoXnew = tileXToLon(newX, zoom);
			float geoYnew = tileYToLat(newY, zoom);
			float geoXnewNext = tileXToLon(newX + 1, zoom);
			float geoYnewNext = tileYToLat(newY + 1, zoom);

			// Convert lat/long to normalized coordinates
			float normXnew = fort14->GetNormalizedX(geoXnew);
			float normYnew = fort14->GetNormalizedY(geoYnew);
			float normXnewNext = fort14->GetNormalizedX(geoXnewNext);
			float normYnewNext = fort14->GetNormalizedY(geoYnewNext);

			// Calculate the width and height
			float width = normXnewNext - normXnew;
			float height = normYnewNext - normYnew;

			renderSurface->UpdateSurfacePosition(normXnew, normYnew, width, height);

			x = newX;
			y = newY;
		}

		if (currentType == SatelliteMap)
			tileServer->setBoundingBox(SatelliteTile, x, y, zoom);
		else if (currentType == StreetMap)
			tileServer->setBoundingBox(StreetTile, x, y, zoom);
		else if (currentType == TerrainMap)
			tileServer->setBoundingBox(TerrainTile, x, y, zoom);

	}
}


void OSMTileLayer::tileAvailable(Tile *newTile)
{
	if (newTile && renderSurface)
	{
		int tileX = newTile->getTileX();
		int tileY = newTile->getTileY();

		if (x <= tileX && tileX < x+surfaceDimension &&
		    y <= tileY && tileY < y+surfaceDimension)
		{
			renderSurface->SetTextureData(tileX - x,
						      tileY - y,
						      newTile);
		}
	}
}


int OSMTileLayer::lonToTileX(float lon, int z)
{
	return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z)));
}


int OSMTileLayer::latToTileY(float lat, int z)
{
	return (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
}


float OSMTileLayer::tileXToLon(int x, int z)
{
	return x / pow(2.0, z) * 360.0 - 180;
}


float OSMTileLayer::tileYToLat(int y, int z)
{
	double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}
