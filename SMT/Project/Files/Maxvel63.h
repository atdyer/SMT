#ifndef MAXVEL63_H
#define MAXVEL63_H


#include <QObject>

#include "Project/Files/ProjectFile.h"

class Maxvel63 : public QObject
{
		Q_OBJECT
	public:
		explicit Maxvel63(QObject *parent=0);
		Maxvel63(ProjectFile *projectFile, QObject *parent=0);
		Maxvel63(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // MAXVEL63_H
