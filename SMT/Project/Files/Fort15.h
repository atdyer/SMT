#ifndef FORT15_H
#define FORT15_H

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort15 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort15(QObject *parent=0);
		Fort15(ProjectFile *projectFile, QObject *parent=0);
		Fort15(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // FORT15_H
