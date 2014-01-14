#ifndef CREATIONSELECTIONLAYER_H
#define CREATIONSELECTIONLAYER_H

#include <vector>
#include <stack>
#include <algorithm>

#include "Layers/Layer.h"
#include "Layers/SelectionLayer.h"
#include "Layers/TerrainLayer.h"
#include "Layers/Actions/ElementState.h"

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/SolidShader.h"

#include "SubdomainTools/SelectionTool.h"
#include "SubdomainTools/ClickTool.h"
#include "SubdomainTools/CircleTool.h"
#include "SubdomainTools/RectangleTool.h"
#include "SubdomainTools/PolygonTool.h"
#include "SubdomainTools/BoundaryFinder.h"

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>


/**
 * @brief A Layer class used for the creation of subdomains. Specializes in selecting
 * elements from large domains.
 *
 * This is a selection layer that is used only for selecting the elements needed
 * to create a subdomain. It is unable to select nodes separately from elements,
 * as this functionality is not needed when creating subdomains. A number of tools
 * are available for selecting (and deselecting) elements:
 * - CircleTool - select elements inside of a circle
 * - EllipseTool - select elements inside of an ellipse
 * - PolyLineTool - select elements inside of an arbitrary shape
 * - RectangleTool - select elements inside of a rectangle
 * - LineTool - select elements underneath a line
 *
 *
 * I decided to use the Vertex Buffer Object from the TerrainLayer for drawing
 * selected elements. This gives us a number of benefits over storing selected
 * nodes locally:
 * - Don't have to recalculate indices in local node list after every action
 * - Less local data means undo/redo stack can save state at each interaction
 *   without having to worry about memory. This means we don't have to worry
 *   about selecting only unique elements at each interaction.
 * - Space is saved on the GPU
 *
 * For the undo/redo stack, because the typical subdomain size is relatively
 * small (compared to memory available), we keep track of the complete state
 * of selected elements after each interaction.
 *
 */
class CreationSelectionLayer : public SelectionLayer
{
		Q_OBJECT
	public:
		CreationSelectionLayer();
		~CreationSelectionLayer();

		virtual void	Draw();
		virtual void	LoadDataToGPU();
		virtual void	SetData(QString fileLocation);
		virtual bool	DataLoaded();

		virtual unsigned int	GetNumElementsSelected();

		virtual void	SetCamera(GLCamera *newCamera);
		void	SetTerrainLayer(TerrainLayer* newLayer);
		virtual void	UseTool(ToolType tool, SelectionType selection);

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);
		void	WindowSizeChanged(float w, float h);

		virtual void	Undo();
		virtual bool	GetUndoAvailable();
		virtual void	Redo();
		virtual bool	GetRedoAvailable();

		std::vector<unsigned int>	GetBoundaryNodes();
		ElementState*			GetCurrentSelection();

	private:

		/* Layers */
		TerrainLayer*	terrainLayer;	/**< The Terrain Layer that the selections are taken from */


		/* Selection Tools */
		ToolType	activeToolType;	/**< The type of tool currently being used */
		SelectionTool*	activeTool;	/**< The tool currently being used */
		ClickTool*	clickTool;	/**< Tool for selecting elements one at a time */
		CircleTool*	circleTool;	/**< Tool for selecting elements inside of a circle */
		RectangleTool*	rectangleTool;	/**< Tool for selecting elements inside of a rectangle */
		PolygonTool*	polygonTool;	/**< Tool for selecting elements inside of a user defined polygon */
		BoundaryFinder*	boundaryFinder;	/**< Tool used for finding the boundary nodes of a selection */

		/* Selected Elements */
		ElementState*		selectedState;	/**< The current state of selected Elements */

		/* Boundary Nodes */
		std::vector<unsigned int>	boundaryNodes;	/**< List of boundary node numbers */

		/* Undo and Redo Stacks */
		std::stack<ElementState*, std::vector<ElementState*> >	undoStack;	/**< The undo stack */
		std::stack<ElementState*, std::vector<ElementState*> >	redoStack;	/**< The redo stack */

		/* Shaders */
		SolidShader*	outlineShader;	/**< The shader used to draw Element outlines */
		SolidShader*	fillShader;	/**< The shader used to draw Element fill */
		SolidShader*	boundaryShader;	/**< The shader used to draw the Subdomain boundary */

		/* OpenGL Functions */
		void	InitializeGL();

		/* Tool Initialization Functions */
		void	CreateClickTool();
		void	CreateCircleTool();
		void	CreateRectangleTool();
		void	CreatePolygonTool();

		/* Helper Functions */
		void	ClearUndoStack();
		void	ClearRedoStack();
		void	UseNewState(ElementState* newState);
		void	UseState(ElementState* state);
		void	GetSelectionFromActiveTool();

	signals:

		void	CircleToolStatsSet(float, float, float);
		void	RectangleToolStatsSet(float, float);

	private slots:

		/* Helper Slots */
		void	TerrainDataLoaded();
		void	GetSelectionFromTool();
};

#endif // CREATIONSELECTIONLAYER_H
