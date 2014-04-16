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
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;

	texID = 0;

	currentType = Street;
	isVisible = false;
	testTile.memory = (uchar*)malloc(1);
	testTile.size = 0;

	numNodes = 25;
	numTriangles = 32;
	nodes = 0;
	triangles = 0;

	camera = 0;
	outlineShader = 0;
	fillShader = 0;

	Initialize();

	LoadTile(34.6345, -76.5231, 15);
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

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (fillShader && fillShader->Use())
		{
			glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (outlineShader && outlineShader->Use())
		{
			glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

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
}


void OpenStreetMapLayer::ToggleStreet()
{
	if (!isVisible)
	{
		currentType = Street;
		isVisible = true;
	} else if (currentType == Street) {
		isVisible = false;
	} else {
		currentType = Street;
	}

	LoadTexture();
}


void OpenStreetMapLayer::ToggleSatellite()
{
	if (!isVisible)
	{
		currentType = Satellite;
		isVisible = true;
	} else if (currentType == Satellite) {
		isVisible = false;
	} else {
		currentType = Satellite;
	}
}


void OpenStreetMapLayer::ToggleTerrain()
{
	if (!isVisible)
	{
		currentType = Terrain;
		isVisible = true;
	} else if (currentType == Terrain) {
		isVisible = false;
	} else {
		currentType = Terrain;
	}
}


void OpenStreetMapLayer::SetTestPoint(float x, float y)
{
	testX = x;
	testY = y;
}


void OpenStreetMapLayer::Initialize()
{
	InitializeRenderSurface();
	InitializeGL();
	InitializeTexture();
}


void OpenStreetMapLayer::InitializeRenderSurface()
{
	nodes = new float[4*numNodes];
	triangles = new int[3*numTriangles];

	int counter = 0;

	for (int i=0; i<5; ++i)
	{
		for (int j=0; j<5; ++j)
		{
			float x = -1.0 + 0.5*i;
			float y = -1.0 + 0.5*j;
			float z = 0.0;
			float w = 1.0;

//			std::cout << x << ", " << y << ", " << z << std::endl;

			nodes[counter++] = x;
			nodes[counter++] = y;
			nodes[counter++] = z;
			nodes[counter++] = w;
		}
	}

	std::cout << "Added " << counter << " nodes" << std::endl;

	counter = 0;
	for (int i=0; i<20; i+=5)
	{
		for (int outer=i, inner=i+6; outer<i+4; ++outer, ++inner)
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
	const size_t vboSize = 4*sizeof(GLfloat)*numNodes;
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
	glBufferData(GL_ARRAY_BUFFER, vboSize, NULL, GL_STATIC_DRAW);
	GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (glNodeData)
	{
		for (int i=0; i<numNodes; i++)
		{
			glNodeData[4*i+0] = (GLfloat)nodes[4*i+0];
			glNodeData[4*i+1] = (GLfloat)nodes[4*i+1];
			glNodeData[4*i+2] = (GLfloat)nodes[4*i+2];
			glNodeData[4*i+3] = (GLfloat)nodes[4*i+3];
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
}


void OpenStreetMapLayer::InitializeTexture()
{
	glGenTextures(1, &texID);
}


void OpenStreetMapLayer::LoadTexture()
{
	QImage qI;
	bool loaded = qI.loadFromData(testTile.memory, (int)testTile.size, 0);
	if (loaded)
	{
		QImage texData = QGLWidget::convertToGLFormat(qI);
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

	CURL *image = 0;

	image = curl_easy_init();

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

















