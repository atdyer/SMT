#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->paneBox->setCurrentIndex(0);
	ui->paneBox->setMinimumWidth(ui->projectTree->width()+6);

	currentProject = 0;
	displayOptionsDialog = 0;

	// Create GLPanel status bar and all labels
	glStatusBar = new QStatusBar();

	mouseXLabel = new QLabel("<b>X:</b> - ");
	mouseYLabel = new QLabel("<b>Y:</b> - ");

	numNodesLabel = new QLabel("<b>Nodes:</b> -       ");
	numElementsLabel = new QLabel("<b>Elements:</b> -       ");
	numTSLabel = new QLabel("<b>Timesteps:</b> -");

	// Stylize GL Panel status bar stuff
	glStatusBar->setSizeGripEnabled(false);
	glStatusBar->setStyleSheet("QStatusBar::item {border: 0px solid black };");

	// Add it all to the UI
	ui->GLPanelStatusLayout->insertWidget(0, glStatusBar);
	glStatusBar->insertPermanentWidget(0, mouseXLabel);
	glStatusBar->insertPermanentWidget(1, mouseYLabel);
	glStatusBar->insertWidget(0, numNodesLabel);
	glStatusBar->insertWidget(1, numElementsLabel);
	glStatusBar->insertWidget(2, numTSLabel);

	// Hide the progress bar and bottom section
	ui->progressBar->hide();

}

MainWindow::~MainWindow()
{
	delete ui;
}
