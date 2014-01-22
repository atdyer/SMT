#include "SubDomain.h"


SubDomain::SubDomain(QString domainName, ProjectFile *projectFile, QObject *parent) :
	Domain(projectFile, parent),
	selectionLayerSubdomain(0),
	bnList(0),
	domainName(domainName),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort020(0),
	fort021(0),
	fort022(0),
	maxele(0),
	maxvel(0),
	py140(0),
	py141(0)
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


void SubDomain::SetNodalValues(unsigned int nodeNumber, QString x, QString y, QString z)
{
	if (fort14)
	{
		fort14->SetNodalValues(nodeNumber, x, y, z);
	}
}


void SubDomain::CreateAllFiles()
{
	bnList = new BNList14(domainName, projectFile, this);
	fort14 = new Fort14(domainName, projectFile, this);
	fort15 = new Fort15(domainName, projectFile, this);
	fort22 = new Fort22(domainName, projectFile, this);
	fort63 = new Fort63(domainName, projectFile, this);
	fort64 = new Fort64(domainName, projectFile, this);
	fort015 = new Fort015(domainName, projectFile, this);
	fort020 = new Fort020(domainName, projectFile, this);
	fort021 = new Fort021(domainName, projectFile, this);
	fort022 = new Fort022(domainName, projectFile, this);
	maxele = new Maxele63(domainName, projectFile, this);
	maxvel = new Maxvel63(domainName, projectFile, this);
	py140 = new Py140(domainName, projectFile, this);
	py141 = new Py141(domainName, projectFile, this);

	fort14->SetCamera(camera);
}
