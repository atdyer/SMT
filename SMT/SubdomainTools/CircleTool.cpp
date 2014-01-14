#include "CircleTool.h"


/**
 * @brief Constructor that initializes the tool with default values
 *
 * Constructor that initializes the tool with default values. By default,
 * the tool is not visible and operates on an 800x800 window.
 *
 */
CircleTool::CircleTool()
{
	fort14 = 0;
	terrain = 0;
	camera = 0;

	visible = false;
	mousePressed = false;

	xPixel = 0.0;
	xNormal = 0.0;
	xDomain = 0.0;
	yPixel = 0.0;
	yNormal = 0.0;
	yDomain = 0.0;
	edgeXPixel = 0.0;
	edgeYPixel = 0.0;
	edgeXNormal = 0.0;
	edgeYNormal = 0.0;
	edgeXDomain = 0.0;
	edgeYDomain = 0.0;
	radPixel = 0.0;
	radNormal = 0.0;
	radDomain = 0.0;

	w = 800;
	h = 800;
	l = -1.0;
	r = 1.0;
	b = -1.0;
	t = 1.0;

	quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

}


/**
 * @brief Destructor
 *
 * Destructor that only needs to clean up the glu object used to draw the circle
 *
 */
CircleTool::~CircleTool()
{
	if (quad)
		gluDeleteQuadric(quad);
}


/**
 * @brief Draws the circle
 *
 * This function draws the circle using the gluDisk function.
 *
 */
void CircleTool::Draw()
{
	if (visible)
	{
		glUseProgram(0); // Turn off any shaders that were previously being used
		glLoadIdentity();
		gluOrtho2D(l, r, b, t);
		glTranslatef(l+2*r*xPixel/w, t+2*b*yPixel/h, 0.0);
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluDisk(quad, 2*radPixel/h, 5.0, 100, 2);
		glTranslatef(-(l+2*r*xPixel/w), -(t+2*b*yPixel/h), 0.0);
	}
}


/**
 * @brief Sets the GLCamera that will be used to draw the circle.
 *
 * Sets the GLCamera that will be used to draw the circle. Typically, this
 * should be the same GLCamera being used to draw the TerrainLayer
 *
 * @param cam Pointer to the desired GLCamera
 */
void CircleTool::SetCamera(GLCamera *cam)
{
	camera = cam;
}


void CircleTool::SetFort14(Fort14 *newFort14)
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
void CircleTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


/**
 * @brief Sets internal values of the viewport size that are used to draw
 * the circle
 *
 * Sets the viewport size that is used to to draw the circle.
 * This needs to be called every time the size of the OpenGL context changes size.
 *
 * @param w The viewport width in pixels
 * @param h The viewport height in pixels
 */
void CircleTool::SetViewportSize(float w, float h)
{
	this->w = w;
	this->h = h;
	this->l = -1.0*w/h;
	this->r = 1.0*w/h;
	this->b = -1.0;
	this->t = 1.0;
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
}


/**
 * @brief Actions that are performed when a mouse button is pressed
 *
 * Actions that are performed when a mouse button is pressed. In this case,
 * a left click will drop the center of the circle and make the circle
 * visible.
 *
 * @param event The QMouseEvent object created by the GUI on the click
 */
void CircleTool::MouseClick(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		visible = true;
		mousePressed = true;
		SetCenter(event->x(), event->y());
		emit Instructions(QString("Drag to resize circle, drop to select elements"));
	}
}


/**
 * @brief Actions that are performed when the mouse is moved
 *
 * Actions that are performed when the mouse is moved.
 *
 * @param event The QMouseEvent object created by the GUI on the mouse move
 */
void CircleTool::MouseMove(QMouseEvent *event)
{
	if (mousePressed)
	{
		SetRadiusPoint(event->x(), event->y());
	}
}


/**
 * @brief Actions that are performed when a mouse button is released
 *
 * Actions that are performed when a mouse button is released. In this case,
 * if the left mouse button was just released (meaning we've been drawing the
 * circle), tell everyone we've finished drawing the circle by emitting
 * ToolFinishedDrawing().
 *
 * @param event The QMouseEvent object created by the GUI on the button release
 */
void CircleTool::MouseRelease(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mousePressed = false;
		emit Message(QString("Circle Tool:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Center: (<b>")
			     .append(QString::number(xDomain, 'g', 8))
			     .append("</b>, <b>")
			     .append(QString::number(yDomain, 'g', 8))
			     .append("</b>)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Radius: <b>")
			     .append(QString::number(radDomain))
			     .append("</b>"));
		emit ToolFinishedDrawing();
	}
}


