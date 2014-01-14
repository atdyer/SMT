#include "Maxvel63.h"

Maxvel63::Maxvel63(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Maxvel63::Maxvel63(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Maxvel63::Maxvel63(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
