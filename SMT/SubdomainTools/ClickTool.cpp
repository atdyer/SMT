#include "ClickTool.h"


/**
 * @brief A constructor that initializes the tool with default values
 */
ClickTool::ClickTool()
{
	fort14 = 0;
	terrain = 0;
	camera = 0;

	glLoaded = false;
	visible = false;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	pointShader = 0;
	glPoint[0] = glPoint[1] = glPoint[2] =  0.0;
	glPoint[3] = 1.0;
	glIndex = 0;

	mousePressed = false;
	mouseMoved = false;
	oldX = 0;
	oldY = 0;
	xPixel = 0;
	yPixel = 0;
	xGL = 0.0;
	yGL = 0.0;
}


/**
 * @brief Destructor that cleans up memory allocated by the object and deletes
 * buffers in the OpenGL context
 */
ClickTool::~ClickTool()
{
	/* Clean up shader */
	if (pointShader)
		delete pointShader;

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
 * @brief Draws the clicked point
 *
 * Draws the clicked point.
 *
 */
void ClickTool::Draw()
{
	if (glLoaded && visible)
	{
		glBindVertexArray(VAOId);
		if (pointShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(10.0);
			if (pointShader->Use())
				glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*)0);
			glPointSize(1.0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


/**
 * @brief Sets the GLCamera that will be used to draw the clicked point
 *
 * Sets the GLCamera that will be used to draw the clicked point.
 *
 * @param cam The camera that will be used to draw the clicked point
 */
void ClickTool::SetCamera(GLCamera *cam)
{
	camera = cam;
	if (pointShader)
		pointShader->SetCamera(cam);
}


void ClickTool::SetFort14(Fort14 *newFort14)
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
void ClickTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


/**
 * @brief Called when viewport size changes
 *
 * Does nothing.
 *
 */
void ClickTool::SetViewportSize(float, float)
{

}


/**
 * @brief Actions that are performed when the mouse button is clicked
 *
 * Actions that are performed when the mouse button is clicked. Updates the
 * coordinates of the clicked point and sends them to the GPU
 *
 * @param event The QMouseEvent object created by the GUI on the click
 */
void ClickTool::MouseClick(QMouseEvent *event)
{
	mousePressed = true;
	mouseMoved = false;
	visible = true;
	xPixel = oldX = event->x();
	yPixel = oldY = event->y();

	UpdateCoordinates();

	if (glLoaded)
		UpdateGL();

	emit Instructions(QString("Release to select Element, move to pan"));
}


/**
 * @brief Actions that are performed when the mouse moves
 *
 * If the mouse button is pressed, the camera is panned.
 *
 */
void ClickTool::MouseMove(QMouseEvent *event)
{
	if (mousePressed && camera)
	{
		visible = false;
		mouseMoved = true;
		xPixel = event->x();
		yPixel = event->y();
		camera->Pan(xPixel-oldX, yPixel-oldY);
		oldX = xPixel;
		oldY = yPixel;

		emit Instructions(QString("Move to pan"));
	}
}


/**
 * @brief Actions that are performed when the left mouse button is released
 *
 * Actions that are performed when the left mouse button is released. Makes
 * tool invisible and tells everyone we're finished drawing.
 *
 * @param event The QMouseEvent object created by the GUI on the button release
 */
void ClickTool::MouseRelease(QMouseEvent *)
{
	mousePressed = false;
	visible = false;
	if (!mouseMoved)
	{
		emit ToolFinishedDrawing();
	} else {
		emit Instructions(QString("Click on any Element to select/deselect it"));
	}
}


/**
 * @brief Actions performed when the mouse wheel is used
 *
 * Actions performed when the mouse wheel is used. In this case, no action
 * is required.
 *
 */
void ClickTool::MouseWheel(QWheelEvent *)
{

}


/**
 * @brief Actions performed when a key is pressed
 *
 * Actions performed when a key is pressed. In this case, no action
 * is required
 *
 */
void ClickTool::KeyPress(QKeyEvent *)
{

}


/**
 * @brief Function called when the user wants to use to tool
 *
 * Function called when the user wants to use to tool. Initializes the
 * OpenGL context if it has not already done so.
 *
 */
void ClickTool::UseTool()
{
	if (!glLoaded)
		InitializeGL();
	emit Instructions(QString("Click on any Element to select/deselect it"));
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
std::vector<Node*> ClickTool::GetSelectedNodes()
{
	FindNode();
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
std::vector<Element*> ClickTool::GetSelectedElements()
{
	FindElement();
	return selectedElements;
}


/**
 * @brief Initializes this object's state on the OpenGL context
 *
 * Initializes this object's state on the OpenGL context by creating
 * the Vertex Array Object, Vertex Buffer Object, and Index Buffer
 * Object. The VBO and IBO data are sent to the OpenGL context.
 *
 */
void ClickTool::InitializeGL()
{
	if (!glLoaded)
	{
		if (!pointShader)
			pointShader = new SolidShader();
		pointShader->SetColor(QColor(0.0*255, 0.0*255, 0.0*255, 1.0*255));
		pointShader->SetCamera(camera);

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

		const size_t VertexBufferSize = 4*sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, &glPoint[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

		const size_t IndexBufferSize = 1*sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, &glIndex, GL_STATIC_DRAW);

		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
			} else {
				DEBUG("Rectangle Tool Not Initialized");
				glLoaded = false;
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("Rectangle Tool OpenGL Error: " << errString);
			glLoaded = false;
		}
	}
}


/**
 * @brief Transforms the pixel coordinates to OpenGL coordinates
 *
 * Transforms the pixel coordinates to OpenGL coordinates.
 *
 */
void ClickTool::UpdateCoordinates()
{
	if (camera)
	{
		camera->GetUnprojectedPoint(float(xPixel), float(yPixel), &xGL, &yGL);
		glPoint[0] = xGL;
		glPoint[1] = yGL;
	} else {
		xGL = xPixel;
		yGL = yPixel;
	}
}


/**
 * @brief Updates the x- and y-coordinates on the OpenGL context with
 * the first two values in the glPoint array
 *
 * Updates the x- and y-coordinates on the OpenGL context with
 * the first two values in the glPoint array.
 *
 */
void ClickTool::UpdateGL()
{
	if (!glLoaded)
		InitializeGL();

	if (glLoaded)
	{
		if (VBOId)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBOId);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(GLfloat), &glPoint[0]);
		}
	}
}


void ClickTool::FindNode()
{
	if (fort14)
	{
		selectedNodes.clear();
		Node* selectedNode = fort14->FindNode(xGL, yGL);
		if (selectedNode)
			selectedNodes.push_back(selectedNode);
	}
}


/**
 * @brief Searches the TerrainLayer for the clicked Element and adds it to the
 * selectedElements list if one is found
 *
 * Searches the TerrainLayer for the clicked Element and adds it to the
 * selectedElements list if one is found.
 *
 */
void ClickTool::FindElement()
{
	if (terrain)
	{
		selectedElements.clear();
		Element* selectedElement = terrain->GetElement(xGL, yGL);
		if (selectedElement)
			selectedElements.push_back(selectedElement);
	}
	else if (fort14)
	{
		selectedElements.clear();
		Element* selectedElement = fort14->FindElement(xGL, yGL);
		if (selectedElement)
			selectedElements.push_back(selectedElement);
	}
}
