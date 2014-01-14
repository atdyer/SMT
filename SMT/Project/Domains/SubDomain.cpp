#include "SubDomain.h"


SubDomain::SubDomain(QString domainName, ProjectFile *projectFile, QObject *parent) :
	Domain(projectFile, parent),
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
