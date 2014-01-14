#ifndef FORT22_H
#define FORT22_H

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort22 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort22(QObject *parent=0);
		Fort22(ProjectFile *projectFile, QObject *parent=0);
		Fort22(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // FORT22_H
