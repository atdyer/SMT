#ifndef FORT021_H
#define FORT021_H

#include <QObject>

#include <fstream>
#include <ostream>

#include "Project/Files/ProjectFile.h"

class Fort021 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort021(QObject *parent=0);
		Fort021(QString domainName, ProjectFile *projectFile, QObject *parent=0);

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
#endif // FORT021_H
