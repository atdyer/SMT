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
	if (imageData.memory)
		free(imageData.memory);
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
				curl_easy_setopt(image, CURLOPT_URL, imageURL.toStdString().data());
				curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
				curl_easy_setopt(image, CURLOPT_WRITEDATA, (void*)&imageData);

				CURLcode fetchResult = curl_easy_perform(image);

				if (fetchResult == CURLE_OK)
				{

					QImage qImage;
					bool loaded = qImage.loadFromData(imageData.memory, (int)imageData.size, 0);
					if (loaded)
					{
						// Convert format
						QImage formattedImage = qImage.convertToFormat(QImage::Format_ARGB32);

						// Create new image on heap that is a shallow copy of the GL version of the image
						QImage *finalImage = new QImage(QGLWidget::convertToGLFormat(formattedImage));
						emit finished(finalImage);

					} else {
						emit finished(0);
					}

				} else {
					std::cout << "Failed to retrieve tile: "
						  << curl_easy_strerror(fetchResult) << std::endl;
					emit finished(0);
				}
			} else {
				emit finished(0);
			}

		} else {
			emit finished(0);
		}
	}
	emit finished(0);
}


QString TileLoaderRunnable::BuildURL()
{
	QString url;
	if (type == SatelliteTile)
	{
		url = satelliteURL1 +
		      QString::number(z) + "/" +
		      QString::number(x) + "/" +
		      QString::number(y) + ".jpg";
	}
	else if (type == StreetTile)
	{
		url = streetURLa +
		      QString::number(z) + "/" +
		      QString::number(x) + "/" +
		      QString::number(y) + ".png";
	}
	else if (type == TerrainTile)
	{
		url = terrainURL +
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
