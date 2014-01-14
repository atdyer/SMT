#ifndef TERRAINLAYER_H
#define TERRAINLAYER_H

#include "Quadtree/Quadtree.h"
#include "Layer.h"
#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/GradientShader.h"
#include "OpenGL/Shaders/CulledSolidShader.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <QThread>

#define BOUNDARY_PROGRESS_VALUE 100
#define QUADTREE_PROGRESS_VALUE 10000


/**
 * @brief This class is used to read and display terrain data from a fort.14 file
 *
 * This class is responsible for all data associated with an ADCIRC fort.14 file. It is
 * capable of reading the file, storing the data, and quickly accessing and drawing the
 * data as needed.
 *
 */
class TerrainLayer : public Layer
{
		Q_OBJECT
	public:

		/* Constructor/Destructor */
		TerrainLayer();
		~TerrainLayer();

		/* Virtual methods to override */
		virtual void	Draw();
		virtual void	LoadDataToGPU();
		virtual void	SetData(QString fileLocation);
		virtual bool	DataLoaded();

		/* Getter Methods */
		std::string		GetFort14Location();
		Node*			GetNode(unsigned int nodeNumber);
		Node*			GetNode(float x, float y);
		std::vector<Node*>	GetNodesFromCircle(float x, float y, float radius);
		Element*		GetElement(unsigned int elementNumber);
		Element*		GetElement(float x, float y);
		std::vector<Element>*	GetAllElements();
		std::vector<Element*>	GetElementsFromCircle(float x, float y, float radius);
		std::vector<Element*>	GetElementsFromRectangle(float l, float r, float b, float t);
		std::vector<Element*>	GetElementsFromPolygon(std::vector<Point> polyLine);
		unsigned int		GetNumNodes();
		unsigned int		GetNumElements();
		float			GetMinX();
		float			GetMaxX();
		float			GetMinY();
		float			GetMaxY();
		float			GetMinZ();
		float			GetMaxZ();
		float			GetUnprojectedX(float x);
		float			GetUnprojectedY(float y);
		ShaderType		GetOutlineShaderType();
		ShaderType		GetFillShaderType();
		QColor			GetSolidOutline();
		QColor			GetSolidFill();
		QColor			GetSolidBoundary();
		QGradientStops		GetGradientOutline();
		QGradientStops		GetGradientFill();
		QGradientStops		GetGradientBoundary();
		GLuint			GetVBOId();

		/* Setter Methods */
		virtual void	SetCamera(GLCamera *newCamera);
		void		SetFort14Location(std::string newLocation);
		void		SetSolidOutline(QColor newColor);
		void		SetSolidFill(QColor newColor);
		void		SetSolidBoundary(QColor newColor);
		void		SetGradientOutline(QGradientStops newStops);
		void		SetGradientFill(QGradientStops newStops);
		void		SetGradientBoundary(QGradientStops newStops);

		/* Visibility Methods */
		void	ToggleQuadtreeVisible();

		// Large Domain Functions
		void	UpdateZoomLevel(float zoomAmount);



	protected:

		/* Raw data from fort.14 */
		std::string			fort14Location;	/**< The absolute path of the fort.14 file */
		std::vector<Node>		nodes;		/**< List of all Nodes in the Layer */
		std::vector<Element>		elements;	/**< List of all Elements in the Layer */
		std::vector<unsigned int>	boundaryNodes;	/**< List of boundary node numbers if this is a subdomain */
		std::string			infoLine;	/**< The info line in the fort.14 file */
		unsigned int			numNodes;	/**< The number of Nodes in the domain as specified in fort.14 */
		unsigned int			numElements;	/**< The number of Elements in the domain as specified in fort.14 */

		/* Coordinate Properties */
		float			minX;		/**< The minimum x-value */
		float			midX;		/**< The calculated center of the x-values */
		float			maxX;		/**< The maximum x-value */
		float			minY;		/**< The minimum y-value */
		float			midY;		/**< the calculated center of the y-values */
		float			maxY;		/**< The maximum y-value */
		float			minZ;		/**< The minimum z-value */
		float			maxZ;		/**< The maximum z-value */
		float			max;		/**< The maximum of the width/height of the domain */

