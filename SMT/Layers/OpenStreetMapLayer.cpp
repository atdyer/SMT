#include "OpenStreetMapLayer.h"

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size*nmemb;
	struct TileStruct *mem = (struct TileStruct *)userp;

	mem->memory = (uchar*)realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL)
	{
		std::cout << "Not enough memory" << std::endl;
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

OpenStreetMapLayer::OpenStreetMapLayer()
{
	fort14 = 0;

	VAOId = 0;
	VBOId = 0;
	IBOId = 0;

	texID = 0;

	currentType = Street;
	isVisible = false;
	testTile.memory = (uchar*)malloc(1);
	testTile.size = 0;

	surfaceDim = 2;
	numNodes = surfaceDim*surfaceDim;
	numTriangles = (surfaceDim-1)*(surfaceDim-1)*2;
	nodes = 0;
	triangles = 0;

	camera = 0;
	outlineShader = 0;
	fillShader = 0;
	mapShader = 0;

	Initialize();
}


OpenStreetMapLayer::~OpenStreetMapLayer()
{
	if (nodes)
		delete nodes;
	if (triangles)
		delete triangles;

	if (outlineShader)
		delete outlineShader;
	if (fillShader)
		delete fillShader;
	if (mapShader)
		delete mapShader;

	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);

	if (texID)
		glDeleteTextures(1, &texID);

	if (testTile.memory)
		free(testTile.memory);
}


void OpenStreetMapLayer::Draw()
{
	if (isVisible)
	{
		glBindVertexArray(VAOId);

//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		if (fillShader && fillShader->Use())
//		{
//			glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (GLvoid*)0);
//		}

//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//		if (outlineShader && outlineShader->Use())
//		{
//			glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (GLvoid*)0);
//		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);
		if (mapShader && mapShader->Use())
		{

			glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (GLvoid*)0);

		}
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void OpenStreetMapLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;

	if (outlineShader)
		outlineShader->SetCamera(camera);
	if (fillShader)
		fillShader->SetCamera(camera);
	if (mapShader)
		mapShader->SetCamera(camera);
}


void OpenStreetMapLayer::ToggleStreet()
{
	if (!isVisible)
	{
		currentType = Street;
		isVisible = true;

		RefreshTile();


	} else if (currentType == Street) {
		isVisible = false;
	} else {
		currentType = Street;

		RefreshTile();
	}
}


void OpenStreetMapLayer::ToggleSatellite()
{
	if (!isVisible)
	{
		currentType = Satellite;
		isVisible = true;

		RefreshTile();

	} else if (currentType == Satellite) {
		isVisible = false;
	} else {
		currentType = Satellite;

		RefreshTile();
	}
}


void OpenStreetMapLayer::ToggleTerrain()
{
	if (!isVisible)
	{
		currentType = Terrain;
		isVisible = true;

		RefreshTile();

	} else if (currentType == Terrain) {
		isVisible = false;
	} else {
		currentType = Terrain;
		RefreshTile();
	}
}


void OpenStreetMapLayer::SetFort14(Fort14 *newFort14)
{
	fort14 = newFort14;
}


void OpenStreetMapLayer::Initialize()
{
	InitializeRenderSurface();
	InitializeGL();
	InitializeTexture();
}


void OpenStreetMapLayer::InitializeRenderSurface()
{
	nodes = new float[6*numNodes];
	triangles = new int[3*numTriangles];

	int counter = 0;

	for (int i=0; i<surfaceDim; ++i)
	{
		for (int j=0; j<surfaceDim; ++j)
		{
			float stride = 2.0/(surfaceDim-1);
			float x = -1.0 + stride*i;
			float y = -1.0 + stride*j;
			float z = 0.0;
			float w = 1.0;

//			std::cout << x << ", " << y << ", " << z << std::endl;

			nodes[counter++] = x;
			nodes[counter++] = y;
			nodes[counter++] = z;
			nodes[counter++] = w;
			nodes[counter++] = (x+1.0)/2.0;
			nodes[counter++] = 1.0 - (y+1.0)/2.0;

			std::cout << (x+1.0)/2.0 << ", " << (y+1.0)/2.0 << std::endl;
		}
	}

	std::cout << "Added " << counter << " nodal values" << std::endl;

	counter = 0;
	for (int i=0; i<(surfaceDim-1)*surfaceDim; i+=surfaceDim)
	{
		for (int outer=i, inner=i+surfaceDim+1; outer<i+surfaceDim-1; ++outer, ++inner)
		{
			int a = outer;
			int b = outer+1;
			int c = inner;
			int d = inner-1;

			triangles[counter++] = a;
			triangles[counter++] = b;
			triangles[counter++] = c;

			triangles[counter++] = a;
			triangles[counter++] = d;
			triangles[counter++] = c;
		}
	}

	std::cout << "Added " << counter << " triangles" << std::endl;
}


