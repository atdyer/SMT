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

#include "TileLoaderRunnable.h"

static int currSatelliteURL = 1;
static QString nextSatelliteURL()
{
	currSatelliteURL = currSatelliteURL == 5 ? 1 : currSatelliteURL;
	QString url = "http://otile" + QString::number(currSatelliteURL++) + ".mqcdn.com/tiles/1.0.0/sat/";
	return url;
}

static int currStreetURL = 1;
static QString nextStreetURL()
{
	currStreetURL = currStreetURL == 4 ? 1 : currStreetURL;
	QString url = "http://" +
		      (currStreetURL == 1 ? QString('a') :
					    (currStreetURL == 2 ? QString('b') :
								  QString('c'))) +
		      ".tile.openstreetmap.org/";
	++currStreetURL;
	return url;
}

static int currTerrainURL = 1;
static QString nextTerrainURL()
{
	currTerrainURL = currTerrainURL == 5 ? 1 : currTerrainURL;
	QString url = "http://" +
		      (currTerrainURL == 1 ? QString('a') :
					     (currTerrainURL == 2 ? QString('b') :
								    currTerrainURL == 3 ? QString('c') :
											  QString('d'))) +
		      ".tile.stamen.com/terrain-background/";
	++currTerrainURL;
	return url;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size*nmemb;
	struct RawImageData *mem = (struct RawImageData *)userp;

	mem->memory = (uchar*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL)
	{
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

TileLoaderRunnable::TileLoaderRunnable(TileType tileType, int tileX, int tileY, int zoom)
{
	skip = false;
	type = tileType;
	x = tileX;
	y = tileY;
	z = zoom;

	setAutoDelete(false);
}


TileLoaderRunnable::~TileLoaderRunnable()
{
//	std::cout << "Deleting Tile Loader Runnable" << std::endl;
	if (imageData.memory)
		free(imageData.memory);
//	std::cout << "Deleted Tile Loader Runnable" << std::endl;
}


void TileLoaderRunnable::run()
{
//	std::cout << "Worker thread: " << QThread::currentThread() << std::endl;
	if (!skip)
	{

		// Load the tile
		QString imageURL = BuildURL();

		if (!imageURL.isEmpty())
		{

			CURL *image = 0;
			image = curl_easy_init();

			if (image)
			{
				curl_easy_setopt(image, CURLOPT_NOSIGNAL, 1);
				curl_easy_setopt(image, CURLOPT_URL, imageURL.toStdString().data());
				curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
				curl_easy_setopt(image, CURLOPT_WRITEDATA, (void*)&imageData);
				curl_easy_setopt(image, CURLOPT_NOSIGNAL, 1);
				curl_easy_setopt(image, CURLOPT_TIMEOUT, 3);

//				std::cout << "Requesting tile: " << imageURL.toStdString().data() << std::endl;
				CURLcode fetchResult = curl_easy_perform(image);
//				std::cout << "Requesting tile: " << imageURL.toStdString().data() << " - Received" << std::endl;

				QImage qImage;
				bool loaded = false;
				bool valid = false;

				if (fetchResult != CURLE_OK)
				{
					std::cout << "Failed to retrieve tile: "
						  << curl_easy_strerror(fetchResult) << std::endl;
					loaded = qImage.load(":/tiles/images/tiles/noload.png");
				} else {
					if (type == StreetTile)
						loaded = qImage.loadFromData(imageData.memory, (int)imageData.size, "PNG");
					else
						loaded = qImage.loadFromData(imageData.memory, (int)imageData.size, "JPG");

					// If it wasn't loaded, load the error image
					if (loaded)
						valid = true;
					else
						loaded = qImage.load(":/tiles/images/tiles/noload.png");
				}

				if (loaded)
				{
					// Convert format
					QImage formattedImage = qImage.convertToFormat(QImage::Format_ARGB32);

					// Create new image on heap that is a shallow copy of the GL version of the image
					QImage *finalImage = new QImage(QGLWidget::convertToGLFormat(formattedImage));
					emit finished(finalImage, valid);

				} else {
					std::cout << "Unable to load image data: " << imageURL.toStdString().data() << std::endl;
					emit finished(0, false);
				}

				curl_easy_cleanup(image);

			} else {
				emit finished(0, false);
			}

		} else {
			emit finished(0, false);
		}
	} else {
//		std::cout << "Skipping tile" << std::endl;
		emit finished(0, false);
	}
}


QString TileLoaderRunnable::BuildURL()
{
	QString url;
	if (type == SatelliteTile)
	{
		url = nextSatelliteURL() +
		      QString::number(z) + "/" +
		      QString::number(x) + "/" +
		      QString::number(y) + ".jpg";
	}
	else if (type == StreetTile)
	{
		url = nextStreetURL() +
		      QString::number(z) + "/" +
		      QString::number(x) + "/" +
		      QString::number(y) + ".png";
	}
	else if (type == TerrainTile)
	{
		url = nextTerrainURL() +
		      QString::number(z) + "/" +
		      QString::number(x) + "/" +
		      QString::number(y) + ".jpg";
	}
	return url;
}


void TileLoaderRunnable::setSkip(bool newSkip)
{
	skip = newSkip;
}
