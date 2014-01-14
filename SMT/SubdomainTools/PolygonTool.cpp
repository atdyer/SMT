#include "PolygonTool.h"


/**
 * @brief Constructor that initializes the tool with default values
 *
 * Constructor that initializes the tool with default values. By default,
 * the tool is not visible and operates on an 800x800 window.
 *
 */
PolygonTool::PolygonTool()
{
	fort14 = 0;
	terrain = 0;
	camera = 0;

	glLoaded = false;
	visible = false;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	lineShader = 0;

	mouseX = 0.0;
	mouseY = 0.0;
	mousePressed = false;
	mouseMoved = false;

	pointCount = 0;

	w = 800;
	h = 800;
	l = -1.0;
	r = 1.0;
	b = -1.0;
	t = 1.0;
}


/**
 * @brief Destructor
 *
 * Destructor that cleans up memory allocated by the object and deletes
 * buffers in the OpenGL context.
 *
 */
PolygonTool::~PolygonTool()
{
	/* Clean up shader */
	if (lineShader)
		delete lineShader;

	/* Clean up OpenGL stuff */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);
}


/**
 * @brief Draws the polygon outline
 *
 * Draws the polygon if it is visible and the vertex data
 * has been loaded to the OpenGL context.
 *
 */
void PolygonTool::Draw()
{
	if (visible && glLoaded && pointCount)
	{
		glBindVertexArray(VAOId);
		if (lineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(2.0);
			if (lineShader->Use())
				glDrawElements(GL_LINE_LOOP, pointCount+1, GL_UNSIGNED_INT, (GLvoid*)0);
			glLineWidth(1.0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


/**
 * @brief Sets the GLCamera that will be used to draw the polygon outline
 *
 * Sets the GLCamera that will be used to draw the polygon. Typically, this
 * should be the same GLCamera being used to draw the TerrainLayer
 *
 * @param cam Pointer to the desired GLCamera
 */
void PolygonTool::SetCamera(GLCamera *cam)
{
	camera = cam;
	if (lineShader)
		lineShader->SetCamera(camera);
}


void PolygonTool::SetFort14(Fort14 *newFort14)
{
	fort14 = newFort14;
}


/**
 * @brief Sets the TerrainLayer that selections will be made from
 *
 * Sets the TerrainLayer that selections will be made from.
 *
 * @param layer Pointer to the desired TerrainLayer
 */
void PolygonTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


/**
 * @brief Sets internal values of the viewport size that are used to draw
 * the polygon
 *
 * Sets the viewport size that is used to to draw the rectangle.
 * This needs to be called every time the size of the OpenGL context changes size.
 *
 * @param w The viewport width in pixels
 * @param h The viewport height in pixels
 */
void PolygonTool::SetViewportSize(float w, float h)
{
	this->w = w;
	this->h = h;
	this->l = -1.0*w/h;
	this->r = 1.0*w/h;
	this->b = -1.0;
	this->t = 1.0;
}


/**
 * @brief Actions that are performed when a mouse button is pressed
 *
 * Actions that are performed when a mouse button is pressed. In this case,
 * no actual actions are performed, but a test for lef double-click is initialized.
 *
 * @param event The QMouseEvent object created by the GUI on the click
 */
void PolygonTool::MouseClick(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePressed = true;
		mouseMoved = false;
	}
}


/**
 * @brief Actions that are performed when the mouse is moved
 *
 * Actions that are performed when the mouse is moved. In this case,
 * if at least one point has been dropped, the current mouse location
 * is added to the end of the vertex list.
 *
 * @param event The QMouseEvent object created by the GUI on the mouse move
 */
void PolygonTool::MouseMove(QMouseEvent *event)
{
	mouseMoved = true;
	if (!mousePressed && camera)
	{
		camera->GetUnprojectedPoint(event->x(), event->y(), &mouseX, &mouseY);
		if (pointCount > 0)
			UpdateMouseVertex();
	}
}


/**
 * @brief Actions that are performed when a mouse button is released
 *
 * Actions that are performed when a mouse button is released. In this case,
 * if the left mouse button was just released and the mouse hasn't moved
 * since the click, we add that point to the vertex list. We then check for
 * a double click to see if the user is finished using the tool.
 *
 * @param event The QMouseEvent object created by the GUI on the button release
 */
void PolygonTool::MouseRelease(QMouseEvent *event)
{
	mousePressed = false;
	if (!mouseMoved && camera)
	{
		camera->GetUnprojectedPoint(event->x(), event->y(), &mouseX, &mouseY);
		if (CheckForDoubleClick(mouseX, mouseY))
		{
			FinishDrawingTool();
		} else {
			AddPoint(mouseX, mouseY);
		}
	}
}


/**
 * @brief Actions performed when the mouse wheel is used
 *
 * Actions performed when the mouse wheel is used. In this case, no action
 * is required.
 *
 */
void PolygonTool::MouseWheel(QWheelEvent *event)
{

}


/**
 * @brief Actions performed when a key is pressed
 *
 * Actions performed when a key is pressed. In this case, if the Enter key
 * is pressed, the user is finished using the tool.
 *
 */
void PolygonTool::KeyPress(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter)
	{
		emit ToolFinishedDrawing();
	}
}


/**
 * @brief Function called when the user wants to use to tool
 *
 * Function called when the user wants to use to tool. This resets the
 * tool to default values, preparing it for interaction with the user.
 *
 */
void PolygonTool::UseTool()
{
	ResetTool();
	if (!glLoaded)
		InitializeGL();
	visible = true;
}


/**
 * @brief Function used to query to the tool for all Nodes that were
 * selected in the last interaction
 *
 * <b> Not yet implemented </b>
 *
 * Function used to query to the tool for all Nodes that were
 * selected in the last interaction.
 *
 * @return A vector of pointers to all selected Nodes
 */
std::vector<Node*> PolygonTool::GetSelectedNodes()
{
	return selectedNodes;
}


/**
 * @brief Function used to query the tool for all Elements that were
 * selected in the last interaction
 *
 * Function used to query the tool for all Elements that were
 * selected in the last interaction.
 *
 * @return A vector of pointers to all selected Elements
 */
std::vector<Element*> PolygonTool::GetSelectedElements()
{
	if (terrain)
	{
		selectedElements = terrain->GetElementsFromPolygon(pointsList);
	}
	else if (fort14)
	{
		selectedElements = fort14->FindElementsInPolygon(pointsList);
	}
	return selectedElements;
}


/**
 * @brief Initializes this object's state on the OpenGL context
 *
 * Initializes this object's state on the OpenGL context by creating
 * the Vertex Array Object, Vertex Buffer Object, and Index Buffer
 * Object.
 *
 */
void PolygonTool::InitializeGL()
{
	if (!glLoaded)
	{
		if (!lineShader)
			lineShader = new SolidShader();
		lineShader->SetColor(QColor(0.0*255, 0.0*255, 0.0*255, 0.5*255));
		lineShader->SetCamera(camera);

		if (!VAOId)
			glGenVertexArrays(1, &VAOId);
		if (!VBOId)
			glGenBuffers(1, &VBOId);
		if (!IBOId)
			glGenBuffers(1, &IBOId);

		glBindVertexArray(VAOId);

		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
			} else {
				DEBUG("Polygon Tool Not Initialized");
				glLoaded = false;
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("Polygon Tool OpenGL Error: " << errString);
			glLoaded = false;
		}
	}
}


/**
 * @brief Updates the data in the Vertex Buffer Object on the OpenGL context
 *
 * Updates the Vertex Buffer Object with the current set of vertices. Replaces all
 * old data completely, so a resize of the buffer on the OpenGL context may occur.
 *
 */
void PolygonTool::UpdateVertexBuffer()
{
	if (glLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*(pointCount+1);

		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_DYNAMIC_DRAW);
		GLfloat* glNodeData = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			for (int i=0; i<pointCount; ++i)
			{
				glNodeData[4*i+0] = (GLfloat)pointsList[i].x;
				glNodeData[4*i+1] = (GLfloat)pointsList[i].y;
				glNodeData[4*i+2] = (GLfloat)1.0;
				glNodeData[4*i+3] = (GLfloat)1.0;
			}
			glNodeData[4*pointCount+0] = (GLfloat)mouseX;
			glNodeData[4*pointCount+1] = (GLfloat)mouseY;
			glNodeData[4*pointCount+2] = (GLfloat)1.0;
			glNodeData[4*pointCount+3] = (GLfloat)1.0;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}


/**
 * @brief Updates the data in the Index Buffer Object on the OpenGL context
 *
 * Updates the Index Buffer Object with the current set of indices. Replaces all
 * old data completely, so a resize of the buffer on the OpenGL context may occur.
 *
 */
void PolygonTool::UpdateIndexBuffer()
{
	if (glLoaded)
	{
		const size_t IndexBufferSize = sizeof(GLuint)*(pointCount+1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
		GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glElementData)
		{
			for (int i=0; i<pointCount+1; ++i)
			{
				glElementData[i] = i;
			}
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}


/**
 * @brief Updates the last vertex data on the OpenGL context
 *
 * Updates the last vertex data on the OpenGL context. Only changes the values
 * of the last vertex in the Vertex Buffer Object. Does not resize the VBO and
 * all other data remains intact.
 *
 */
void PolygonTool::UpdateMouseVertex()
{
	if (glLoaded)
	{
		const size_t OffsetSize = 4*sizeof(GLfloat)*pointCount;
		const size_t ReplacementSize = 2*sizeof(GLfloat);
		GLfloat Replacement[2] = {(GLfloat)mouseX, (GLfloat)mouseY};

		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferSubData(GL_ARRAY_BUFFER, OffsetSize, ReplacementSize, &Replacement[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


/**
 * @brief Adds a point to the list of vertices
 *
 * Adds a point to the list of vertices and updates the data on the
 * OpenGL context.
 *
 * @param x The x-coordinate (in OpenGL normalized space)
 * @param y The y-coordinate (int OpenGL normalized space)
 */
void PolygonTool::AddPoint(float x, float y)
{
	pointsList.push_back(Point(x, y));
	++pointCount;
	UpdateVertexBuffer();
	UpdateIndexBuffer();
}


/**
 * @brief Checks for a double-click
 *
 * Checks for a double click. If the given x-y coordinates are the same as
 * the x-y coordinates of the last dropped point, then a double click has
 * occurred.
 *
 * @param x The x-coordinate of the click
 * @param y The y-coordinate of the click
 * @return
 */
bool PolygonTool::CheckForDoubleClick(float x, float y)
{
	if (pointsList.size() > 0 && pointsList[pointsList.size()-1].x == x && pointsList[pointsList.size()-1].y == y)
		return true;
	return false;
}


/**
 * @brief Called when the user is finished using the tool
 *
 * Called when the user is finished using the tool. Hides the tool
 * and emits the ToolFinishedDrawing() signal.
 *
 */
void PolygonTool::FinishDrawingTool()
{
	visible = false;
	emit ToolFinishedDrawing();
}


/**
 * @brief Resets the tool to default values
 *
 * Resets the tool to default values.
 *
 */
void PolygonTool::ResetTool()
{
	pointsList.clear();
	pointCount = 0;
	mousePressed = false;
	mouseMoved = false;
}
