#include "MainWindow.h"
#include "ui_MainWindow.h"

// analyze branch

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

	// Connect the buttons
	connect(ui->newProjectButton, SIGNAL(clicked()), this, SLOT(createProject()));
	connect(ui->openProjectButton, SIGNAL(clicked()), this, SLOT(openProject()));

	// Set up the edit subdomains items
//	connect(ui->editSubdomainList, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(showSubdomain(QListWidgetItem*)));
	ui->editXLoc->setValidator(new QDoubleValidator());
	ui->editYLoc->setValidator(new QDoubleValidator());
	ui->editZLoc->setValidator(new QDoubleValidator());

	// Connect editing subdomain buttons
	connect(ui->editXLoc, SIGNAL(returnPressed()), this, SLOT(setNodeValues()));
	connect(ui->editYLoc, SIGNAL(returnPressed()), this, SLOT(setNodeValues()));
	connect(ui->editZLoc, SIGNAL(returnPressed()), this, SLOT(setNodeValues()));
	connect(ui->applyEditButton, SIGNAL(clicked()), this, SLOT(setNodeValues()));
	connect(ui->resetEditButton, SIGNAL(clicked()), this, SLOT(resetCurrentNodeValues()));
	connect(ui->resetSubdomainChangesButton, SIGNAL(clicked()), this, SLOT(resetAllNodalValues()));
	connect(ui->saveSubdomainChangesButton, SIGNAL(clicked()), this, SLOT(saveAllSubdomainChanges()));

	// Connect menu bar actions
	connect(ui->actionNew_Project, SIGNAL(triggered()), this, SLOT(createProject()));
	connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(openProject()));
	connect(ui->actionClose_Project, SIGNAL(triggered()), this, SLOT(closeProject()));
	connect(ui->actionExit, SIGNAL(triggered()), this, SIGNAL(quit()));

	// Connect map display features
	connect(ui->actionStreet, SIGNAL(triggered()), ui->GLPanel, SLOT(toggleStreet()));
	connect(ui->actionSatellite, SIGNAL(triggered()), ui->GLPanel, SLOT(toggleSattelite()));
	connect(ui->actionTerrain, SIGNAL(triggered()), ui->GLPanel, SLOT(toggleTerrain()));

	// Try out moving the tool buttons into a toolbar
	QToolBar* toolBar = new QToolBar(this);
	toolBar->layout()->setSpacing(0);
	toolBar->layout()->setContentsMargins(0, 0, 0, 0);
	toolBar->setIconSize(QSize(18, 18));
	toolBar->addWidget(ui->newProjectButton);
	toolBar->addWidget(ui->openProjectButton);
	toolBar->addWidget(ui->projectOptionsButton);
	toolBar->addWidget(ui->saveProjectButton);
	toolBar->addWidget(ui->addFileToProjectButton);
	ui->projectButtonsLayout->addWidget(toolBar);

	// Load recently opened projects
	LoadRecentProjects();


    //aa15
    ui->analyzeStacked->setCurrentIndex( ui->analyzeComboBox->currentIndex() );

}

