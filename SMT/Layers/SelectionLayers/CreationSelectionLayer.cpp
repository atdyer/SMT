#include "CreationSelectionLayer.h"

/**
 * @brief Constructor initializes all variables to default values
 *
 * Constructor initializes all variables to default values and pointers
 * to zero.
 *
 */
CreationSelectionLayer::CreationSelectionLayer()
{
	activeToolType = CircleToolType;
	activeTool = 0;
	clickTool = 0;
	circleTool = 0;
	rectangleTool = 0;
	polygonTool = 0;
	boundaryFinder = new BoundaryFinder();

	selectedState = 0;

	glLoaded = false;
	camera = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	outlineShader = 0;
	fillShader = 0;
	boundaryShader = 0;

	mousePressed = false;
	CreateClickTool();
	CreateCircleTool();
	CreateRectangleTool();
	CreatePolygonTool();
}


/**
 * @brief Deconstructor that cleans up data on the GPU and any allocated data
 *
 * Cleans up any data that has been created by this class. Marks data for
 * deletion on the GPU. Note that we are not responsible for cleaning up
 * the VBO.
 *
 */
CreationSelectionLayer::~CreationSelectionLayer()
{
	DEBUG("Deleting Creation Selection Layer. Layer ID: " << GetID());

	/* Clean up shaders */
	if (outlineShader)
		delete outlineShader;
	if (fillShader)
		delete fillShader;
	if (boundaryShader)
		delete boundaryShader;

	// Clean up the OpenGL stuff
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Note that we aren't responsible for cleaning up the VBO */

	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);

	/* Delete all tools */
	if (clickTool)
		delete clickTool;
	if (circleTool)
		delete circleTool;
	if (rectangleTool)
		delete rectangleTool;
	if (polygonTool)
		delete polygonTool;
	if (boundaryFinder)
		delete boundaryFinder;

	/* Delete all states */
	if (selectedState)
		delete selectedState;
	ClearUndoStack();
	ClearRedoStack();
}


/**
 * @brief Draws the selected Elements
 *
 * Draws the currently selected Elements (fill and then outline), as well as boundary
 * segments if they are defined. Also draws any tool that is currently in use.
 *
 */
void CreationSelectionLayer::Draw()
{
	if (glLoaded && selectedState)
	{
		unsigned int numElements = selectedState->GetState()->size();

		glBindVertexArray(VAOId);

		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (fillShader->Use())
				glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (outlineShader->Use())
				glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (boundaryShader && boundaryNodes.size())
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0);
			if (boundaryShader->Use())
				glDrawElements(GL_LINE_STRIP, boundaryNodes.size(), GL_UNSIGNED_INT, (GLvoid*)(0 + sizeof(GLuint)*numElements*3));
			glLineWidth(1.0);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (activeTool)
		activeTool->Draw();
}


/**
 * @brief Loads the currently selected Element data to the GPU
 *
 * Loads the currently selected Element data to the GPU, getting rid of any element
 * data that is already there.
 *
 */
void CreationSelectionLayer::LoadDataToGPU()
{
	/* Make sure we've got all of the necessary Buffer Objects created */
	if (!glLoaded)
		InitializeGL();

	/* Make sure initialization succeeded */
	if (glLoaded && selectedState)
	{
		/* Load the connectivity data (elements) to the GPU, getting rid of any data that's already there */
		std::vector<Element*> *currSelection = selectedState->GetState();
		const size_t IndexBufferSize = 3*sizeof(GLuint)*currSelection->size() + sizeof(GLuint)*boundaryNodes.size();
		if (IndexBufferSize && VAOId && IBOId)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
			GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (glElementData)
			{
				Element* currElement;
				unsigned int i=0;
				unsigned int currSelectionSize = currSelection->size();
				for (std::vector<Element*>::iterator it=currSelection->begin(); it != currSelection->end(); ++it, i++)
				{
					currElement = *it;
					glElementData[3*i+0] = (GLuint)currElement->n1->nodeNumber-1;
					glElementData[3*i+1] = (GLuint)currElement->n2->nodeNumber-1;
					glElementData[3*i+2] = (GLuint)currElement->n3->nodeNumber-1;
				}
				for (i=0; i<boundaryNodes.size(); i++)
				{
					glElementData[3*currSelectionSize+i] = boundaryNodes[i]-1;
				}
			} else {
				glLoaded = false;
				DEBUG("ERROR: Mapping index buffer for Subdomain Creation Selection Layer " << GetID());
				emit emitMessage("<p style:color='red'><strong>Error: Unable to load index data to GPU (Subdomain Creation Selection Layer)</strong>");
				return;
			}

			if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
			{
				glLoaded = false;
				DEBUG("ERROR: Unmapping index buffer for Subdomain Creation Selection Layer " << GetID());
				return;
			}
		}

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
			} else {
				DEBUG("Subdomain Creation Selection Layer Data Not Loaded");
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("CreationSelectionLayer OpenGL Error: " << errString);
			glLoaded = false;
		}

		emit Refreshed();
		emit NumElementsSelected(currSelection->size());
	}
}


