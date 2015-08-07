#include "FullDomain.h"
#include <QMessageBox>

FullDomain::FullDomain(ProjectFile *projectFile, QObject *parent) :
	Domain(projectFile, parent),
	selectionLayerFullDomain(0),
	fort13(0),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort066(0),
	fort067(0),
    maxele63(0),
	maxvel(0)
{
	CreateAllFiles();
	selectionLayerFullDomain = new FullDomainSelectionLayer(fort14, this);
	selectionLayer = selectionLayerFullDomain;
	selectionLayer->SetCamera(camera);

	connect(selectionLayer, SIGNAL(ToolFinishedDrawing()), this, SLOT(EnterDisplayMode()));
	connect(selectionLayer, SIGNAL(UndoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
	connect(selectionLayer, SIGNAL(RedoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
	connect(selectionLayer, SIGNAL(NumElementsSelected(int)), this, SIGNAL(numElementsSelected(int)));
	connect(selectionLayer, SIGNAL(NumNodesSelected(int)), this, SIGNAL(numNodesSelected(int)));
	connect(selectionLayer, SIGNAL(MaxSelectedZ(float)), this, SIGNAL(maxSelectedZ(float)));
	connect(selectionLayer, SIGNAL(MinSelectedZ(float)), this, SIGNAL(minSelectedZ(float)));

	connect(fort14, SIGNAL(NumElementsSet(int)), this, SIGNAL(numElements(int)));
	connect(fort14, SIGNAL(NumNodesSet(int)), this, SIGNAL(numNodes(int)));
}


FullDomain::~FullDomain()
{
	std::cout << "Deleted full domain" << std::endl;
}


bool FullDomain::IsFullDomain()
{
	return true;
}


Fort015* FullDomain::GetFort015()
{
	return fort015;
}


std::vector<unsigned int> FullDomain::GetInnerBoundaryNodes()
{
	return selectionLayerFullDomain->GetInnerBoundaryNodes();
}


std::vector<unsigned int> FullDomain::GetOuterBoundaryNodes()
{
	return selectionLayerFullDomain->GetOuterBoundaryNodes();
}


QString FullDomain::GetPath()
{
	if (projectFile)
		return projectFile->GetFullDomainDirectory();
	return QString();
}


std::vector<Element*> FullDomain::GetSelectedElements()
{
	return selectionLayerFullDomain->GetSelectedElements();
}

void FullDomain::CreateAllFiles()
{
	fort13 = new Fort13(projectFile, this);
    fort14 = new Fort14(projectFile, this);
	fort15 = new Fort15(projectFile, this);
	fort22 = new Fort22(projectFile, this);
	fort63 = new Fort63(projectFile, this);
	fort64 = new Fort64(projectFile, this);
	fort015 = new Fort015(projectFile, this);
	fort066 = new Fort066(projectFile, this);
	fort067 = new Fort067(projectFile, this);
    //maxele = new Maxele63(projectFile, this); // aa15-commented out
	maxvel = new Maxvel63(projectFile, this);

	fort14->SetCamera(camera);
	mapLayer->SetFort14(fort14);
}


void FullDomain::visualizeDomain(QString displayMode, QString FileName){

    if (displayMode == "maxele"){

        maxele63 = new Maxele63(FileName, "Full Domain", projectFile,  this);
        if ( fort14->setMaxele( maxele63 ) ){
            float minMaxele = maxele63->GetMinMaxele();
            float maxMaxele = maxele63->GetMaxMaxele();
            fort14->maxeleGL(minMaxele,maxMaxele);
            updateGL();
        }
        else{
            QMessageBox::warning(0,"Maxele!","Maxele file is not compatible with the domain");
            return;
        }
    }
    else if (displayMode =="compareMaxele"){
        maxele63 = new Maxele63(FileName, "Full Domain", projectFile,  this);
        if ( fort14->setMaxele( maxele63 ) ){
        }
        else{
            QMessageBox::warning(0,"Maxele!","Full Maxele file is not compatible with the domain");
            return;
        }
    }
    else if (displayMode =="default"){
        fort14->RefreshGL();
        updateGL();
    }

}
