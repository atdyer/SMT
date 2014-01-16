#include "FullDomainSelectionLayer.h"

FullDomainSelectionLayer::FullDomainSelectionLayer(Fort14 *fort14, QObject *parent) :
	SelectionLayer(parent),
	fort14(fort14),
	activeToolType(ClickToolType),
	activeTool(0),
	clickTool(0),
	circleTool(0),
	rectangleTool(0),
	polygonTool(0),
	boundaryFinder(0),
	selectedState(0),
	outerBoundaryNodes(),
	innerBoundaryNodes(),
	undoStack(),
	redoStack(),
	outlineShader(0),
	fillShader(0),
	innerBoundaryShader(0),
	outerBoundaryShader(0)
{
	AttachToFort14();
	CreateClickTool();
	CreateCircleTool();
	CreateRectangleTool();
	CreatePolygonTool();
}


FullDomainSelectionLayer::~FullDomainSelectionLayer()
{
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

	if (selectedState)
		delete selectedState;

	ClearUndoStack();
	ClearRedoStack();

	if (outlineShader)
		delete outlineShader;
	if (fillShader)
		delete fillShader;
	if (innerBoundaryShader)
		delete innerBoundaryShader;
	if (outerBoundaryShader)
		delete outerBoundaryShader;
}


void FullDomainSelectionLayer::Draw()
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

		if (outerBoundaryShader && outerBoundaryNodes.size())
		{
			/* Draw points to smooth out the ends of the lines */
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(5.0);
			if (outerBoundaryShader->Use())
				glDrawElements(GL_POINTS, outerBoundaryNodes.size(), GL_UNSIGNED_INT,
					       (GLvoid*)(0 + sizeof(GLuint)*numElements*3));

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(5.0);
			if (outerBoundaryShader->Use())
				glDrawElements(GL_LINE_LOOP, outerBoundaryNodes.size(), GL_UNSIGNED_INT,
					       (GLvoid*)(0 + sizeof(GLuint)*numElements*3));
			glLineWidth(1.0);
		}

		if (innerBoundaryShader && innerBoundaryNodes.size())
		{
			/* Draw points to smooth out the ends of the lines */
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(5.0);
			if (innerBoundaryShader->Use())
				glDrawElements(GL_POINTS, innerBoundaryNodes.size(), GL_UNSIGNED_INT,
					       (GLvoid*)(0 + sizeof(GLuint)*(numElements*3 + outerBoundaryNodes.size())));

//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			glLineWidth(3.0);
//			if (innerBoundaryShader->Use())
//				glDrawElements(GL_LINE_LOOP, innerBoundaryNodes.size(), GL_UNSIGNED_INT,
//					       (GLvoid*)(0 + sizeof(GLuint)*(numElements*3 + outerBoundaryNodes.size())));
//			glLineWidth(1.0);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (activeTool)
		activeTool->Draw();
}


