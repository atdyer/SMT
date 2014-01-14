#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"
#include "SubdomainTools/SelectionTool.h"


/**
 * @brief A tool used to select Nodes or Elements by drawing a circle over
 * the desired area of a TerrainLayer object
 */
class CircleTool : public SelectionTool
{
		Q_OBJECT
	public:
		CircleTool();
		~CircleTool();

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
		GLCamera*	camera;		/**< The GLCamera that is being used to draw the TerrainLayer */

		/* Helper Functions */
		void	SetCenter(int newX, int newY);
		void	SetRadiusPoint(int newX, int newY);
		void	FindElements();
		void	ResetTool();
		float	Distance(float x1, float y1, float x2, float y2);

		// Selected Nodes/Elements
		std::vector<Node*>	selectedNodes;		/**< The list of currently selected Nodes */
		std::vector<Element*>	selectedElements;	/**< The list of currently selected Elements */

		// Circle attributes
		bool	visible;	/**< Flag that shows if the tool is visible */
		bool	mousePressed;	/**< Flag that shows if the left mouse button is pressed */

		float	xPixel;		/**< The x-coordinate of the circle center, in pixels */
		float	yPixel;		/**< The y-coordinate of the circle center, in pixels */
		float	xNormal;	/**< The x-coordinate of the circle center, in normalized OpenGL space */
		float	yNormal;	/**< The y-coordinate of the circle center, in normalized OpenGL space */
		float	xDomain;	/**< The x-coordinate of the circle center, in the original coordinate system of the TerrainLayer */
		float	yDomain;	/**< The y-coordinate of the circle center, in the original coordinate system of the TerrainLayer */

		float	edgeXPixel;	/**< The x-coordinate of the circle edge, in pixels */
		float	edgeYPixel;	/**< The y-coordinate of the circle edge, in pixels */
		float	edgeXNormal;	/**< The x-coordinate of the circle edge, in normalized OpenGL space */
		float	edgeYNormal;	/**< The y-coordinate of the circle edge, in normalized OpenGL space */
		float	edgeXDomain;	/**< The x-coordinate of the circle edge, the original coordinate system of the TerrainLayer */
		float	edgeYDomain;	/**< The y-coordinate of the circle edge, the original coordinate system of the TerrainLayer */

		float	radPixel;	/**< The radius of the circle, in pixels */
		float	radNormal;	/**< The radius of the circle, in normalized OpenGL space */
		float	radDomain;	/**< The radius of the circle, in the original coordinate system of the TerrainLayer */

		// Viewport attributes
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */

		// Use GLU to draw the circle for now
		GLUquadricObj*	quad;	/**< The quadric object used to draw the disk */

	signals:

		void	CircleStatsSet(float, float, float);	/**< Signal emitted when the circle properties change: (x-coordinate, y-coordinate, radius) */
};

#endif // CIRCLETOOL_H