MainWindow::~MainWindow()
{
	if (currentProject)
		delete currentProject;

	delete ui;

	CheckForMemoryLeaks();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_E)
	{
		if (currentProject)
		{
			if (event->modifiers() & Qt::ShiftModifier)
				currentProject->DeselectFullDomainClickElements();
			else
				currentProject->SelectFullDomainClickElements();
		}
	}

	else if (event->key() == Qt::Key_C)
	{
		if (currentProject)
		{
			if (event->modifiers() & Qt::ShiftModifier)
				currentProject->DeselectFullDomainCircleElements();
			else
				currentProject->SelectFullDomainCircleElements();
		}
	}

	else if (event->key() == Qt::Key_R)
	{
		if (currentProject)
		{
			if (event->modifiers() & Qt::ShiftModifier)
				currentProject->DeselectFullDomainRectangleElements();
			else
				currentProject->SelectFullDomainRectangleElements();
		}
	}

	else if (event->key() == Qt::Key_S)
	{
		if (currentProject)
		{
			if (event->modifiers() & Qt::ShiftModifier)
				currentProject->DeselectFullDomainPolygonElements();
			else
				currentProject->SelectFullDomainPolygonElements();
		}
	}

	else if (event->key() == Qt::Key_Z)
	{
		if (currentProject)
		{
			if (event->modifiers() & Qt::ControlModifier)
			{
				if (event->modifiers() & Qt::ShiftModifier)
				{
					if (ui->redoButton->isEnabled())
						currentProject->Redo();
				} else {
					if (ui->undoButton->isEnabled())
						currentProject->Undo();
				}
			}
		}
	}

	else if (event->key() == Qt::Key_0)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(0);
	}

	else if (event->key() == Qt::Key_1)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(1);
	}

	else if (event->key() == Qt::Key_2)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(2);
	}

	else if (event->key() == Qt::Key_3)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(3);
	}

	else if (event->key() == Qt::Key_4)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(4);
	}

	else if (event->key() == Qt::Key_5)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(5);
	}

	else if (event->key() == Qt::Key_6)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(6);
	}

	else if (event->key() == Qt::Key_7)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(7);
	}

	else if (event->key() == Qt::Key_8)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(8);
	}

	else if (event->key() == Qt::Key_9)
	{
		if (currentProject)
			if (event->modifiers() & Qt::AltModifier)
				currentProject->DisplayDomain(9);
	}
}


void MainWindow::CheckForMemoryLeaks()
{
	if (GLShader::GetNumShaders() != 0)
	{
		DEBUG("MEMORY LEAK: " << GLShader::GetNumShaders() << " GLShader objects not deleted");
	}
	if (Layer::GetNumLayers() != 0)
	{
		DEBUG("MEMORY LEAK: " << Layer::GetNumLayers() << " Layer objects not deleted");
	}
	if (GLCamera::GetNumCameras() != 0)
	{
		DEBUG("MEMORY LEAK: " << GLCamera::GetNumCameras() << " GLCamera objects not deleted");
	}
}


void MainWindow::CreateProject(bool currentProjectFile)
{
	if (currentProjectFile)
	{

		Project* newProject = new Project(this);
		if (newProject->IsInitialized())
		{
			if (currentProject)
				delete currentProject;
			currentProject = newProject;
		} else {
			delete newProject;
			return;
		}
	} else {
		QStringList selections;
		QFileDialog dialog(0, "Open an ADCIRC Subdomain Project", QDir::homePath());
		dialog.setModal(true);
		dialog.setNameFilter("ADCIRC Subdomain Projects (*.spf)");
		dialog.setFileMode(QFileDialog::ExistingFile);
		if (dialog.exec())
		{
			if (currentProject)
				delete currentProject;

			selections = dialog.selectedFiles();
			if (!selections.isEmpty())
			{
				currentProject = new Project(selections.first(), this);
			}
		} else {
			return;
		}
	}

	QString currFile = currentProject->GetFilePath();
	if (!currFile.isEmpty())
	{
		QStringList files = settings.value("recentProjectList").toStringList();
		files.removeAll(currFile);
		files.prepend(currFile);
		while (files.size() > MaxRecentProjects)
			files.removeLast();

		settings.setValue("recentProjectList", files);
		UpdateRecentProjects();
	}

	ConnectProject(currentProject);
}


