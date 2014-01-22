#ifndef CLICKTOOL_H
#define CLICKTOOL_H

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"
#include "SubdomainTools/SelectionTool.h"

class ClickTool : public SelectionTool
{
		Q_OBJECT
	public:
		ClickTool();
		~ClickTool();

		void	Draw();
		void	SetCamera(GLCamera* cam);
		void	SetFort14(Fort14 *newFort14);
		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetViewportSize(float w, float h);

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		void	UseTool();

		std::vector<Node*>	GetSelectedNodes();
		std::vector<Element*>	GetSelectedElements();

	private:

		Fort14*	fort14;
		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
		GLCamera*	camera;		/**< The GLCamera that is used to draw the TerrainLayer */

		/* Helper Functions */
		void	InitializeGL();
		void	UpdateCoordinates();
		void	UpdateGL();
		void	FindNode();
		void	FindElement();

		/* OpenGL Stuff */
		bool		glLoaded;	/**< Flag that shows if the VAO/VBO/IBO have been created */
		bool		visible;	/**< Flag that shows if the tool is currently visible */
		GLuint		VAOId;		/**< The vertex array object ID */
		GLuint		VBOId;		/**< The vertex buffer object ID */
		GLuint		IBOId;		/**< The index buffer object ID */
		SolidShader*	pointShader;	/**< The shader used to draw the selection tool */
		GLfloat		glPoint[4];	/**< The point data to be sent to the GPU */
		GLuint		glIndex;	/**< The index of the point data on the GPU */

		/* Mouse State */
		bool	mousePressed;	/**< Flag that shows if the left mouse button is pressed */
		bool	mouseMoved;	/**< Flag that show if the mouse moved during a click */
		int	oldX;		/**< The last x-coordinate of the mouse (in pixels) */
		int	oldY;		/**< The last y-coordinate of the mouse (in pixels) */
		int	xPixel;		/**< The x-coordinate of the mouse (in pixels) */
		int	yPixel;		/**< The y-coordinate of the mouse (in pixels) */
		float	xGL;		/**< The x-coordinate of the mouse (in GL space) */
		float	yGL;		/**< The y-coordinate of the mouse (in GL space) */

		/* Selected Nodes/Elements */
		std::vector<Node*>	selectedNodes;		/**< The list of currently selected Nodes */
		std::vector<Element*>	selectedElements;	/**< The list of currently selected Elements */
};

#endif // CLICKTOOL_H
