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

#ifndef TILELOADERRUNNABLE_H
#define TILELOADERRUNNABLE_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include <QGLWidget>
#include <QThread>

#include <OpenStreetMap/OpenStreetMapData.h>

#include <curl/curl.h>
#include <iostream>

// Data structure used to hold the image
// data as it is retrieved from the OSM servers
struct	RawImageData {
		uchar*	memory;
		size_t	size;
		RawImageData() :
			memory((uchar*)malloc(1)),
			size(0)
		{}
};

class TileLoaderRunnable : public QObject, public QRunnable
{
		Q_OBJECT
	public:
		TileLoaderRunnable(TileType tileType, int tileX, int tileY, int zoom);
		~TileLoaderRunnable();

		void	run();

	private:

		struct	RawImageData	imageData;

		bool		skip;
		TileType	type;
		int		x;
		int		y;
		int		z;

		QString	BuildURL();


	public slots:

		void	setSkip(bool newSkip);

	signals:

		void	finished(QImage *data);


};

#endif // TILELOADERRUNNABLE_H
