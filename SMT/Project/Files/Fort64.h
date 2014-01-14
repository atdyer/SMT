#ifndef FORT64_H
#define FORT64_H

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort64 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort64(QObject *parent=0);
		Fort64(ProjectFile *projectFile, QObject *parent=0);
		Fort64(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // FORT64_H
