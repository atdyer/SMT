#include "Fort066.h"

Fort066::Fort066(QObject *parent) :
	QObject(parent),
	currentData(),
	currTS(0),
	file(),
	numNodes(0),
	numTS(0),
	projectFile(0)
{
}


Fort066::Fort066(ProjectFile *projectFile, QObject *parent) :
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
		QString targetFile = projectFile->GetFullDomainFort066();
		if (!targetFile.isEmpty())
		{
			SearchForFile();
		}
	}
}


bool Fort066::StartReading()
{
	if (projectFile)
	{
		QString targetFile = projectFile->GetFullDomainFort066();
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


bool Fort066::HasNextTimestep()
{
	return currTS < numTS;
}


std::map<unsigned int, std::string> Fort066::GetNextTimestep()
{
	currentData.clear();
	if (file.is_open())
	{
		std::string tsLine, line1, line2;
		unsigned int currNode;
		std::getline(file, tsLine);
		currentData[0] = tsLine;
		for (int i=0; i<numNodes; ++i)
		{
			std::getline(file, line1);
			std::getline(file, line2);
			std::stringstream(line1) >> currNode;
			currentData[currNode] = line1 + "\n" + line2 + "\n";
		}
	}
	++currTS;
	return currentData;
}


int Fort066::GetNumTimesteps()
{
	return numTS;
}


void Fort066::FinishedReading()
{
	if (file.is_open())
		file.close();
	currentData.clear();
	currTS = 0;
	numNodes = 0;
	numTS = 0;
}


void Fort066::SearchForFile()
{
	if (projectFile)
	{
		QDir targetDirectory (projectFile->GetFullDomainDirectory());
		if (targetDirectory.exists())
		{
			if (targetDirectory.exists("fort.066"))
			{
				projectFile->SetFullDomainFort066(targetDirectory.absoluteFilePath("fort.066"));
			}
		}
	}
}