void OpenStreetMapLayer::InitializeGL()
{
	// Generate VAO and buffers
	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
	glGenBuffers(1, &IBOId);

	// Bind the VAO
	glBindVertexArray(VAOId);

	// Send the vertex data
	const size_t vboSize = 6*sizeof(GLfloat)*numNodes;
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *)(0 + 4*sizeof(GLfloat)));
	glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_STATIC_DRAW);
	GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData)
	{
		for (int i=0; i<numNodes; i++)
		{
			glNodeData[6*i+0] = (GLfloat)nodes[6*i+0];
			glNodeData[6*i+1] = (GLfloat)nodes[6*i+1];
			glNodeData[6*i+2] = (GLfloat)nodes[6*i+2];
			glNodeData[6*i+3] = (GLfloat)nodes[6*i+3];
			glNodeData[6*i+4] = (GLfloat)nodes[6*i+4];
			glNodeData[6*i+5] = (GLfloat)nodes[6*i+5];
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			std::cout << "Error unmapping buffer" << std::endl;
		}

	} else {
		std::cout << "Error loading map render surface nodes" << std::endl;
		return;
	}

	// Send index data
	const size_t iboSize = 3*sizeof(GLuint)*numTriangles;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboSize, NULL, GL_STATIC_DRAW);
	GLuint* glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glElementData)
	{
		for (int i=0; i<numTriangles; i++)
		{
			glElementData[3*i+0] = (GLuint)triangles[3*i+0];
			glElementData[3*i+1] = (GLuint)triangles[3*i+1];
			glElementData[3*i+2] = (GLuint)triangles[3*i+2];
		}

		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
		{
			std::cout << "Error unmapping buffer" << std::endl;
		}

	} else {
		std::cout << "Error loading map render surface elements" << std::endl;
		return;
	}

	// Finish up
	glBindVertexArray(0);


	// Build the shaders
	outlineShader = new SolidShader();
	outlineShader->SetColor(QColor(255, 0, 0, 255));
	if (camera)
		outlineShader->SetCamera(camera);

	fillShader = new SolidShader();
	fillShader->SetColor(QColor(0, 255, 0, 255));
	if (camera)
		fillShader->SetCamera(camera);

	mapShader = new OpenStreetMapShader();
	if (camera)
		mapShader->SetCamera(camera);
}


void OpenStreetMapLayer::InitializeTexture()
{
	glGenTextures(1, &texID);
}


int OpenStreetMapLayer::CalculateZoomLevel(int numHorizontalTiles, float padding)
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
		int newZoom = (int)floor(log2((padding*numHorizontalTiles*256.0) /geoWidth));

		if (newZoom > 19)
			newZoom = 19;
		if (newZoom < 0)
			newZoom = 0;

		std::cout << "Zoom Level: " << newZoom << std::endl;

		return newZoom;
	}

	return 0;

}


void OpenStreetMapLayer::RefreshTile()
{
	if (camera && fort14)
	{
//		int zoom = 13;
		int zoom = CalculateZoomLevel(4, 1.2);

		// Convert screen coordinates to normalized coordinates
		float normX, normY;
		camera->GetUnprojectedPoint(camera->GetViewportWidth()/2.0, camera->GetViewportHeight()/2.0, &normX, &normY);

		// Convert normalized coordiantes to lat/long
		float geoX = fort14->GetUnprojectedX(normX);
		float geoY = fort14->GetUnprojectedY(normY);

		// Load the tile
		LoadTile(geoY, geoX, zoom);
		LoadTexture();

		// Get the x, y tile coordinates
		int tileX = lonToTileX(geoX, zoom);
		int tileY = latToTileY(geoY, zoom);

		// Convert tile coordinates to lat/long
		float geoXnew = tileXToLon(tileX, zoom);
		float geoYnew = tileYToLat(tileY, zoom);
		float geoXnewNext = tileXToLon(tileX + 1, zoom);
		float geoYnewNext = tileYToLat(tileY + 1, zoom);

		// Convert lat/long to normalized coordinates
		float normXnew = fort14->GetNormalizedX(geoXnew);
		float normYnew = fort14->GetNormalizedY(geoYnew);
		float normXnewNext = fort14->GetNormalizedX(geoXnewNext);
		float normYnewNext = fort14->GetNormalizedY(geoYnewNext);

		// Calculate the width and height
		float width = normXnewNext - normXnew;
		float height = normYnewNext - normYnew;

		// Update the rendering surface
		UpdateSurfacePosition(normXnew, normYnew, width, height);

	}
}


