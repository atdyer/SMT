#ifndef FORT63_H
#define FORT63_H

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort63 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort63(QObject *parent=0);
		Fort63(ProjectFile *projectFile, QObject *parent=0);
		Fort63(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // FORT63_H
