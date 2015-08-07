#include "Maxele63.h"

Maxele63::Maxele63(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


// full domain constructor - aa15
Maxele63::Maxele63(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
    projectFile(projectFile),
    isFullDomain(true),
    headerLine(),
    numNodes(0)
{
    /*
    if (projectFile)
    {
        targetFile = projectFile->GetFullDomainMaxele();
    }
    */

}


Maxele63::Maxele63(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}


QString Maxele63::GetHeaderLine()
{
    if (headerLine.isEmpty()){
        ReadFile();
    }
    return headerLine;
}

void Maxele63::ReadFile()
{
    // schreib es spaeter
}