void MainWindow::ConnectProject(Project *proj)
{

	proj->SetOpenGLPanel(ui->GLPanel);
	proj->SetProgressBar(ui->progressBar);
	proj->SetProjectTree(ui->projectTree);
	proj->SetEditSubdomainList(ui->editSubdomainList);

	/* Subdomain Creation */
	connect(ui->createSubdomainButton, SIGNAL(clicked()), proj, SLOT(CreateNewSubdomain()));

	/* Selection Tools */
	connect(ui->selectNodesCircle, SIGNAL(clicked()), proj, SLOT(SelectFullDomainCircleElements()));
	connect(ui->selectElementSingle, SIGNAL(clicked()), proj, SLOT(SelectFullDomainClickElements()));
	connect(ui->selectNodeSingle, SIGNAL(clicked()), proj, SLOT(SelectFullDomainPolygonElements()));
	connect(ui->selectNodesSquare, SIGNAL(clicked()), proj, SLOT(SelectFullDomainRectangleElements()));
	connect(ui->deselectElementCircle, SIGNAL(clicked()), proj, SLOT(DeselectFullDomainCircleElements()));
	connect(ui->deselectElementPolygon, SIGNAL(clicked()), proj, SLOT(DeselectFullDomainPolygonElements()));
	connect(ui->deselectElementSingle, SIGNAL(clicked()), proj, SLOT(DeselectFullDomainClickElements()));
	connect(ui->deselectElementSquare, SIGNAL(clicked()), proj, SLOT(DeselectFullDomainRectangleElements()));
	connect(ui->selectNodeButton, SIGNAL(clicked()), proj, SLOT(SelectSingleSubdomainNode()));

	connect(ui->undoButton, SIGNAL(clicked()), proj, SLOT(Undo()));
	connect(ui->redoButton, SIGNAL(clicked()), proj, SLOT(Redo()));
	connect(ui->actionUndo, SIGNAL(triggered()), proj, SLOT(Undo()));
	connect(ui->actionRedo, SIGNAL(triggered()), proj, SLOT(Redo()));

	/* Subdomain Editing */
	connect(proj, SIGNAL(editNode(uint,QString,QString,QString)), this, SLOT(editNode(uint,QString,QString,QString)));

	/* U/I Updates */
	connect(proj, SIGNAL(mouseX(float)), this, SLOT(showMouseX(float)));
	connect(proj, SIGNAL(mouseY(float)), this, SLOT(showMouseY(float)));
	connect(proj, SIGNAL(undoAvailable(bool)), ui->undoButton, SLOT(setEnabled(bool)));
	connect(proj, SIGNAL(redoAvailable(bool)), ui->redoButton, SLOT(setEnabled(bool)));
	connect(proj, SIGNAL(numElements(int)), this, SLOT(showNumElements(int)));
	connect(proj, SIGNAL(numNodes(int)), this, SLOT(showNumNodes(int)));
	connect(proj, SIGNAL(numElementsSelected(int)), this, SLOT(showNumSelectedElements(int)));
	connect(proj, SIGNAL(numNodesSelected(int)), this, SLOT(showNumSelectedNodes(int)));
	connect(proj, SIGNAL(maxSelectedZ(float)), this, SLOT(showMaxSelectedZ(float)));
	connect(proj, SIGNAL(minSelectedZ(float)), this, SLOT(showMinSelectedZ(float)));
	connect(proj, SIGNAL(showProjectView()), this, SLOT(showProjectExplorerPane()));

	/* Running ADCIRC */
	connect(ui->actionFull_Domain, SIGNAL(triggered()), proj, SLOT(RunFullDomain()));
	connect(ui->runFullDomainButton, SIGNAL(clicked()), proj, SLOT(RunFullDomain()));
	connect(proj, SIGNAL(subdomainCreated(QString)), this, SLOT(addSubdomainToList(QString)));

	/* Color Options Action Bar */
	connect(ui->actionColor_Options, SIGNAL(triggered()), proj, SLOT(ShowDisplayOptionsDialog()));

	/* Update list with already created subdomains */
	QStringList currSubs = proj->GetSubdomainNames();
	for (int i=0; i<currSubs.size(); ++i)
		addSubdomainToList(currSubs.at(i));

    /* Analyze */
    fill_visualizeDomainComboBox();
}


void MainWindow::LoadRecentProjects()
{
	QList<QAction*> actionList;
	for (int i=0; i<MaxRecentProjects; ++i)
	{
		recentProjects[i] = new QAction(this);
		recentProjects[i]->setVisible(false);
		connect(recentProjects[i], SIGNAL(triggered()),
			this, SLOT(openRecentProject()));
		actionList.append(recentProjects[i]);
	}
	recentProjectsSeparator = new QAction(this);
	recentProjectsSeparator->setSeparator(true);
	recentProjectsSeparator->setVisible(false);
	actionList.append(recentProjectsSeparator);

	ui->menuFile->insertActions(ui->actionClose_Project, actionList);

	UpdateRecentProjects();
}


