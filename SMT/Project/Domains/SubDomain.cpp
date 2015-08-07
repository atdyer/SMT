#include "SubDomain.h"


SubDomain::SubDomain(QString domainName, ProjectFile *projectFile, QObject *parent) :
	Domain(projectFile, parent),
	selectionLayerSubdomain(0),
	bnList(0),
	domainName(domainName),
	fort13(0),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort020(0),
	fort021(0),
	fort022(0),
    maxele63(0),
	maxvel(0),
	py140(0),
	py141(0),
	editedNodes()
{
	CreateAllFiles();
	selectionLayerSubdomain = new SubDomainSelectionLayer(fort14, this);
	selectionLayer = selectionLayerSubdomain;
	selectionLayer->SetCamera(camera);

	connect(selectionLayerSubdomain, SIGNAL(editNode(uint,QString,QString,QString)),
		this, SIGNAL(editNode(uint,QString,QString,QString)));

	connect(selectionLayer, SIGNAL(ToolFinishedDrawing()), this, SLOT(EnterDisplayMode()));
	connect(selectionLayer, SIGNAL(UndoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
	connect(selectionLayer, SIGNAL(RedoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
	connect(selectionLayer, SIGNAL(NumElementsSelected(int)), this, SIGNAL(numElementsSelected(int)));
	connect(selectionLayer, SIGNAL(NumNodesSelected(int)), this, SIGNAL(numNodesSelected(int)));
	connect(selectionLayer, SIGNAL(MaxSelectedZ(float)), this, SIGNAL(maxSelectedZ(float)));
	connect(selectionLayer, SIGNAL(MinSelectedZ(float)), this, SIGNAL(minSelectedZ(float)));

	connect(fort14, SIGNAL(NumElementsSet(int)), this, SIGNAL(numElements(int)));
	connect(fort14, SIGNAL(NumNodesSet(int)), this, SIGNAL(numNodes(int)));
	connect(fort14, SIGNAL(Refresh()), this, SIGNAL(updateGL()));
}


SubDomain::~SubDomain()
{
	std::cout << "Deleted subdomain: " << domainName.toStdString() << std::endl;
}


bool SubDomain::IsFullDomain()
{
	return false;
}


Fort015* SubDomain::GetFort015()
{
	return fort015;
}


Py140* SubDomain::GetPy140()
{
	return py140;
}


QString SubDomain::GetPath()
{
	if (projectFile)
		return projectFile->GetSubDomainDirectory(domainName);
	return QString();
}


QString SubDomain::GetDomainName()
{
	return domainName;
}


Node* SubDomain::GetSelectedNode()
{
	return selectionLayerSubdomain->GetSelectedNode();
}


void SubDomain::ResetNodalValues(unsigned int nodeNumber, Fort14 *fullDomainFort14)
{
	if (fullDomainFort14 && py140 && fort14)
	{
		// Get the full domain node number from the py.140 file
		unsigned int fullDomainNodeNumber = py140->ConvertNewToOld(nodeNumber);

		// Get the original nodal values from the full domain fort.14
		Node originalNode = fullDomainFort14->GetNode(fullDomainNodeNumber);

		QString xDat (originalNode.xDat.data());
		QString yDat (originalNode.yDat.data());
        QString zDat (originalNode.zDat.data());

		// Set the subdomain nodal values
        fort14->SetNodalValues(nodeNumber, xDat, yDat, zDat);

		// Update the UI with the reset values
        emit editNode(nodeNumber, xDat, yDat, zDat);
	}
}


void SubDomain::ResetAllNodalValues(Fort14 *fullDomainFort14)
{
	for (std::set<unsigned int>::iterator it = editedNodes.begin();
	     it != editedNodes.end(); ++it)
	{
		unsigned int currNode = *it;
		ResetNodalValues(currNode, fullDomainFort14);
	}
	editedNodes.clear();
}


void SubDomain::SaveAllChanges()
{
	if (fort14)
	{
		fort14->SaveChanges();
	}
}


void SubDomain::SetNodalValues(unsigned int nodeNumber, QString x, QString y, QString z)
{
	if (fort14)
	{
		fort14->SetNodalValues(nodeNumber, x, y, z);
		editedNodes.insert(nodeNumber);
	}
}


void SubDomain::CreateAllFiles()
{
	bnList = new BNList14(domainName, projectFile, this);
	fort13 = new Fort13(domainName, projectFile, this);
	fort14 = new Fort14(domainName, projectFile, this);
	fort15 = new Fort15(domainName, projectFile, this);
	fort22 = new Fort22(domainName, projectFile, this);
	fort63 = new Fort63(domainName, projectFile, this);
	fort64 = new Fort64(domainName, projectFile, this);
	fort015 = new Fort015(domainName, projectFile, this);
	fort020 = new Fort020(domainName, projectFile, this);
	fort021 = new Fort021(domainName, projectFile, this);
	fort022 = new Fort022(domainName, projectFile, this);
    //maxele = new Maxele63(domainName, projectFile, this); // aa15-commented out
	maxvel = new Maxvel63(domainName, projectFile, this);
	py140 = new Py140(domainName, projectFile, this);
	py141 = new Py141(domainName, projectFile, this);

	fort14->SetCamera(camera);
	mapLayer->SetFort14(fort14);
}


void SubDomain::setMaxeleDif(Maxele63 * fullMaxele63){

    for (int i=0; i<fort14->GetNumNodes();i++){
        unsigned int old = py140->ConvertNewToOld(i+1);
        fort14->setMaxeleDif( fullMaxele63->GetMaxele(old-1), i );
    }
}


void SubDomain::visualizeDomain(QString displayMode, QString FileName,Maxele63 * fullMaxele63){

    if (displayMode == "maxele"){

        maxele63 = new Maxele63(FileName, "Subdomain", projectFile,  this);
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
    else if (displayMode == "compareMaxele"){
        maxele63 = new Maxele63(FileName, "Subdomain", projectFile,  this);
        if ( fort14->setMaxele( maxele63 ) ){
            setMaxeleDif(fullMaxele63);
            float minMaxele = fort14->GetMinDif();
            float maxMaxele = fort14->GetMaxDif();
            fort14->maxeleGL(minMaxele,maxMaxele);
            updateGL();
        }
        else{
            QMessageBox::warning(0,"Maxele!","Maxele file is not compatible with the domain");
            return;
        }


    }
    else if (displayMode =="default"){
        fort14->RefreshGL();
        updateGL();
    }

}
