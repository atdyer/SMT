#ifndef FORT020_H
#define FORT020_H

#include <QObject>

#include <fstream>
#include <ostream>

#include "Project/Files/ProjectFile.h"

class Fort020 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort020(QObject *parent=0);
		Fort020(QString domainName, ProjectFile *projectFile, QObject *parent=0);

		bool	StartWriting();
		void	WriteHeader(int numTS, std::vector<unsigned int> nodeList);
		void	WriteTimestep(std::vector<std::string> tsData);
		void	FinishedWriting();

	private:

		QString			domainName;
		std::ofstream		file;
		ProjectFile*	projectFile;

		QString	GetFilePath();
};

#endif // FORT020_H
