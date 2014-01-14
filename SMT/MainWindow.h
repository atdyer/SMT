#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Project/Project.h"
#include "Dialogs/DisplayOptionsDialog.h"
#include "Dialogs/CreateProjectDialog.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
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
};

#endif // MAINWINDOW_H
