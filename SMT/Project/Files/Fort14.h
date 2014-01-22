#ifndef FORT14_H
#define FORT14_H

#include <QObject>
#include <QProgressBar>
#include <QThread>

#include "Project/Files/ProjectFile.h"
#include "Project/Files/Workers/Fort14Reader.h"

#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/GradientShader.h"
#include "OpenGL/Shaders/SolidShader.h"

#include "Quadtree/Quadtree.h"

class Fort14 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14(QObject *parent=0);
		Fort14(ProjectFile *projectFile, QObject *parent=0);
		Fort14(QString domainName, ProjectFile *projectFile, QObject *parent=0);
		~Fort14();

		void	Draw();

		Node*			FindNode(float xGL, float yGL);
		Element*		FindElement(float xGL, float yGL);
		std::vector<Element*>	FindElementsInCircle(float x, float y, float radius);
		std::vector<Element*>	FindElementsInRectangle(float l, float r, float b, float t);
		std::vector<Element*>	FindElementsInPolygon(std::vector<Point> polyLine);

		ShaderType		GetBoundaryShaderType();
		std::vector<Element>*	GetElements();
		QString			GetFilePath();
		ShaderType		GetFillShaderType();
		QGradientStops		GetGradientBoundaryColors();
		QGradientStops		GetGradientFillColors();
		QGradientStops		GetGradientOutlineColors();
		float			GetMaxX();
		float			GetMaxY();
		float			GetMaxZ();
		float			GetMinX();
		float			GetMinY();
		float			GetMinZ();
		int			GetNumElements();
		int			GetNumNodes();
		ShaderType		GetOutlineShaderType();
		float			GetUnprojectedX(float x);
		float			GetUnprojectedY(float y);

		std::vector<Element*>	GetSelectedElements();
		QColor			GetSolidBoundaryColor();
		QColor			GetSolidFillColor();
		QColor			GetSolidOutlineColor();

		void			SetCamera(GLCamera *camera);
		void			SetGradientBoundaryColors(QGradientStops newStops);
		void			SetGradientFillColors(QGradientStops newStops);
		void			SetGradientOutlineColors(QGradientStops newStops);
		void			SetProgressBar(QProgressBar *newBar);
		void			SetSolidBoundaryColor(QColor newColor);
		void			SetSolidFillColor(QColor newColor);
		void			SetSolidOutlineColor(QColor newColor);

		void			ToggleQuadtreeVisible();

	private:

		QString				domainName;
		std::vector<Element>		elements;
		std::vector<std::vector<unsigned int> >	elevationBoundaries;
		std::vector<std::vector<unsigned int> >	flowBoundaries;
		float				max;
		float				maxX;
		float				maxY;
		float				maxZ;
		float				midX;
		float				midY;
		float				minX;
		float				minY;
		float				minZ;
		std::vector<Node>		nodes;
		unsigned int			numElements;
		unsigned int			numNodes;
		QProgressBar*			progressBar;
		ProjectFile*		projectFile;
		Quadtree*			quadtree;
		bool				quadtreeVisible;
		bool				readingLock;

		/* OpenGL */
		bool		glLoaded;
		GLCamera*	camera;
		GLShader*	outlineShader;
		GLShader*	fillShader;
		GLShader*	boundaryShader;
		SolidShader*	solidOutline;
		SolidShader*	solidFill;
		SolidShader*	solidBoundary;
		GradientShader*	gradientOutline;
		GradientShader* gradientFill;
		GradientShader*	gradientBoundary;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;

		void	CreateDefaultShaders();
		void	LoadGL();
		void	PopulateQuadtree();
		void	ReadFile();
		void	UnlockFile();

	signals:

		void	DataLoadedToGPU(GLuint);
		void	NumElementsSet(int);
		void	NumNodesSet(int);

	public slots:

		void	SelectCircle(int x, int y, int radius);
		void	SelectPoint(int x, int y);
		void	SelectPolygon(std::vector<Point> polyLine);
		void	SelectRectangle(int l, int r, int b, int t);

	protected slots:

		void	FinishedReading();
		void	LockFile();
		void	Progress(int percent);
		void	SetDomainBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);



};

#endif // FORT14_H
