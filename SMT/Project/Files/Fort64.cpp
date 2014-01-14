#include "Fort64.h"

Fort64::Fort64(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort64::Fort64(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort64::Fort64(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}

