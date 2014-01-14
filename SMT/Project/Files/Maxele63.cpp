#include "Maxele63.h"

Maxele63::Maxele63(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Maxele63::Maxele63(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Maxele63::Maxele63(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
