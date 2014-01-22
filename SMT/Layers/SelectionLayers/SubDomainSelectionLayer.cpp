#include "SubDomainSelectionLayer.h"

SubDomainSelectionLayer::SubDomainSelectionLayer(Fort14 *fort14, QObject *parent) :
	SelectionLayer(parent),
	fort14(fort14),
	activeToolType(ClickToolType),
	currentSelectionMode(Select),
	activeTool(0),
	clickTool(0),
	pointShader(0),
	currentNode(0)
{
	AttachToFort14();
	CreateClickTool();
}


SubDomainSelectionLayer::~SubDomainSelectionLayer()
{
	if (clickTool)
		delete clickTool;

	if (pointShader)
		delete pointShader;
}


void SubDomainSelectionLayer::Draw()
{
	if (glLoaded && currentNode)
	{
		glBindVertexArray(VAOId);

//		GLuint currentData = 0;
//		GLfloat coords[3] = {0.0, 0.0, 0.0};
//		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint), &currentData);
//		std::cout << "Trying to draw node " << currentData << std::endl;
//		glGetBufferSubData(GL_ARRAY_BUFFER, 4*currentData*sizeof(GLfloat), 3*sizeof(GLfloat), coords);
//		std::cout << "Trying to draw coordinates: " << coords[0] << ", " << coords[1] << ", " << coords[2] << std::endl;

		if (pointShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(5.0);
			if (pointShader->Use())
				glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*)0);

			GLenum errorCheck = glGetError();
			if (errorCheck == GL_NO_ERROR)
			{

			} else {
				const GLubyte *errString = gluErrorString(errorCheck);
				DEBUG(errString);
			}
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}

	if (activeTool)
		activeTool->Draw();
}


void SubDomainSelectionLayer::LoadDataToGPU()
{
	if (!glLoaded)
		InitializeGL();

	if (glLoaded && currentNode)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat);
		const size_t IndexBufferSize = 1*sizeof(GLuint);

		if (IndexBufferSize && VAOId && IBOId)
		{
			glBindVertexArray(VAOId);
			glBindBuffer(GL_ARRAY_BUFFER, VBOId);
			glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
			GLfloat* glVertexData = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (glVertexData)
			{
				glVertexData[0] = (GLfloat)currentNode->normX;
				glVertexData[1] = (GLfloat)currentNode->normY;
				glVertexData[2] = (GLfloat)currentNode->normZ;
				glVertexData[3] = (GLfloat)1.0;
			}

			if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
			{
				glLoaded = false;
				DEBUG("Error unmapping");
				return;
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
			GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (glElementData)
			{
				glElementData[0] = (GLuint)currentNode->nodeNumber-1;
			} else {
				glLoaded = false;
				std::cout << "Error mapping" << std::endl;
			}

			GLenum errorCheck = glGetError();
			if (errorCheck == GL_NO_ERROR)
			{
				if (VAOId && VBOId && IBOId)
				{
					glLoaded = true;
				} else {
					DEBUG("Error sending point data to GPU in Subdomain Selection Layer");
					glLoaded = false;
				}
			} else {
				const GLubyte *errString = gluErrorString(errorCheck);
				DEBUG("Error sending point data to GPU in Subdomain Selection Layer: " << errString);
				glLoaded = false;
				return;
			}

			if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
			{
				glLoaded = false;
				DEBUG("Error unmapping");
				return;
			}
		}

		emit Refreshed();
	}
}


void SubDomainSelectionLayer::SetData(QString fileLocation)
{

}


bool SubDomainSelectionLayer::DataLoaded()
{
	return false;
}


unsigned int SubDomainSelectionLayer::GetNumNodesSelected()
{
	if (currentNode)
		return 1;
	return 0;
}


unsigned int SubDomainSelectionLayer::GetNumElementsSelected()
{
	return 0;
}


void SubDomainSelectionLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;

	/* Set the camera for the shaders */
	if (pointShader)
		pointShader->SetCamera(newCamera);

	/* Set the camera for the tools */
	if (clickTool)
		clickTool->SetCamera(newCamera);
}


void SubDomainSelectionLayer::UseTool(ToolType tool, SelectionType selection, SelectionMode mode)
{
	activeToolType = tool;
	currentSelectionMode = mode;

	/* If the tool hasn't been created yet, create it now */
	if (activeToolType == ClickToolType)
	{
		if (!clickTool)
			CreateClickTool();
		activeTool = clickTool;
	}

	if (activeTool)
		activeTool->UseTool();
}


void SubDomainSelectionLayer::MouseClick(QMouseEvent *event)
{
	if (activeTool)
		activeTool->MouseClick(event);
}


void SubDomainSelectionLayer::MouseMove(QMouseEvent *event)
{
	if (activeTool)
		activeTool->MouseMove(event);
}


void SubDomainSelectionLayer::MouseRelease(QMouseEvent *event)
{
	if (activeTool)
	{
		activeTool->MouseRelease(event);
	}
}


void SubDomainSelectionLayer::MouseWheel(QWheelEvent *event)
{
	if (activeTool)
		activeTool->MouseWheel(event);
}


void SubDomainSelectionLayer::KeyPress(QKeyEvent *event)
{
	if (activeTool)
		activeTool->KeyPress(event);
}


void SubDomainSelectionLayer::WindowSizeChanged(float w, float h)
{
	if (clickTool)
		clickTool->SetViewportSize(w, h);
}


void SubDomainSelectionLayer::Undo()
{

}


bool SubDomainSelectionLayer::GetUndoAvailable()
{
	return false;
}


void SubDomainSelectionLayer::Redo()
{

}


bool SubDomainSelectionLayer::GetRedoAvailable()
{
	return false;
}


Node* SubDomainSelectionLayer::GetSelectedNode()
{
	return currentNode;
}


void SubDomainSelectionLayer::AttachToFort14()
{
	connect(fort14, SIGNAL(DataLoadedToGPU(GLuint)), this, SLOT(UseVBOId(GLuint)));
}


void SubDomainSelectionLayer::InitializeGL()
{
//	if (VBOId)
//	{
	pointShader = new SolidShader();
	pointShader->SetColor(QColor(1.0, 0.0, 0.0, 1.0));

	if (camera)
	{
		pointShader->SetCamera(camera);
	}

	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
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
			DEBUG("Subdomain Edit Selection Layer Initialized");
			glLoaded = true;
		} else {
			DEBUG("Subdomain Edit Selection Layer Not Initialized");
			glLoaded = false;
		}
	} else {
		const GLubyte *errString = gluErrorString(errorCheck);
		DEBUG("Subdomain Edit Selection Layer OpenGL Error: " << errString);
		glLoaded = false;
	}
//	} else {
//		DEBUG("Subdomain Edit Selection Layer GL not initialized: fort.14 not set");
//		glLoaded = false;
//	}
}


void SubDomainSelectionLayer::CreateClickTool()
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


void SubDomainSelectionLayer::GetSelectionFromActiveTool()
{
	if (activeTool)
	{
		std::vector<Node*> newList = activeTool->GetSelectedNodes();

		if (newList.size() > 0)
			currentNode = newList[0];

		LoadDataToGPU();
	}
}


void SubDomainSelectionLayer::UseVBOId(GLuint newVBO)
{
//	VBOId = newVBO;
}


void SubDomainSelectionLayer::GetSelectionFromTool()
{
	GetSelectionFromActiveTool();
	activeTool = 0;
}