void MainWindow::UpdateRecentProjects()
{
	QStringList files = settings.value("recentProjectList").toStringList();
	int numRecentProjects = qMin(files.size(), (int)MaxRecentProjects);

	for (int i=0; i<numRecentProjects; ++i)
	{
		QString text = tr("&%1 %2").arg(i+1).arg(QFileInfo(files[i]).fileName());
		recentProjects[i]->setText(text);
		recentProjects[i]->setData(files[i]);
		recentProjects[i]->setVisible(true);
	}

	for (int i=numRecentProjects; i<MaxRecentProjects; ++i)
		recentProjects[i]->setVisible(false);

	recentProjectsSeparator->setVisible(numRecentProjects > 0);
}


void MainWindow::showMaxSelectedZ(float newZ)
{
	ui->createMaxElevation->setText(QString::number(newZ, 'f', 4));
}


void MainWindow::showMinSelectedZ(float newZ)
{
	ui->createMinElevation->setText(QString::number(newZ, 'f', 4));
}


void MainWindow::showMouseX(float newX)
{
	if (mouseXLabel)
		mouseXLabel->setText(QString("<b>X:</b> ").append(QString::number(newX, 'f', 4)).append("   "));
}


void MainWindow::showMouseY(float newY)
{
	if (mouseYLabel)
		mouseYLabel->setText(QString("<b>Y:</b> ").append(QString::number(newY, 'f', 4)).append("   "));
}


void MainWindow::showNumNodes(int numNodes)
{
	if (numNodesLabel)
		numNodesLabel->setText(QString("<b>Nodes:</b> ").append(QString::number(numNodes)).append("   "));
}


void MainWindow::showNumElements(int numElements)
{
	if (numElementsLabel)
		numElementsLabel->setText(QString("<b>Elements:</b> ").append(QString::number(numElements)).append("   "));
}


void MainWindow::showNumTS(int numTS)
{
	if (numTSLabel)
		numTSLabel->setText(QString("<b>Timesteps:</b> ").append(QString::number(numTS)).append("   "));
}


void MainWindow::showNumSelectedNodes(int numNodes){
	ui->createNumNodesSelected->setText(QString::number(numNodes));
}


void MainWindow::showNumSelectedElements(int numElements)
{
	ui->createNumElementsSelected->setText(QString::number(numElements));
}


void MainWindow::showCircleStats(float x, float y, float rad)
{
	if (glStatusBar)
		glStatusBar->showMessage(QString("Center: ").append(QString::number(x, 'f', 5)).append(", ").append(QString::number(y, 'f', 5)).append("   Radius: ").append(QString::number(rad, 'f', 5)));
}


void MainWindow::createProject()
{
	CreateProject(true);
}


void MainWindow::openProject()
{
	CreateProject(false);
}


void MainWindow::openRecentProject()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
	{
		if (currentProject)
			delete currentProject;
		currentProject = new Project(action->data().toString(), this);
		ConnectProject(currentProject);
	}
}


void MainWindow::closeProject()
{
	if (currentProject)
	{
		delete currentProject;
		currentProject = 0;
	}
	ui->projectTree->clear();
}


void MainWindow::addSubdomainToList(QString s)
{
	QListWidgetItem* newItemRun = new QListWidgetItem(s);
	newItemRun->setFlags(newItemRun->flags() | Qt::ItemIsUserCheckable);
	newItemRun->setCheckState(Qt::Unchecked);
	ui->subdomainsToRunList->addItem(newItemRun);

	QListWidgetItem* newItemEdit = new QListWidgetItem(s);
	newItemEdit->setFlags(newItemEdit->flags() | Qt::ItemIsSelectable);
	ui->editSubdomainList->addItem(newItemEdit);
}