void CreationSelectionLayer::SetData(QString fileLocation)
{

}


bool CreationSelectionLayer::DataLoaded()
{
	return false;
}


/**
 * @brief Sets the camera used during drawing operations
 *
 * Sets the camera being used during drawing operations
 *
 * @param cam Pointer to the GLCamera object
 */
void CreationSelectionLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;

	/* Set the camera for the shaders */
	if (outlineShader)
		outlineShader->SetCamera(newCamera);
	if (fillShader)
		fillShader->SetCamera(newCamera);
	if (boundaryShader)
		boundaryShader->SetCamera(newCamera);

	/* Set the camera for the tools */
	if (clickTool)
		clickTool->SetCamera(newCamera);
	if (circleTool)
		circleTool->SetCamera(newCamera);
	if (rectangleTool)
		rectangleTool->SetCamera(newCamera);
	if (polygonTool)
		polygonTool->SetCamera(newCamera);
}


/**
 * @brief Returns the number of Elements that are currently selected
 *
 * Returns the number of Elements that are currently selected
 *
 * @return The number of Elements that are currently selected
 */
unsigned int CreationSelectionLayer::GetNumElementsSelected()
{
	if (selectedState)
		return selectedState->GetState()->size();
	return 0;
}


/**
 * @brief Sets the Terrain Layer that all actions and selections will be performed on
 *
 * Sets the Terrain Layer that all actions and selections will be performed on. This is
 * where the selection layer gets the vertex data (VBOId) for drawing.
 *
 * @param newLayer Pointer to the TerrainLayer object
 */
void CreationSelectionLayer::SetTerrainLayer(TerrainLayer *newLayer)
{
	terrainLayer = newLayer;

	connect(terrainLayer, SIGNAL(finishedLoadingToGPU()), this, SLOT(TerrainDataLoaded()));

	if (clickTool)
		clickTool->SetTerrainLayer(newLayer);
	if (circleTool)
		circleTool->SetTerrainLayer(newLayer);
	if (rectangleTool)
		rectangleTool->SetTerrainLayer(newLayer);
	if (polygonTool)
		polygonTool->SetTerrainLayer(newLayer);
}


/**
 * @brief Select the Selection Tool to be used for the next interaction
 *
 * Select the Selection Tool to be used for the next interaction. This
 * type of SelectionLayer can only select Elements, so SelectionType
 * is ignored.
 *
 * @param toolID The tool to be used for the next interaction
 */
void CreationSelectionLayer::UseTool(ToolType tool, SelectionType)
{
	activeToolType = tool;

	/* If the tool hasn't been created yet, create it now */
	if (activeToolType == ClickToolType)
	{
		if (!clickTool)
			CreateClickTool();
		activeTool = clickTool;
	}
	else if (activeToolType == CircleToolType)
	{
		if (!circleTool)
			CreateCircleTool();
		activeTool = circleTool;
	}
	else if (activeToolType == RectangleToolType)
	{
		if (!rectangleTool)
			CreateRectangleTool();
		activeTool = rectangleTool;
	}
	else if (activeToolType == PolygonToolType)
	{
		if (!polygonTool)
			CreatePolygonTool();
		activeTool = polygonTool;
	}

	if (activeTool)
		activeTool->UseTool();
}


void CreationSelectionLayer::MouseClick(QMouseEvent *event)
{
	if (activeTool)
		activeTool->MouseClick(event);
}


void CreationSelectionLayer::MouseMove(QMouseEvent *event)
{
	if (activeTool)
		activeTool->MouseMove(event);
}


void CreationSelectionLayer::MouseRelease(QMouseEvent *event)
{
	if (activeTool)
	{
		activeTool->MouseRelease(event);
	}
}


void CreationSelectionLayer::MouseWheel(QWheelEvent *event)
{
	if (activeTool)
		activeTool->MouseWheel(event);
}


void CreationSelectionLayer::KeyPress(QKeyEvent *event)
{
	if (activeTool)
		activeTool->KeyPress(event);
}


/**
 * @brief Tells all of the tools that the size of the OpenGL context has changed
 *
 * Tells all of the tools that the size of the OpenGL context has changed
 *
 * @param w The new OpenGL context width
 * @param h The new OpenGL context height
 */
