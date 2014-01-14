#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <string>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"
#include "SubdomainTools/SelectionTool.h"


/**
 * @brief A tool used to select Nodes or Elements by drawing a rectangle over
 * the desired area of a TerrainLayer object
 */
class RectangleTool : public SelectionTool
{
		Q_OBJECT
	public:
		RectangleTool();
		~RectangleTool();

		void	Draw();
		void	SetCamera(GLCamera* cam);
		void	SetFort14(Fort14 *newFort14);
		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetViewportSize(float w, float h);

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		void	UseTool();

		std::vector<Node*>	GetSelectedNodes();
		std::vector<Element*>	GetSelectedElements();

	private:

		Fort14*	fort14;
		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
		GLCamera*	camera;		/**< The GLCamera that is used to draw the TerrainLayer */

		/* Helper Functions */
		void	InitializeGL();
		void	UpdateGL();
		void	SetFirstCorner(int newX, int newY);
		void	SetSecondCorner(int newX, int newY);
		void	FindElements();
		void	ResetTool();
		void	CalculateVertexPoints();

		/* OpenGL Stuff */
		bool		glLoaded;		/**< Flag that shows if the VAO/VBO/IBO have been created */
		bool		visible;		/**< Flag that shows if the tool is currently visible */
		GLuint		VAOId;			/**< The vertex array object ID */
		GLuint		VBOId;			/**< The vertex buffer object ID */
		GLuint		IBOId;			/**< The index buffer object ID */
		SolidShader*	fillShader;		/**< The shader used to draw the selection tool */
		GLfloat		vertexPoints[12][4];	/**< The actual vertices used to draw the tool */
		GLuint		indexArray[8][3];	/**< The indices that make up the triangles */
		size_t		vertexBufferSize;	/**< The size of the vertex data in bytes */

		/* Mouse State */
		bool	mousePressed;	/**< Flag that shows if the left mouse button is pressed */

		/* Selected Nodes/Elements */
		std::vector<Node*>	selectedNodes;		/**< The list of currently selected Nodes */
		std::vector<Element*>	selectedElements;	/**< The list of currently selected Elements */

		/* Rectangle Attributes */
		float	firstCornerPixel[2];	/**< The coordinates of the first corner of the rectangle, in pixels */
		float	secondCornerPixel[2];	/**< The coordinates of the second corner of the rectangle, in pixels */

		float	firstCornerNormal[2];	/**< The coordinates of the first corner of the rectangle, in normalized OpenGL space */
		float	secondCornerNormal[2];	/**< The coordinates of the second corner of the rectangle, in normalized OpenGL space */

		float	firstCornerDomain[2];	/**< The coordinates of the first corner of the rectangle, in the original coordinate system of TerrainLayer */
		float	secondCornerDomain[2];	/**< The coordinates of the second corner of the rectangle, in the original coordinate system of TerrainLayer */

		/* Viewport Attributes */
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */

	signals:

		void	RectangleStatsSet(float w, float h);	/**< Signal emitted when the rectangle properties change: (width, height) */
};

#endif // RECTANGLETOOL_H
