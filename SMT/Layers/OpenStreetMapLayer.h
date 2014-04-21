#ifndef OPENSTREETMAPLAYER_H
#define OPENSTREETMAPLAYER_H

#include <QString>

#include "Project/Files/Fort14.h"
#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/OpenStreetMapShader.h"

#include "OpenStreetMap/Tiles/TileServer.h"

#include <curl/curl.h>
#include <math.h>

//struct Vertex
//{
//		GLfloat position[4];
//		GLfloat texCoords[2];
//		GLuint	texID[1];
//};

struct TileStruct {
		uchar *memory;
		size_t size;
};

enum MapType {StreetMap, SatelliteMap, TerrainMap};

class OpenStreetMapLayer
{
	public:
		OpenStreetMapLayer();
		~OpenStreetMapLayer();

		void	Draw();
		void	SetCamera(GLCamera *newCamera);
		void	SetFort14(Fort14 *newFort14);

		void	ToggleStreet();
		void	ToggleSatellite();
		void	ToggleTerrain();

	private:

		void	Initialize();
		void	InitializeRenderSurfaceNew();
		void	InitializeRenderSurface();
		void	InitializeGL();
		void	InitializeTextures();

		int	CalculateZoomLevel(int numHorizontalTiles, float padding);
		void	RefreshTiles();

		void	LoadTextures(int tileX, int tileY, int zoom);
		void	LoadTexture();
		QImage	FetchTile(int x, int y, int zoom);
		void	LoadTile(float lat, float lon, int z);
		void	UpdateSurfacePositionNew(float x, float y, float width, float height);
		void	UpdateSurfacePosition(float x, float y, float width, float height);

		int	lonToTileX(float lon, int z);
		int	latToTileY(float lat, int z);
		float	tileXToLon(int x, int z);
		float	tileYToLat(int y, int z);



		// Fort.14 file
		Fort14*	fort14;

		// Map Type
		MapType	currentType;
		bool	isVisible;

		// Test Tile
		struct TileStruct testTile;

		// Tile data from OSM
		struct TileStruct osmTile;	// We'll be loading them one at a time for now

		// Rendering surface geometry
		int	surfaceTileDim;
		int	numTiles;
		int	surfaceDim;
		int	numNodes;
		int	numTriangles;
		float*	nodes;
		int*	triangles;

		// Rendering camera
		GLCamera*	camera;

		// Rendering shaders
		SolidShader*	outlineShader;
		SolidShader*	fillShader;
		OpenStreetMapShader*	mapShader;

		// OpenGL rendering surface
		GLuint	VAOId;
		GLuint	VBOId;
		GLuint	IBOId;

		// Textures
		GLuint	texID;
		GLuint*	texIDs;

		TileServer*	testServer;


};

#endif // OPENSTREETMAPLAYER_H
