#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QToolBar>
#include <QSettings>

#include "Project/Project.h"
#include "Dialogs/DisplayOptionsDialog.h"
#include "Dialogs/CreateProjectDialog.h"

namespace Ui {
	class MainWindow;
}

enum { MaxRecentProjects = 5 };

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	protected:

		void	keyPressEvent(QKeyEvent *event);
		
	private:
		// The Main Window
		Ui::MainWindow *ui;

		// The status bar specific to the GLPanel
		QStatusBar	*glStatusBar;
		QLabel		*mouseXLabel, *mouseYLabel, *numNodesLabel, *numElementsLabel, *numTSLabel;

		// The Project Object
		Project*	currentProject;

		// Dialogs
		DisplayOptionsDialog*	displayOptionsDialog;

		// Recent Project Actions
		QAction		*recentProjects[MaxRecentProjects];
		QAction		*recentProjectsSeparator;

		// Program settings
		QSettings	settings;

		void	CheckForMemoryLeaks();
		void	CreateProject(bool newProjectFile);
		void	ConnectProject(Project *proj);
		void	LoadRecentProjects();
		void	UpdateRecentProjects();

	public slots:

		void	showMaxSelectedZ(float newZ);
		void	showMinSelectedZ(float newZ);
		void	showMouseX(float newX);
		void	showMouseY(float newY);
		void	showNumNodes(int numNodes);
		void	showNumElements(int numElements);
		void	showNumTS(int numTS);
		void	showNumSelectedNodes(int numNodes);
		void	showNumSelectedElements(int numElements);

		// Circle Tool Slots
		void	showCircleStats(float x, float y, float rad);

	private slots:

		/* Project buttons */
		void	createProject();
		void	openProject();
		void	openRecentProject();
		void	closeProject();

		/* Left side pane slots */
		void	addSubdomainToList(QString s);
		void	editNode(unsigned int nodeNum, QString x, QString y, QString z);

		void	showProjectExplorerPane();
		void	showCreateSubdomainPane();
		void	showEditSubdomainPane();
		void	showAdcircPane();
		void	showAnalyzeResultsPane();

		/* Node editing slots */
		void	setNodeValues();
		void	resetCurrentNodeValues();
		void	resetAllNodalValues();
		void	saveAllSubdomainChanges();

		/* Extra */
		void	printText(QString text);

	signals:

		void	quit();
};

#endif // MAINWINDOW_H
