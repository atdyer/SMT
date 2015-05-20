#ifndef MAXELE63_H
#define MAXELE63_H

#include <QObject>
#include <QString>

#include <vector>
#include <istream>
#include <fstream>
#include <sstream>

#include "Project/Files/ProjectFile.h"

class Maxele63 : public QObject
{
		Q_OBJECT
	public:
		explicit Maxele63(QObject *parent=0);
		Maxele63(ProjectFile *projectFile, QObject *parent=0);
		Maxele63(QString domainName, ProjectFile *projectFile, QObject *parent=0);

        QString                 GetHeaderLine();
        unsigned int            GetNumNodes();
        std::vector<float>      GetMaxeles();


	private:

        QString             domainName;
        ProjectFile*        projectFile;
        bool                isFullDomain;

        QString             headerLine;
        unsigned int        numNodes;
        std::vector<float>  maxeles;

        void ReadFile();
};

#endif // MAXELE63_H