/**
 * @brief Actions performed when the mouse wheel is used
 *
 * Actions performed when the mouse wheel is used. In this case, no action
 * is required.
 *
 */
void CircleTool::MouseWheel(QWheelEvent*)
{

}


/**
 * @brief Actions performed when a key is pressed
 *
 * Actions performed when a key is pressed. In this case, no action
 * is required
 *
 */
void CircleTool::KeyPress(QKeyEvent*)
{

}


/**
 * @brief Function called when the user wants to use to tool
 *
 * Function called when the user wants to use to tool. This resets the
 * tool to default values, preparing it for interaction with the user.
 *
 */
void CircleTool::UseTool()
{
	ResetTool();
	emit Instructions(QString("Click to drop circle center"));
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
std::vector<Node*> CircleTool::GetSelectedNodes()
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
std::vector<Element*> CircleTool::GetSelectedElements()
{
	FindElements();
	return selectedElements;
}


/**
 * @brief Set the center of the circle
 *
 * Sets the center of the circle.
 *
 * @param newX The x-coordinate of the circle center, in pixels
 * @param newY The y-coordiante of the circle center, in pixels
 */
void CircleTool::SetCenter(int newX, int newY)
{
	xPixel = newX;
	yPixel = newY;

	if (camera)
	{
		camera->GetUnprojectedPoint(xPixel, yPixel, &xNormal, &yNormal);
	} else {
		DEBUG("Circle Tool: No Camera");
	}

	if (terrain)
	{
		xDomain = terrain->GetUnprojectedX(xNormal);
		yDomain = terrain->GetUnprojectedY(yNormal);
	}
	else if (fort14)
	{
		xDomain = fort14->GetUnprojectedX(xNormal);
		yDomain = fort14->GetUnprojectedY(yNormal);
	} else {
		DEBUG("Circle Tool: No Terrain");
	}

	emit CircleStatsSet(xDomain, yDomain, radDomain);
}


/**
 * @brief Set the the second point that defines the radius of the circle
 *
 * Sets the point that is used to define the radius of the circle. The distance
 * between this point and the circle center define the radius, which is used
 * to draw that circle around the center point.
 *
 * @param newX The x-coordinate of the circle edge, in pixels
 * @param newY The y-coordinate of the circle edge, in pixels
 */
void CircleTool::SetRadiusPoint(int newX, int newY)
{
	edgeXPixel = newX;
	edgeYPixel = newY;

	if (camera)
	{
		camera->GetUnprojectedPoint(edgeXPixel, edgeYPixel, &edgeXNormal, &edgeYNormal);
	}
	if (terrain)
	{
		edgeXDomain = terrain->GetUnprojectedX(edgeXNormal);
		edgeYDomain = terrain->GetUnprojectedY(edgeYNormal);
	}
	else if (fort14)
	{
		edgeXDomain = fort14->GetUnprojectedX(edgeXNormal);
		edgeYDomain = fort14->GetUnprojectedY(edgeYNormal);
	}

	radPixel = Distance(xPixel, yPixel, edgeXPixel, edgeYPixel);
	radNormal = Distance(xNormal, yNormal, edgeXNormal, edgeYNormal);
	radDomain = Distance(xDomain, yDomain, edgeXDomain, edgeYDomain);

	emit CircleStatsSet(xDomain, yDomain, radDomain);
}


/**
 * @brief Searches the current TerrainLayer for Elements that fall within the currently
 * defined circle
 *
 * Searches the current TerrainLayer for Elements that fall within the currently
 * defined circle.
 *
 */
void CircleTool::FindElements()
{
	if (terrain)
	{
		selectedElements = terrain->GetElementsFromCircle(xNormal, yNormal, radNormal);
	}
	else if (fort14)
	{
		selectedElements = fort14->FindElementsInCircle(xNormal, yNormal, radNormal);
	}
}


/**
 * @brief Resets the tool to default values
 *
 * Resets the tool to default values.
 *
 */
void CircleTool::ResetTool()
{
	visible = false;
	xPixel = 0.0;
	xNormal = 0.0;
	xDomain = 0.0;
	yPixel = 0.0;
	yNormal = 0.0;
	yDomain = 0.0;
	radPixel = 0.0;
	radNormal = 0.0;
	radDomain = 0.0;
	selectedNodes.clear();
	selectedElements.clear();
}


/**
 * @brief Helper function that calculates the distance between two points
 *
 * @param x1 The x-coordinate of the first point
 * @param y1 The y-coordinate of the first point
 * @param x2 The x-coordinate of the second point
 * @param y2 The y-coordinate of the second point
 * @return The distance between the two points
 */
float CircleTool::Distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2-x1, 2.0)+pow(y2-y1, 2.0));
}