		/* All shaders needed to draw a terrain layer */
		GLShader*	outlineShader;		/**< Pointer to the GLShader object that will be used to draw the outline */
		GLShader*	fillShader;		/**< Pointer to the GLShader object that will be used to draw the fill */
		GLShader*	boundaryShader;		/**< Pointer to the GLShader object that will be used to draw the boundaries of a subdomain */

		// Terrain Specific OpenGL Variables
		GLuint		VAOId;			/**< The vertex array object ID in the OpenGL context */
		GLuint		VBOId;			/**< The vertex buffer object ID in the OpenGL context */
		GLuint		IBOId;			/**< The index buffer object ID in the OpenGL context */

		// Flags
		bool	flipZValue;		/**< Flag that determines if the z-value is multiplied by -1 before being loaded to the GPU */
		bool	fileLoaded;		/**< Flag that shows if data has been successfully read from the fort.14 file */
		bool	glLoaded;		/**< Flag that shows if data has been successfully sent to the GPU */
		bool	largeDomain;		/**< Flag that shows if the domain is extremely large (probably a full domain) */

		/* Quadtree and Large Domain Variables */
		Quadtree*	quadtree;	/**< The quadtree used for Node picking */
		bool		drawQuadtreeOutline;	/**< Flag that shows if we want to draw the quadtree outline */
		std::vector<std::vector<Element*>*>	visibleElementLists;	/**< The list of lists elements that are currently visible */
		int					numVisibleElements;	/**< The total number of elements that are currently visible */
		int					viewingDepth;

	private:

		bool		useCulledShaders;	/**< Flag that tells us if we need to use culled shaders */
		SolidShader*	solidOutline;		/**< Shader used to draw a solid outline */
		SolidShader*	solidFill;		/**< Shader used to draw a solid fill */
		SolidShader*	solidBoundary;		/**< Shader used to draw a solid boundary */
		GradientShader*	gradientOutline;	/**< Shader used to draw a gradient outline */
		GradientShader*	gradientFill;		/**< Shader used to draw a gradient fill */
		GradientShader*	gradientBoundary;	/**< Shader used to draw a gradient boundary */

		void	SwitchToCulledShaders();
		void	UpdateGradientShadersRange();
		void	CheckForLargeDomain();
		void	UpdateVisibleElements();

		/* File Reading Methods */
		unsigned int	CalculateTotalProgress(bool readNodes, bool readElements, bool readBoundaries, bool normalizeCoordinates, bool createQuadtree);
		unsigned int	ReadNodalData(unsigned int nodeCount, std::ifstream* fileStream);
		unsigned int	ReadNodalData(unsigned int nodeCount, std::ifstream *fileStream, unsigned int currProgress, unsigned int totalProgress);
		unsigned int	ReadElementData(unsigned int elementCount, std::ifstream* fileStream);
		unsigned int	ReadElementData(unsigned int elementCount, std::ifstream *fileStream, unsigned int currProgress, unsigned int totalProgress);
		unsigned int	ReadBoundaryNodes(std::ifstream *fileStream);
		unsigned int	ReadBoundaryNodes(std::ifstream *fileStream, unsigned int currProgress, unsigned int totalProgress);

		/* Data Processing Methods */
		unsigned int	NormalizeCoordinates();
		unsigned int	NormalizeCoordinates(unsigned int currProgress, unsigned int totalProgress);

	public slots:

		// Slots used for work that needs to be performed on a worker thread
		void	readFort14();		/**< Reads the fort.14 file */

	signals:

		// Signals used during threaded reading of fort.14
		void	fort14Valid();
		void	foundNumNodes(int);
		void	foundNumElements(int);
		void	finishedLoadingToGPU();
		void	error(QString err);
};

#endif // TERRAINLAYER_H
