#include "Fort22.h"

Fort22::Fort22(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort22::Fort22(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort22::Fort22(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
