#ifndef PROJECT_H
#define PROJECT_H


#include <QObject>
#include <QProgressBar>
#include <QTreeWidget>
#include <QInputDialog>
#include <QHeaderView>
#include <QListWidget>

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

		void	DisplayDomain(int index);
		bool	IsInitialized();
		void	ResetNodalValues(QString subdomainName, unsigned int nodeNumber);
		void	SetNodalValues(QString subdomainName, unsigned int nodeNumber, QString x, QString y, QString z);
		void	SetOpenGLPanel(OpenGLPanel *newPanel);
		void	SetProgressBar(QProgressBar *newBar);
		void	SetProjectTree(QTreeWidget *newTree);
		void	SetEditSubdomainList(QListWidget *newList);

		QStringList	GetSubdomainNames();

	private:

		DisplayOptionsDialog*	displayOptions;
		QListWidget*		editSubdomainList;
		FullDomain*		fullDomain;
		FullDomainRunner*	fullDomainRunner;
		OpenGLPanel*		glPanel;
		QProgressBar*		progressBar;
		ProjectFile*		projectFile;
		bool			projectInitialized;
		QTreeWidget*		projectTree;
		std::vector<SubDomain*>	subDomains;
		Domain*			visibleDomain;


		FullDomain*	BuildFullDomain();
		SubDomain*	BuildSubdomain(QString subdomainName);
		void		CreateAllSubdomains();
		void		CreateProjectFile();
		Domain*		DetermineSelectedDomain(QTreeWidgetItem *item);
		Domain*		DetermineSelectedDomain(QListWidgetItem *item);
		Domain*		DetermineDomain(QString domainName);
		SubDomain*	DetermineSubdomain(Domain *domain);
		void		Initialize();
		void		OpenProjectFile(QString filePath);
		void		PopulateProjectTree();
		void		SetVisibleDomain(Domain *newDomain);

		void	MatchFullCamera(SubDomain *targetSub);	// Matches subdomain camera to full domain camera
		void	MatchSubCamera(SubDomain *targetSub);	// Matches full domain camera to subdomain camera
		void	MatchSubCamera(SubDomain *originSub, SubDomain *targetSub); // Matches originSub camera to targetSub camera

	public slots:

		void	CreateNewSubdomain();
		void	EditProjectSettings();
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

		void	SelectSingleSubdomainNode();

		void	ShowDisplayOptionsDialog();
		void	ToggleQuadtreeVisible();

		void	Undo();
		void	Redo();

	private slots:

		void	ProjectTreeItemChanged(QTreeWidgetItem *item, QTreeWidgetItem*);
		void	ProjectTreeItemChanged(QListWidgetItem *item, QListWidgetItem*);
		void	MatchColors(QAction *action);
		void	MatchCamera(QAction *action);
		void	ClearSelections();

	signals:

		void	editNode(unsigned int, QString, QString, QString);
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