void MainWindow::editNode(unsigned int nodeNum, QString x, QString y, QString z)
{
	ui->editNodeNumber->setText(QString::number(nodeNum));
	ui->editXLoc->setText(x);
	ui->editYLoc->setText(y);
	ui->editZLoc->setText(z);
}


void MainWindow::showProjectExplorerPane()
{
	ui->paneBox->setCurrentIndex(0);
}


void MainWindow::showCreateSubdomainPane()
{
	ui->paneBox->setCurrentIndex(1);
}


void MainWindow::showEditSubdomainPane()
{
	ui->paneBox->setCurrentIndex(2);
}


void MainWindow::showAdcircPane()
{
	ui->paneBox->setCurrentIndex(3);
}


void MainWindow::showAnalyzeResultsPane()
{
	ui->paneBox->setCurrentIndex(4);
}


void MainWindow::setNodeValues()
{
	if (currentProject)
	{
		QString currentSubdomain = ui->editSubdomainList->currentItem()->text();
		unsigned int nodeNumber = ui->editNodeNumber->text().toUInt();
		currentProject->SetNodalValues(currentSubdomain,
					       nodeNumber,
					       ui->editXLoc->text(),
					       ui->editYLoc->text(),
					       ui->editZLoc->text());
	}
}


void MainWindow::resetCurrentNodeValues()
{
	if (currentProject)
	{
		QString currentSubdomain = ui->editSubdomainList->currentItem()->text();
		unsigned int nodeNumber = ui->editNodeNumber->text().toUInt();
		currentProject->ResetNodalValues(currentSubdomain, nodeNumber);
	}
}


void MainWindow::resetAllNodalValues()
{
	if (currentProject)
	{
		QString currentSubdomain = ui->editSubdomainList->currentItem()->text();
		currentProject->ResetAllNodalValues(currentSubdomain);
	}
}


void MainWindow::saveAllSubdomainChanges()
{
	if (currentProject)
	{
		QString currentSubdomain = ui->editSubdomainList->currentItem()->text();
		currentProject->SaveSubdomainChanges(currentSubdomain);
	}
}


void MainWindow::printText(QString text)
{
	std::cout << text.toStdString().data() << std::endl;
}

void MainWindow::on_analyzeComboBox_currentIndexChanged(int index)
{
    ui->analyzeStacked->setCurrentIndex(index);
}


//aa15
void MainWindow::on_paneBox_currentChanged(int index)
{
    int analysisIndex = 4;

    if (currentProject!=0){    // if there is an active project:

        if ( index==analysisIndex ){ // ANALYZE RESULTS:
            fill_visualizeDomainComboBox();
        }
        else{
            // ...
        }

    }

}


void MainWindow::fill_visualizeDomainComboBox(){


    // insert items to comboboxes
    ui->visualizeDomainComboBox->clear();
    ui->visualizeDomainComboBox->addItem("Full Domain");
    ui->compareSubdomainComboBox->clear();

    int nsubdomains = currentProject->GetNumberOfSubdomains();
    if ( nsubdomains > 0){
        QStringList subdomainNames = currentProject->GetSubdomainNames();
        for (int i=0;i<nsubdomains;i++){
            ui->visualizeDomainComboBox->addItem(subdomainNames[i]);
            ui->compareSubdomainComboBox->addItem(subdomainNames[i]);
        }
    }

    ui->fullMaxeleLineEdit->setText( currentProject->GetDomainPath(0)+"/maxele.63" );

}



void MainWindow::on_openFileButton_clicked()
{

    QString maxeleFileName = QFileDialog::getOpenFileName(this,"Open File",QString(),"Maxele File (*.63)");
    ui->maxeleDirLineEdit->setText(maxeleFileName);
}


void MainWindow::on_visualizeGridButton1_clicked()
{
    int domainIndex = ui->visualizeDomainComboBox->currentIndex();

    if (ui->visualizeDomainComboBox->count()==0 ) {
        QMessageBox::warning(this,"Project!","No domain is avaliable to visualize. Open or create a project.");
        return;
    }

    // display grid
    if (currentProject!=0){
        currentProject->setDisplay("default",domainIndex, "","");
    }
}


