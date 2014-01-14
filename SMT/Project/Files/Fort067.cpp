#include "Fort067.h"

Fort067::Fort067(QObject *parent) :
	QObject(parent),
	currentData(),
	currTS(0),
	file(),
	numNodes(0),
	numTS(0),
	projectFile(0)
{
}


Fort067::Fort067(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	currentData(),
	currTS(0),
	file(),
	numNodes(0),
	numTS(0),
	projectFile(projectFile)
{
	if (projectFile)
	{
		QString targetFile = projectFile->GetFullDomainFort067();
		if (!targetFile.isEmpty())
		{
			SearchForFile();
		}
	}
}


bool Fort067::StartReading()
{
	if (projectFile)
	{
		QString targetFile = projectFile->GetFullDomainFort067();
		file.open(targetFile.toStdString().data());
		if (file.is_open())
		{
			int NSPOOLGS;
			std::string line;
			std::getline(file, line);
			std::getline(file, line);
			std::stringstream(line) >> NSPOOLGS >> numNodes >> numTS;
			if (numNodes && numTS)
			{
				return true;
			} else {
				file.close();
				return false;
			}
		}
	}
	return false;
}


bool Fort067::HasNextTimestep()
{
	return currTS < numTS;
}


std::map<unsigned int, std::string> Fort067::GetNextTimestep()
{
	currentData.clear();
	if (file.is_open())
	{
		std::string tsLine, line;
		unsigned int currNode;
		std::getline(file, tsLine);
		currentData[0] = tsLine;
		for (int i=0; i<numNodes; ++i)
		{
			file >> currNode;
			std::getline(file, line);
			currentData[currNode] = "\t" + line + "\n";
		}
	}
	++currTS;
	return currentData;
}


int Fort067::GetNumTimesteps()
{
	return numTS;
}


void Fort067::FinishedReading()
{
	if (file.is_open())
		file.close();
	currentData.clear();
	currTS = 0;
	numNodes = 0;
	numTS = 0;
}


void Fort067::SearchForFile()
{
	if (projectFile)
	{
		QDir targetDirectory (projectFile->GetFullDomainDirectory());
		if (targetDirectory.exists())
		{
			if (targetDirectory.exists("fort.067"))
			{
				projectFile->SetFullDomainFort067(targetDirectory.absoluteFilePath("fort.067"));
			}
		}
	}
}
