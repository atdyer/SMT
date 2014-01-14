#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"

#include "Project/Files/Fort14.h"


/**
 * @brief A virtual class that provides all basic functionality needed for a
 * tool that is used to select Nodes or Elements
 *
 * A virtual class that provides all basic functionality needed for a
 * tool that is used to select Nodes or Elements.
 *
 * A subclass of QObject to provide Signal/Slot functionality.
 *
 */
class SelectionTool : public QObject
{
		Q_OBJECT
	public:
		SelectionTool();

		virtual void	Draw() = 0;
		virtual void	SetCamera(GLCamera *cam) = 0;
		virtual void	SetFort14(Fort14 *newFort14) = 0;
		virtual void	SetTerrainLayer(TerrainLayer *layer) = 0;
		virtual void	SetViewportSize(float w, float h) = 0;

		virtual void	MouseClick(QMouseEvent *event) = 0;
		virtual void	MouseMove(QMouseEvent *event) = 0;
		virtual void	MouseRelease(QMouseEvent *event) = 0;
		virtual void	MouseWheel(QWheelEvent *event) = 0;
		virtual void	KeyPress(QKeyEvent *event) = 0;

		virtual void	UseTool() = 0;

		virtual std::vector<Node*>	GetSelectedNodes() = 0;
		virtual std::vector<Element*>	GetSelectedElements() = 0;

	signals:

		void	Message(QString);	/**< Signal emitted containing a message to display */
		void	Instructions(QString);	/**< Signal emitted containing instructions to display */
		void	ToolFinishedDrawing();	/**< Signal emitted when to tool has finished drawing */
};

#endif // SELECTIONTOOL_H
