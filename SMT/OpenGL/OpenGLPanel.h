#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "OpenGL/glew.h"
#include <QGLWidget>
#include <QWheelEvent>
#include <QContextMenuEvent>
#include <QAction>
#include <QActionGroup>
#include <QMenu>

#include "Layers/SelectionLayer.h"
#include "SubdomainTools/CircleTool.h"

#include "Project/Domains/Domain.h"

/**
 * @brief This is a custom widget that is used specifically for drawing Domain objects
 *
 * A custom widget used to draw Domain objects in an OpenGL context
 *
 */
class OpenGLPanel : public QGLWidget
{
		Q_OBJECT
	public:
		explicit	OpenGLPanel(QWidget *parent = 0);
		void		SetActiveDomain(Domain* newDomain);
		void		SetActiveDomainNew(Domain* newDomain);

	protected:

		Domain*		activeDomain;	/**< The Domain currently being displayed */
		Domain*		activeNewDomain;

		void	initializeGL();
		void	resizeGL(int w, int h);
		void	paintGL();

		// Mouse and key events
		void	wheelEvent(QWheelEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);
		void	mouseReleaseEvent(QMouseEvent *event);
		void	contextMenuEvent(QContextMenuEvent *event);

	private:

		int	viewportWidth;	/**< The width of the GL Panel in pixels */
		int	viewportHeight;	/**< The height of the GL Panel in pixels */

		/* Context Menu Stuff */
		QMenu*		rightClickMenu;
		QMenu*		matchColorsMenu;
		QMenu*		matchCameraMenu;

		QActionGroup*	colorActionGroup;
		QActionGroup*	cameraActionGroup;

		QAction*	colorOptionsAction;
		QAction*	zoomToFitAction;
		QAction*	clearSelectionsAction;
		QAction*	matchFullColorsAction;
		QAction*	matchFullCameraAction;

		void	CreateMenus();

	public slots:

		void	addSubdomainToMenus(QString domainName);

	private slots:

		void	UseCursor(const QCursor &cursorType);

	signals:

		void	emitMessage(QString);

		void	openColorOptions();
		void	matchColors(QAction*);
		void	matchCamera(QAction*);

};

#endif // OPENGLPANEL_H
