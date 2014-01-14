#include "Fort022.h"

Fort022::Fort022(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort022::Fort022(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
