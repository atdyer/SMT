#ifndef MAXELE63_H
#define MAXELE63_H

#include <QObject>
#include <QString>

#include <vector>
#include <istream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "Project/Files/ProjectFile.h"

class Maxele63 : public QObject
{
		Q_OBJECT
	public:
		explicit Maxele63(QObject *parent=0);
		Maxele63(ProjectFile *projectFile, QObject *parent=0);
		Maxele63(QString domainName, ProjectFile *projectFile, QObject *parent=0);
        Maxele63(QString targetFile, QString domainName, ProjectFile *projectFile, QObject *parent=0);

        QString                 GetHeaderLine();
        unsigned int            GetNumNodes();
        float       GetMaxele(int index);
        float       GetMaxMaxele();
        float       GetMinMaxele();



	private:

        QString             targetFile;
        QString             domainName;
        ProjectFile*        projectFile;
        bool                isFullDomain;

        QString             headerLine;
        unsigned int        numNodes;
        std::vector<float>  maxeles;
        void ReadFile();
};

#endif // MAXELE63_H