void MainWindow::on_visualizeMaxeleButton_clicked()
{
    int domainIndex = ui->visualizeDomainComboBox->currentIndex();
    QString maxeleFileName = ui->maxeleDirLineEdit->text();


    if (ui->visualizeDomainComboBox->count()==0 ) {
        QMessageBox::warning(this,"Project!","No domain is avaliable to visualize. Open or create a project.");
        return;
    }
    else if (maxeleFileName.isEmpty()) {
        QMessageBox::warning(this,"Maxele Path!","Provide the maxele path.");
        return;
    }

    // set display
    if (currentProject!=0){
        currentProject->setDisplay("maxele",domainIndex, maxeleFileName,"");
    }


}

void MainWindow::on_visualizeDomainComboBox_currentIndexChanged(const QString &arg1)
{
    int newIndex = ui->visualizeDomainComboBox->currentIndex();
    currentProject->DisplayDomain(newIndex);
    ui->maxeleDirLineEdit->setText( currentProject->GetDomainPath(newIndex)+"/maxele.63" );

    // synchronize two comboboxes
    if (ui->compareSubdomainComboBox->currentIndex() != newIndex-1 and
            ui->compareSubdomainComboBox->currentIndex() != 0 ){
        ui->compareSubdomainComboBox->setCurrentIndex(newIndex-1);
    }
}



void MainWindow::on_openFullMaxeleButton_clicked()
{
    QString fullMaxeleFileName = QFileDialog::getOpenFileName(this,"Open File",QString(),"Full Maxele File (*.63)");
    ui->fullMaxeleLineEdit->setText(fullMaxeleFileName);
}

void MainWindow::on_openSubMaxeleButton_clicked()
{
    QString subMaxeleFileName = QFileDialog::getOpenFileName(this,"Open File",QString(),"Subdomain Maxele File (*.63)");
    ui->subMaxeleLineEdit->setText(subMaxeleFileName);
}


void MainWindow::on_compareSubdomainComboBox_currentIndexChanged(int index)
{
    int newIndex = ui->compareSubdomainComboBox->currentIndex()+1;
    currentProject->DisplayDomain(newIndex);
    ui->subMaxeleLineEdit->setText( currentProject->GetDomainPath(newIndex)+"/maxele.63" );

    // synchronize two comboboxes
    if (ui->visualizeDomainComboBox->currentIndex() != newIndex){
        ui->visualizeDomainComboBox->setCurrentIndex(newIndex);
    }
}

void MainWindow::on_visualizeGridButton2_clicked()
{
    int domainIndex = ui->compareSubdomainComboBox->currentIndex()+1;

    if (ui->compareSubdomainComboBox->count()==0 ) {
        QMessageBox::warning(this,"Project!","No subdomain is avaliable to visualize. Open or create a project.");
        return;
    }

    // display grid
    if (currentProject!=0){
        currentProject->setDisplay("default",domainIndex, "","");
    }
}

void MainWindow::on_visualizeMaxeleComparisonButton_clicked()
{

    QString fullMaxeleFileName = ui->fullMaxeleLineEdit->text();
    int subdomainIndex = ui->compareSubdomainComboBox->currentIndex()+1;
    QString subMaxeleFileName = ui->subMaxeleLineEdit->text();

    if (ui->compareSubdomainComboBox->count()==0 ) {
        QMessageBox::warning(this,"Project!","No subdomain is avaliable to visualize. Open or create a project.");
        return;
    }
    else if (fullMaxeleFileName.isEmpty()) {
        QMessageBox::warning(this,"Maxele Path!","Provide the full domain maxele path.");
        return;
    }
    else if (subMaxeleFileName.isEmpty()) {
        QMessageBox::warning(this,"Maxele Path!","Provide the subdomain maxele path.");
        return;
    }


    // set display
    if (currentProject!=0){
        currentProject->setDisplay("compareMaxele",
                                   subdomainIndex,
                                   subMaxeleFileName,
                                   fullMaxeleFileName);
    }


}
