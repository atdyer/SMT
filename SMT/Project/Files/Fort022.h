#ifndef FORT022_H
#define FORT022_H

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort022 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort022(QObject *parent=0);
		Fort022(QString domainName, ProjectFile *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile*	projectFile;
};

#endif // FORT022_H
