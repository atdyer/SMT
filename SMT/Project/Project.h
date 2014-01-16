#ifndef PROJECT_H
#define PROJECT_H


#include <QObject>
#include <QProgressBar>
#include <QTreeWidget>
#include <QInputDialog>
#include <QHeaderView>

#include <vector>

#include "Adcirc/FullDomainRunner.h"
#include "Adcirc/SubdomainCreator.h"

#include "Dialogs/CreateProjectDialog.h"
#include "Dialogs/CreateSubdomainDialog.h"
#include "Dialogs/DisplayOptionsDialog.h"

#include "Project/Domains/FullDomain.h"
#include "Project/Domains/SubDomain.h"
#include "Project/Files/ProjectFile.h"

#include "OpenGL/OpenGLPanel.h"



class Project : public QObject
{
		Q_OBJECT
	public:
		explicit Project(QObject *parent=0);
		Project(QString projectFile, QObject *parent=0);
		~Project();

		void	SetOpenGLPanel(OpenGLPanel *newPanel);
		void	SetProgressBar(QProgressBar *newBar);
		void	SetProjectTree(QTreeWidget *newTree);

		QStringList	GetSubdomainNames();

	private:

		DisplayOptionsDialog*	displayOptions;
		FullDomain*		fullDomain;
		FullDomainRunner*	fullDomainRunner;
		OpenGLPanel*		glPanel;
		QProgressBar*		progressBar;
		ProjectFile*	projectFile;
		QTreeWidget*		projectTree;
		std::vector<SubDomain*>	subDomains;
		Domain*		visibleDomain;


		FullDomain*	BuildFullDomain();
		SubDomain*	BuildSubdomain(QString subdomainName);
		void		CreateAllSubdomains();
		void		CreateProjectFile();
		Domain*	DetermineSelectedDomain(QTreeWidgetItem *item);
		void		Initialize();
		void		OpenProjectFile(QString filePath);
		void		PopulateProjectTree();
		void		SetVisibleDomain(Domain *newDomain);

	public slots:

		void	CreateNewSubdomain();
		void	EditProjectSettings();
		void	Redo();
		void	RunFullDomain();
		void	RunSubdomain(QString subdomain);
		void	SaveProject();

		void	SelectFullDomainCircleElements();
		void	SelectFullDomainClickElements();
		void	SelectFullDomainPolygonElements();
		void	SelectFullDomainRectangleElements();
		void	DeselectFullDomainCircleElements();
		void	DeselectFullDomainClickElements();
		void	DeselectFullDomainPolygonElements();
		void	DeselectFullDomainRectangleElements();

		void	ShowDisplayOptionsDialog();

		void	ToggleQuadtreeVisible();

		void	Undo();

	private slots:

		void	ProjectTreeItemChanged(QTreeWidgetItem *item, QTreeWidgetItem*);

	signals:

		void	maxSelectedZ(float);
		void	minSelectedZ(float);
		void	mouseX(float);
		void	mouseY(float);
		void	numElements(int);
		void	numElementsSelected(int);
		void	numNodes(int);
		void	numNodesSelected(int);
		void	redoAvailable(bool);
		void	showProjectView();
		void	subdomainCreated(QString);
		void	undoAvailable(bool);


};

#endif // PROJECT_H
