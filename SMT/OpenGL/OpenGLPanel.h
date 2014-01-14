#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "OpenGL/glew.h"
#include <QGLWidget>
#include <QWheelEvent>

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

	private:

		int	viewportWidth;	/**< The width of the GL Panel in pixels */
		int	viewportHeight;	/**< The height of the GL Panel in pixels */

	private slots:

		void	UseCursor(const QCursor &cursorType);

	signals:

		void	emitMessage(QString);

};

#endif // OPENGLPANEL_H
