#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>
#include <QProgressBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "Project/Files/ProjectFile.h"
#include "Project/Files/Fort14.h"

#include "OpenGL/GLCamera.h"

#include "Layers/SelectionLayer.h"

class Domain : public QObject
{
		Q_OBJECT
	public:
		Domain(ProjectFile *projectFile, QObject *parent=0);
		~Domain();

		void		Draw();
		Fort14*	GetFort14();
		virtual QString	GetPath() = 0;
		virtual bool	IsFullDomain() = 0;

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		void	Redo();
		bool	RedoAvailable();

		void	SetCamera(GLCamera *newCam);
		void	SetProgressBar(QProgressBar* newBar);
		void	SetWindowSize(float w, float h);

		void	Undo();
		bool	UndoAvailable();
		void	UseTool(ToolType tool, SelectionType selection, SelectionMode mode);

	protected:

		GLCamera*		camera;
		Fort14*		fort14;
		QProgressBar*		progressBar;
		ProjectFile*	projectFile;
		SelectionLayer*		selectionLayer;

		/* Mouse Clicking and Moving Stuff */
		ActionType	currentMode;	/**< The current mode used to determine where actions are sent */
		int		oldx;	/**< The old x-coordinate of the mouse */
		int		oldy;	/**< The old y-coordinate of the mouse */
		int		newx;	/**< The new x-coordinate of the mouse */
		int		newy;	/**< The new y-coordinate of the mouse */
		int		dx;	/**< The change in x-coordinates between mouse events */
		int		dy;	/**< The change in y-coordinates between mouse events */
		Qt::MouseButton	pushedButton;	/**< The mouse button being pushed */
		bool		clicking;	/**< Flag that shows if a mouse button is being held down */
		bool		mouseMoved;	/**< Flag that shows if the mouse has moved since the last event */


		void		Zoom(float zoomAmount);
		void		Pan(float dx, float dy);
		void		CalculateMouseCoordinates();

		
	signals:

		void	updateGL();
		void	setCursor(const QCursor &);
		void	maxSelectedZ(float);
		void	minSelectedZ(float);
		void	mouseX(float);
		void	mouseY(float);
		void	undoAvailable(bool);
		void	redoAvailable(bool);
		void	numNodes(int);
		void	numNodesSelected(int);
		void	numElements(int);
		void	numElementsSelected(int);
		
	public slots:

		void	SetTerrainSolidOutline(QColor newColor);
		void	SetTerrainSolidFill(QColor newColor);
		void	SetTerrainGradientOutline(QGradientStops newStops);
		void	SetTerrainGradientFill(QGradientStops newStops);

	private slots:

		void	EnterDisplayMode();
		
};

#endif // DOMAIN_H
