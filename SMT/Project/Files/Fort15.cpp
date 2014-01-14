#include "Fort15.h"

Fort15::Fort15(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort15::Fort15(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort15::Fort15(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