void CreationSelectionLayer::WindowSizeChanged(float w, float h)
{
	if (circleTool)
		circleTool->SetViewportSize(w, h);
	if (rectangleTool)
		rectangleTool->SetViewportSize(w, h);
	if (polygonTool)
		polygonTool->SetViewportSize(w, h);
}


/**
 * @brief Undoes the previously performed selection or deselection
 *
 * Undoes the previously performed selection or deselection by reverting
 * to the previous state (the last one pushed onto the undo stack).
 *
 */
void CreationSelectionLayer::Undo()
{
	if (!undoStack.empty() && selectedState)
	{
		redoStack.push(selectedState);
		UseState(undoStack.top());
		undoStack.pop();
		emit RedoAvailable(true);
		if (undoStack.empty())
			emit UndoAvailable(false);
	}
}


bool CreationSelectionLayer::GetUndoAvailable()
{
	return !undoStack.empty();
}


/**
 * @brief Redoes the last undone selection or deselection
 *
 * Redoes the last undone selection or deselection by reverting
 * to the next state on the redo stack.
 *
 */
void CreationSelectionLayer::Redo()
{
	if (!redoStack.empty() && selectedState)
	{
		undoStack.push(selectedState);
		UseState(redoStack.top());
		redoStack.pop();
		emit UndoAvailable(true);
		if (redoStack.empty())
			emit RedoAvailable(false);
	}
}


bool CreationSelectionLayer::GetRedoAvailable()
{
	return !redoStack.empty();
}


/**
 * @brief If a valid boundary exists, a list of the boundary Nodes will be returned.
 *
 * If a valid boundary exists, a list of the boundary Nodes will be returned. The list will
 * be in counter-clockwise order around the subdomain.
 *
 * @return A list of boundary nodes in counter-clockwise order
 */
std::vector<unsigned int> CreationSelectionLayer::GetBoundaryNodes()
{
	return boundaryNodes;
}


ElementState* CreationSelectionLayer::GetCurrentSelection()
{
	return selectedState;
}


/**
 * @brief Initializes the Buffer Objects and Shaders objects necessary for drawing the
 * selection layer
 *
 * This function initializes the Buffer Objects in the OpenGL context as well as the
 * Shader objects necessary for drawing the selection layer. Default transparent grays
 * are used for color.
 *
 * This layer makes use of the vertex data that is already on the GPU from the TerrainLayer.
 * Therefore, we only need to create a new Vertex Array Object and Index Buffer Object.
 *
 */
void CreationSelectionLayer::InitializeGL()
{
	/* Only perform initialization if we have a VBO from a TerrainLayer */
	if (VBOId)
	{
		/* Create new shaders */
		outlineShader = new SolidShader();
		fillShader = new SolidShader();
		boundaryShader = new SolidShader();

		/* Set the shader properties */
		fillShader->SetColor(QColor(0.4*255, 0.4*255, 0.4*255, 0.4*255));
		outlineShader->SetColor(QColor(0.2*255, 0.2*255, 0.2*255, 0.2*255));
		boundaryShader->SetColor(QColor(0.0*255, 0.0*255, 0.0*255, 0.8*255));
		if (camera)
		{
			fillShader->SetCamera(camera);
			outlineShader->SetCamera(camera);
			boundaryShader->SetCamera(camera);
		}

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &IBOId);

		/* Bind the VBO and IBO to the VAO */
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
				DEBUG("Subdomain Creation Selection Layer Initialized");
				glLoaded = true;
			} else {
				DEBUG("Subdomain Creation Selection Layer Not Initialized");
				glLoaded = false;
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("Subdomain Creation Selection Layer OpenGL Error: " << errString);
			glLoaded = false;
		}
	} else {
		DEBUG("Subdomain Creation Selection Layer GL not initialized: TerrainLayer not set");
		glLoaded = false;
	}
}