void FullDomainSelectionLayer::LoadDataToGPU()
{
	/* Make sure we've got all of the necessary Buffer Objects created */
	if (!glLoaded)
		InitializeGL();

	/* Make sure initialization succeeded */
	if (glLoaded && selectedState)
	{
		/* Load the connectivity data (elements) to the GPU, getting rid of any data that's already there */
		std::vector<Element*> *currSelection = selectedState->GetState();
		const size_t IndexBufferSize = 3*sizeof(GLuint)*currSelection->size() + sizeof(GLuint)*(outerBoundaryNodes.size() + innerBoundaryNodes.size());
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
				for (std::vector<Element*>::iterator it=currSelection->begin(); it != currSelection->end(); ++it, ++i)
				{
					currElement = *it;
					glElementData[3*i+0] = (GLuint)currElement->n1->nodeNumber-1;
					glElementData[3*i+1] = (GLuint)currElement->n2->nodeNumber-1;
					glElementData[3*i+2] = (GLuint)currElement->n3->nodeNumber-1;
				}
				for (i=0; i<outerBoundaryNodes.size(); ++i)
				{
					glElementData[3*currSelectionSize+i] = outerBoundaryNodes[i]-1;
				}
				for (i=0; i<innerBoundaryNodes.size(); ++i)
				{
					glElementData[3*currSelectionSize+outerBoundaryNodes.size()+i] = innerBoundaryNodes[i]-1;
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


void FullDomainSelectionLayer::SetData(QString fileLocation)
{

}


bool FullDomainSelectionLayer::DataLoaded()
{
	return false;
}


unsigned int FullDomainSelectionLayer::GetNumNodesSelected()
{
	return 0;
}


unsigned int FullDomainSelectionLayer::GetNumElementsSelected()
{
	if (selectedState)
		return selectedState->GetState()->size();
	return 0;
}


void FullDomainSelectionLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;

	/* Set the camera for the shaders */
	if (outlineShader)
		outlineShader->SetCamera(newCamera);
	if (fillShader)
		fillShader->SetCamera(newCamera);
	if (outerBoundaryShader)
		outerBoundaryShader->SetCamera(newCamera);

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


void FullDomainSelectionLayer::UseTool(ToolType tool, SelectionType selection)
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


void FullDomainSelectionLayer::MouseClick(QMouseEvent *event)
{
	if (activeTool)
		activeTool->MouseClick(event);
}


void FullDomainSelectionLayer::MouseMove(QMouseEvent *event)
{
	if (activeTool)
		activeTool->MouseMove(event);
}


void FullDomainSelectionLayer::MouseRelease(QMouseEvent *event)
{
	if (activeTool)
	{
		activeTool->MouseRelease(event);
	}
}


void FullDomainSelectionLayer::MouseWheel(QWheelEvent *event)
{
	if (activeTool)
		activeTool->MouseWheel(event);
}


void FullDomainSelectionLayer::KeyPress(QKeyEvent *event)
{
	if (activeTool)
		activeTool->KeyPress(event);
}


void FullDomainSelectionLayer::WindowSizeChanged(float w, float h)
{
	if (circleTool)
		circleTool->SetViewportSize(w, h);
	if (rectangleTool)
		rectangleTool->SetViewportSize(w, h);
	if (polygonTool)
		polygonTool->SetViewportSize(w, h);
}


void FullDomainSelectionLayer::Undo()
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


bool FullDomainSelectionLayer::GetUndoAvailable()
{
	return !undoStack.empty();
}


void FullDomainSelectionLayer::Redo()
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


bool FullDomainSelectionLayer::GetRedoAvailable()
{
	return !redoStack.empty();
}


std::vector<unsigned int> FullDomainSelectionLayer::GetInnerBoundaryNodes()
{
	return innerBoundaryNodes;
}


std::vector<unsigned int> FullDomainSelectionLayer::GetOuterBoundaryNodes()
{
	return outerBoundaryNodes;
}


std::vector<Element*> FullDomainSelectionLayer::GetSelectedElements()
{
	if (selectedState)
		return *selectedState->GetState();
	std::vector<Element*> noselection;
	return noselection;
}


void FullDomainSelectionLayer::AttachToFort14()
{
	connect(fort14, SIGNAL(DataLoadedToGPU(GLuint)), this, SLOT(UseVBOId(GLuint)));
}


void FullDomainSelectionLayer::ClearUndoStack()
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


void FullDomainSelectionLayer::ClearRedoStack()
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


void FullDomainSelectionLayer::InitializeGL()
{
	/* Only perform initialization if we have a VBO from a TerrainLayer */
	if (VBOId)
	{
		/* Create new shaders */
		outlineShader = new SolidShader();
		fillShader = new SolidShader();
		outerBoundaryShader = new SolidShader();
		innerBoundaryShader = new SolidShader();

		/* Set the shader properties */
		fillShader->SetColor(QColor(0.4*255, 0.4*255, 0.4*255, 0.4*255));
		outlineShader->SetColor(QColor(0.2*255, 0.2*255, 0.2*255, 0.2*255));
		outerBoundaryShader->SetColor(QColor(0.0*255, 0.0*255, 0.0*255, 1.0*255));
		innerBoundaryShader->SetColor(QColor(0.0*255, 0.0*255, 0.0*255, 1.0*255));
		if (camera)
		{
			fillShader->SetCamera(camera);
			outlineShader->SetCamera(camera);
			outerBoundaryShader->SetCamera(camera);
			innerBoundaryShader->SetCamera(camera);
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


void FullDomainSelectionLayer::CreateClickTool()
{
	if (!clickTool)
		clickTool = new ClickTool();
	clickTool->SetFort14(fort14);
	clickTool->SetCamera(camera);
	connect(clickTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(clickTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(clickTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(clickTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
}


void FullDomainSelectionLayer::CreateCircleTool()
{
	if (!circleTool)
		circleTool = new CircleTool();

	circleTool->SetFort14(fort14);
	circleTool->SetCamera(camera);
	connect(circleTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(circleTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(circleTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(circleTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
	connect(circleTool, SIGNAL(CircleStatsSet(float,float,float)), this, SIGNAL(CircleToolStatsSet(float,float,float)));
}


void FullDomainSelectionLayer::CreateRectangleTool()
{
	if (!rectangleTool)
		rectangleTool = new RectangleTool();

	rectangleTool->SetFort14(fort14);
	rectangleTool->SetCamera(camera);
	connect(rectangleTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(rectangleTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(rectangleTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(rectangleTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
	connect(rectangleTool, SIGNAL(RectangleStatsSet(float,float)), this, SIGNAL(RectangleToolStatsSet(float,float)));
}


void FullDomainSelectionLayer::CreatePolygonTool()
{
	if (!polygonTool)
		polygonTool = new PolygonTool();

	polygonTool->SetFort14(fort14);
	polygonTool->SetCamera(camera);
	connect(polygonTool, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(polygonTool, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(polygonTool, SIGNAL(ToolFinishedDrawing()), this, SLOT(GetSelectionFromTool()));
	connect(polygonTool, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));
}


void FullDomainSelectionLayer::UseNewState(ElementState *newState)
{
	/* A new selection has been made, so redo is no longer available */
	ClearRedoStack();

	/* Push the old state onto the undo stack */
	undoStack.push(selectedState);
	emit UndoAvailable(true);

	UseState(newState);
}


void FullDomainSelectionLayer::UseState(ElementState *state)
{
	/* Set the current state to the new one */
	selectedState = state;

	Boundaries *currentBoundaries = selectedState->GetBoundaries();
	outerBoundaryNodes = currentBoundaries->outerBoundaryNodes;
	innerBoundaryNodes = currentBoundaries->innerBoundaryNodes;

	emit NumNodesSelected(currentBoundaries->numNodes);
	emit NumElementsSelected(currentBoundaries->numElements);
	emit MaxSelectedZ(currentBoundaries->maxZ);
	emit MinSelectedZ(currentBoundaries->minZ);

	/* Update the data on the GPU */
	LoadDataToGPU();
}


void FullDomainSelectionLayer::GetSelectionFromActiveTool()
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


void FullDomainSelectionLayer::UseVBOId(GLuint newVBO)
{
	VBOId = newVBO;
}


void FullDomainSelectionLayer::GetSelectionFromTool()
{
	GetSelectionFromActiveTool();
	activeTool = 0;
}

