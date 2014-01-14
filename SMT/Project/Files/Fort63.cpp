#include "Fort63.h"

Fort63::Fort63(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort63::Fort63(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort63::Fort63(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