void CreationSelectionLayer::CreateClickTool()
{
	if (!clickTool)
		clickTool = new ClickTool();
	clickTool->SetTerrainLayer(terrainLayer);
	clickTool->SetCamera(camera);
	connect(clickTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(clickTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(clickTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(clickTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
}


/**
 * @brief Creates the circle selection tool
 *
 * Creates the circle selection tool. We hook up the signal/slot mechanism here
 * so that this layer knows when the tool has finished finding all elements and
 * is ready to be queried.
 *
 */
void CreationSelectionLayer::CreateCircleTool()
{
	if (!circleTool)
		circleTool = new CircleTool();

	circleTool->SetTerrainLayer(terrainLayer);
	circleTool->SetCamera(camera);
	connect(circleTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(circleTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(circleTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(circleTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
	connect(circleTool, SIGNAL(CircleStatsSet(float,float,float)), this, SIGNAL(CircleToolStatsSet(float,float,float)));
}


void CreationSelectionLayer::CreateRectangleTool()
{
	if (!rectangleTool)
		rectangleTool = new RectangleTool();

	rectangleTool->SetTerrainLayer(terrainLayer);
	rectangleTool->SetCamera(camera);
	connect(rectangleTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(rectangleTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(rectangleTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(rectangleTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
	connect(rectangleTool, SIGNAL(RectangleStatsSet(float,float)), this, SIGNAL(RectangleToolStatsSet(float,float)));
}


void CreationSelectionLayer::CreatePolygonTool()
{
	if (!polygonTool)
		polygonTool = new PolygonTool();

	polygonTool->SetTerrainLayer(terrainLayer);
	polygonTool->SetCamera(camera);
	connect(polygonTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(polygonTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(polygonTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(polygonTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
}


/**
 * @brief Clears the undo stack
 *
 * Clears the undo stack by deleting all objects it contains
 *
 */
void CreationSelectionLayer::ClearUndoStack()
{
	while(!undoStack.empty())
	{
		ElementState *curr = undoStack.top();
		undoStack.pop();
		if (curr)
			delete curr;
	}
	emit UndoAvailable(false);
}


/**
 * @brief Clears the redo stack
 *
 * Clears the redo stack by deleting all objects it contains
 *
 */
void CreationSelectionLayer::ClearRedoStack()
{
	while(!redoStack.empty())
	{
		ElementState *curr = redoStack.top();
		redoStack.pop();
		if (curr)
			delete curr;
	}
	emit RedoAvailable(false);
}


/**
 * @brief Called after a new selection is made to set the current state to the newly created one
 *
 * Called after a new selection is made to set the current state to the newly created one. Takes
 * care of undo/redo stacks and boundary searching.
 *
 * @param newState The newly created state
 */
void CreationSelectionLayer::UseNewState(ElementState *newState)
{
	/* A new selection has been made, so redo is no longer available */
	ClearRedoStack();

	/* Push the old state onto the undo stack */
	undoStack.push(selectedState);
	emit UndoAvailable(true);

	UseState(newState);
}


/**
 * @brief Sets the currently visible state
 *
 * Sets the currently visible state. Performs boundary search on that state and loads all data
 * to the GPU.
 *
 * @param state The state to make visible
 */
void CreationSelectionLayer::UseState(ElementState *state)
{
	/* Set the current state to the new one */
	selectedState = state;

	/* Find the boundary */
	boundaryNodes = boundaryFinder->FindBoundaries(selectedState);

	/* Update the data on the GPU */
	LoadDataToGPU();
}


void CreationSelectionLayer::GetSelectionFromActiveTool()
{
	if (!selectedState)
		selectedState = new ElementState();
	if (activeTool)
	{
		/* Create the new state object */
		ElementState *newState = new ElementState(activeTool->GetSelectedElements());

		/* Get pointers to new list of selected elements and current list of selected elements */
		std::vector<Element*> *newList = newState->GetState();
		std::vector<Element*> *currList = selectedState->GetState();

		if (newList->size() > 0)
		{
			unsigned int oldNumSelected = currList->size();
			if (currList->size() > 0)
			{
				/* There are currently selected elements, so combine the lists */
				newList->reserve(newList->size() + currList->size());
				newList->insert(newList->end(), currList->begin(), currList->end());

			}

			/* Sort the new list */
			std::sort(newList->begin(), newList->end());

			/* Get rid of any duplicates in the newly created list */
			std::vector<Element*>::iterator it;
			it = std::unique(newList->begin(), newList->end());
			newList->resize(std::distance(newList->begin(), it));

			emit Message(QString::number(newList->size() - oldNumSelected).append(" new elements selected. <b>").append(QString::number(newList->size()).append("</b> total elements selected.")));

			UseNewState(newState);

		} else {
			/* No elements were selected, so just go ahead and delete the new list */
			delete newState;
		}
	}
}


/**
 * @brief Slot that contains all action that need to be performed after the
 * terrain data has been loaded to the GPU
 *
 * Slot that contains all actions that need to be performed after the
 * terrain data has been loaded to the GPU. Queries the terrain layer
 * for its vertex buffer object ID.
 *
 */
void CreationSelectionLayer::TerrainDataLoaded()
{
	VBOId = terrainLayer->GetVBOId();
}


void CreationSelectionLayer::GetSelectionFromTool()
{
	GetSelectionFromActiveTool();
	activeTool = 0;
}
