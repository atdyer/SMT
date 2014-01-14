#ifndef FORT066_NEW_H
#define FORT066_NEW_H

#include <map>
#include <fstream>
#include <istream>
#include <sstream>

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort066 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort066(QObject *parent=0);
		Fort066(ProjectFile *projectFile, QObject *parent=0);

		bool					StartReading();
		bool					HasNextTimestep();
		std::map<unsigned int, std::string>	GetNextTimestep();
		int					GetNumTimesteps();
		void					FinishedReading();

	private:

		std::map<unsigned int, std::string>	currentData;
		int					currTS;
		std::ifstream				file;
		int					numNodes;
		int					numTS;
		ProjectFile*			projectFile;


		void	SearchForFile();
};

#endif // FORT066_NEW_H
