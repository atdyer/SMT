#ifndef SELECTIONLAYER_H
#define SELECTIONLAYER_H

#include "Layers/Layer.h"
#include "Layers/TerrainLayer.h"

#include "Project/Files/Fort14.h"

#include "OpenGL/GLCamera.h"

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>


/**
 * @brief A virtual class that contains all of the generic functions needed for
 * a selection layer.
 *
 * A virtual class that contains all of the generic functions needed for a selection layer.
 * Subclasses of SelectionLayer should contain all of the tools necessary for performing
 * a specific subset of selection functionalities. For example, when creating a subdomain,
 * the user only needs to be able to select elements, so the selection layer that is
 * used for creating subdomains should only contain tools that are needed for selecting
 * elements.
 *
 * Because selections will always come from mesh data, every selection layer will have
 * a TerrainLayer that it gets that mesh data from.
 *
 */
class SelectionLayer : public Layer
{
		Q_OBJECT
	public:

		SelectionLayer(QObject *parent=0);

		virtual void	Draw() = 0;
		virtual void	LoadDataToGPU() = 0;

		virtual unsigned int	GetNumNodesSelected();
		virtual unsigned int	GetNumElementsSelected();

		virtual void	SetCamera(GLCamera *newCamera) = 0;
		virtual void	UseTool(ToolType tool, SelectionType selection) = 0;

		virtual void	MouseClick(QMouseEvent *event) = 0;
		virtual void	MouseMove(QMouseEvent *event) = 0;
		virtual void	MouseRelease(QMouseEvent *event) = 0;
		virtual void	MouseWheel(QWheelEvent *event) = 0;
		virtual void	KeyPress(QKeyEvent *event) = 0;
		virtual void	WindowSizeChanged(float w, float h) = 0;

		virtual void	Undo() = 0;
		virtual bool	GetUndoAvailable() = 0;
		virtual void	Redo() = 0;
		virtual bool	GetRedoAvailable() = 0;

	protected:

		/* OpenGL Variables */
		bool		glLoaded;	/**< Flag that shows if the OpenGL context has been initialized */
		GLCamera*	camera;		/**< The camera used in OpenGL drawing operations */
		GLuint		VAOId;		/**< The vertex array object ID */
		GLuint		VBOId;		/**< The vertex buffer object ID */
		GLuint		IBOId;		/**< The index buffer object ID */

		/* Flags */
		bool	mousePressed;		/**< Flag that shows if the mouse button is currently pressed */

	signals:

		void	Instructions(QString);
		void	MaxSelectedZ(float);
		void	Message(QString);
		void	MinSelectedZ(float);
		void	NumNodesSelected(int);
		void	NumElementsSelected(int);
		void	RedoAvailable(bool);
		void	Refreshed();
		void	ToolFinishedDrawing();
		void	UndoAvailable(bool);
};

#endif // SELECTIONLAYER_H