void OpenStreetMapLayer::LoadTexture()
{
	QImage qI;
	bool loaded = qI.loadFromData(testTile.memory, (int)testTile.size, 0);
	if (loaded)
	{
		QImage formattedImage = qI.convertToFormat(QImage::Format_ARGB32);
		QImage texData = QGLWidget::convertToGLFormat(formattedImage);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texID);

		GLenum minFilter = GL_LINEAR;
		GLenum magFilter = GL_LINEAR;
		GLenum wrapMode = GL_CLAMP_TO_EDGE;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D,
			     0,
			     GL_RGBA,
			     texData.width(),
			     texData.height(),
			     0,
			     GL_RGBA,
			     GL_UNSIGNED_BYTE,
			     texData.bits());

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			std::cout << "Texture loaded" << std::endl;
		} else {
			std::cout << "Error loading texture" << std::endl;
		}

	} else {
		std::cout << "Unable to create QImage from data" << std::endl;
	}
}


void OpenStreetMapLayer::LoadTile(float lat, float lon, int z)
{
	int tileX = lonToTileX(lon, z);
	int tileY = latToTileY(lat, z);

	QString url;
	if (currentType == Street)
	{
		url = "http://a.tile.openstreetmap.org/" +
		      QString::number(z) +
		      "/" +
		      QString::number(tileX) +
		      "/" +
		      QString::number(tileY) +
		      ".png";
	}
	else if (currentType == Satellite)
	{
		url = "http://otile1.mqcdn.com/tiles/1.0.0/sat/" +
		      QString::number(z) +
		      "/" +
		      QString::number(tileX) +
		      "/" +
		      QString::number(tileY) +
		      ".jpg";
	}
	else if (currentType == Terrain)
	{
		url = "http://tile.stamen.com/terrain-background/" +
		      QString::number(z) +
		      "/" +
		      QString::number(tileX) +
		      "/" +
		      QString::number(tileY) +
		      ".jpg";
	} else {
		return;
	}

	std::cout << "Loading tile: " << url.toStdString().data() << std::endl;

	CURL *image = 0;

	image = curl_easy_init();

	if (testTile.memory)
		free(testTile.memory);
	testTile.memory = (uchar*)malloc(1);
	testTile.size = 0;

	if (image)
	{
		curl_easy_setopt(image, CURLOPT_URL, url.toStdString().data());
		curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(image, CURLOPT_WRITEDATA, (void *)&testTile);

		std::cout << "Sending tile map request" << std::endl;
		CURLcode fetchResult = curl_easy_perform(image);
		std::cout << "Retrieved" << std::endl;

		if (fetchResult != CURLE_OK)
		{
			std::cout << "Failed: " << curl_easy_strerror(fetchResult) << std::endl;
		}
	}

}


void OpenStreetMapLayer::UpdateSurfacePosition(float x, float y, float width, float height)
{
	int counter = 0;

	for (int i=0; i<surfaceDim; ++i)
	{
		for (int j=0; j<surfaceDim; ++j)
		{
			float strideWidth = width/(surfaceDim-1);
			float strideHeight = height/(surfaceDim-1);
			float xSurf = x + strideWidth*i;
			float ySurf = y + strideHeight*j;

			nodes[counter++] = xSurf;
			nodes[counter++] = ySurf;
			counter += 4;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData)
	{
		for (int i=0; i<numNodes; i++)
		{
			glNodeData[6*i+0] = (GLfloat)nodes[6*i+0];
			glNodeData[6*i+1] = (GLfloat)nodes[6*i+1];
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			std::cout << "Error unmapping buffer" << std::endl;
		}

	} else {
		std::cout << "Error loading map render surface nodes" << std::endl;
		return;
	}
}


int OpenStreetMapLayer::lonToTileX(float lon, int z)
{
	return (int)(floor((lon + 180.0) / 360.0 * pow(2.0, z)));
}


int OpenStreetMapLayer::latToTileY(float lat, int z)
{
	return (int)(floor((1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z)));
}


float OpenStreetMapLayer::tileXToLon(int x, int z)
{
	return x / pow(2.0, z) * 360.0 - 180;
}


float OpenStreetMapLayer::tileYToLat(int y, int z)
{
	double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
	return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

















