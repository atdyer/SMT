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

	if (activeTool)
		activeTool->Draw();
}


void SubDomainSelectionLayer::LoadDataToGPU()
{
	if (!glLoaded)
		InitializeGL();

	if (glLoaded)
	{
		if (VBOId && currentNode)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBOId);
			GLfloat glPoint[2] = {currentNode->normX, currentNode->normY};
			glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(GLfloat), &glPoint[0]);
		}
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
	GLfloat glPoint[4] = {0.0, 0.0, 0.0, 1.0};
	glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, &glPoint[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

	const size_t IndexBufferSize = 1*sizeof(GLuint);
	GLuint glIndex = 0;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, &glIndex, GL_STATIC_DRAW);

	glBindVertexArray(0);
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
