#ifndef OPENSTREETMAPLAYER_H
#define OPENSTREETMAPLAYER_H

#include <QString>

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/SolidShader.h"

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

		void	ToggleStreet();
		void	ToggleSatellite();
		void	ToggleTerrain();

		void	SetTestPoint(float x, float y);

	private:

		void	Initialize();
		void	InitializeRenderSurface();
		void	InitializeGL();
		void	InitializeTexture();

		void	LoadTexture();

		int	lonToTileX(float lon, int z);
		int	latToTileY(float lat, int z);
		float	tileXToLon(int x, int z);
		float	tileYToLat(int y, int z);

		void	LoadTile(float lat, float lon, int z);

		// Testing
		float	testX;
		float	testY;

		// Map Type
		MapType	currentType;
		bool	isVisible;

		// Test Tile
		struct TileStruct testTile;

		// Rendering surface geometry
		int	numNodes;
		int	numTriangles;
		float*	nodes;
		int*	triangles;

		// Rendering camera
		GLCamera*	camera;

		// Rendering shaders
		SolidShader*	outlineShader;
		SolidShader*	fillShader;

		// OpenGL rendering surface
		GLuint	VAOId;
		GLuint	VBOId;
		GLuint	IBOId;

		// Textures
		GLuint	texID;

};

#endif // OPENSTREETMAPLAYER_H
