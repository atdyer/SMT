#include "OpenGLPanel.h"


OpenGLPanel::OpenGLPanel(QWidget *parent) :
	QGLWidget(parent)
{
	setFocus();
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);

	activeDomain = 0;
	activeNewDomain = 0;

	viewportWidth = 0.0;
	viewportHeight = 0.0;
}


/**
 * @brief This function is called when the OpenGL context is initialized
 *
 * Called when the OpenGL context is initialized. Use it to load all OpenGL extensions
 * and set all default drawing settings
 *
 */
void OpenGLPanel::initializeGL()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		DEBUG("Error: GLEW unable to load OpenGL Extensions");
		exit(EXIT_FAILURE);
	} else {
		DEBUG("OpenGL Extensions Loaded");
//		cout << "OpenGL Version: "  << glGetString(GL_VERSION) << endl;
//		cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
//              cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
//              cout << "OpenGL Platform Vendor: " << glGetString(GL_VENDOR) << endl;
//              cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	}

	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glClearColor(0.1, 0.2, 0.3, 1.0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}


/**
 * @brief Called with the size of the OpenGL context changes
 * @param w The new width
 * @param h The new height
 */
void OpenGLPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	viewportWidth = w;
	viewportHeight = h;

	if (activeDomain)
		activeDomain->SetWindowSize(w, h);
	if (activeNewDomain)
		activeNewDomain->SetWindowSize(w, h);
}


/**
 * @brief Called every time the OpenGL context needs to update the display
 */
void OpenGLPanel::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (activeDomain)
		activeDomain->Draw();
	if (activeNewDomain)
		activeNewDomain->Draw();
}


/**
 * @brief Event fired when the mouse wheel is scrolled
 * @param event
 */
void OpenGLPanel::wheelEvent(QWheelEvent *event)
{
	if (activeDomain)
		activeDomain->MouseWheel(event);
	if (activeNewDomain)
		activeNewDomain->MouseWheel(event);
}


/**
 * @brief Event fired when a mouse button is clicked
 * @param event
 */
void OpenGLPanel::mousePressEvent(QMouseEvent *event)
{
	if (activeDomain)
		activeDomain->MouseClick(event);
	if (activeNewDomain)
		activeNewDomain->MouseClick(event);
}


/**
 * @brief Event fired when the mouse is moved over the panel
 * @param event
 */
void OpenGLPanel::mouseMoveEvent(QMouseEvent *event)
{
	if (activeDomain)
		activeDomain->MouseMove(event);
	if (activeNewDomain)
		activeNewDomain->MouseMove(event);
}


/**
 * @brief Event fired when a mouse button is released
 * @param event
 */
void OpenGLPanel::mouseReleaseEvent(QMouseEvent *event)
{
	if (activeDomain)
		activeDomain->MouseRelease(event);
	if (activeNewDomain)
		activeNewDomain->MouseRelease(event);
}


/**
 * @brief Sets the visible Domain
 *
 * Sets the visible Domain. Does not destroy or keep track of any previously visible Domain.
 *
 * @param The Domain to make visible
 */
void OpenGLPanel::SetActiveDomain(Domain *newDomain)
{
	/* Disconnect the old domain */
	if (activeDomain)
	{
		disconnect(activeDomain, SIGNAL(UpdateGL()), this, SLOT(updateGL()));
	}

	/* Set up connections with the new one */
	activeDomain = newDomain;
	activeDomain->SetWindowSize(viewportWidth, viewportHeight);
	connect(activeDomain, SIGNAL(UpdateGL()), this, SLOT(updateGL()));
	connect(activeDomain, SIGNAL(SetCursor(QCursor)), this, SLOT(UseCursor(QCursor)));
}


void OpenGLPanel::SetActiveDomainNew(Domain *newDomain)
{
	if (activeNewDomain)
	{
		disconnect(activeNewDomain, SIGNAL(updateGL()), this, SLOT(updateGL()));
	}

	activeNewDomain = newDomain;
	activeNewDomain->SetWindowSize(viewportWidth, viewportHeight);
	connect(activeNewDomain, SIGNAL(updateGL()), this, SLOT(updateGL()));
	connect(activeNewDomain, SIGNAL(setCursor(QCursor)), this, SLOT(UseCursor(QCursor)));
	updateGL();
}


void OpenGLPanel::UseCursor(const QCursor &cursorType)
{
	setCursor(cursorType);
}
