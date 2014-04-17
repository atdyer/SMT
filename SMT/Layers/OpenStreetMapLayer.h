#ifndef OPENSTREETMAPLAYER_H
#define OPENSTREETMAPLAYER_H

#include <QString>

#include "Project/Files/Fort14.h"
#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/OpenStreetMapShader.h"

#include <curl/curl.h>
#include <math.h>

struct TileStruct {
		uchar *memory;
		size_t size;
};

enum MapType {Street, Satellite, Terrain};

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
		void	InitializeRenderSurface();
		void	InitializeGL();
		void	InitializeTexture();

		int	CalculateZoomLevel(int numHorizontalTiles, float padding);
		void	RefreshTile();
		void	LoadTexture();
		void	LoadTile(float lat, float lon, int z);
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

		// Rendering surface geometry
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

};

#endif // OPENSTREETMAPLAYER_H
