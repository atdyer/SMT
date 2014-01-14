#ifndef MAXELE63_H
#define MAXELE63_H

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Maxele63 : public QObject
{
		Q_OBJECT
	public:
		explicit Maxele63(QObject *parent=0);
		Maxele63(ProjectFile *projectFile, QObject *parent=0);
		Maxele63(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // MAXELE63_H
