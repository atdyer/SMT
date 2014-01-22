#ifndef SUBDOMAINSELECTIONLAYER_H
#define SUBDOMAINSELECTIONLAYER_H

#include "Layers/SelectionLayer.h"

#include "Project/Files/Fort14.h"

#include "SubdomainTools/SelectionTool.h"
#include "SubdomainTools/ClickTool.h"

class SubDomainSelectionLayer : public SelectionLayer
{
		Q_OBJECT
	public:
		SubDomainSelectionLayer(Fort14 *fort14, QObject *parent=0);
		~SubDomainSelectionLayer();

		virtual void	Draw();
		virtual void	LoadDataToGPU();
		virtual void	SetData(QString fileLocation);
		virtual bool	DataLoaded();

		virtual unsigned int	GetNumNodesSelected();
		virtual unsigned int	GetNumElementsSelected();

		virtual void	SetCamera(GLCamera *newCamera);
		virtual void	UseTool(ToolType tool, SelectionType selection, SelectionMode mode);

		virtual void	MouseClick(QMouseEvent *event);
		virtual void	MouseMove(QMouseEvent *event);
		virtual void	MouseRelease(QMouseEvent *event);
		virtual void	MouseWheel(QWheelEvent *event);
		virtual void	KeyPress(QKeyEvent *event);
		virtual void	WindowSizeChanged(float w, float h);

		virtual void	Undo();
		virtual bool	GetUndoAvailable();
		virtual void	Redo();
		virtual bool	GetRedoAvailable();

		Node*	GetSelectedNode();

	private:

		Fort14	*fort14;

		/* Selection Tools */
		ToolType	activeToolType;
		SelectionMode	currentSelectionMode;
		SelectionTool*	activeTool;
		ClickTool*	clickTool;

		/* Shaders */
		SolidShader*	pointShader;

		/* Current Selection */
		Node*	currentNode;

		/* Initialization/Deallocation Functions */
		void	AttachToFort14();
		void	InitializeGL();

		/* Tool Initialization Functions */
		void	CreateClickTool();

		/* State Functions */
		void	GetSelectionFromActiveTool();

	private slots:

		void	UseVBOId(GLuint newVBO);
		void	GetSelectionFromTool();

	signals:

		void	editNode(unsigned int, QString, QString, QString);
};

#endif // SUBDOMAINSELECTIONLAYER_H
