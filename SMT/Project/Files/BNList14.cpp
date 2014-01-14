#include "BNList14.h"

BNList14::BNList14(QObject *parent) :
	QObject(parent),
	domainName(),
	innerNodes(),
	outerNodes(),
	projectFile(0),
	targetFile(),
	version(0)
{
}


BNList14::BNList14(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	innerNodes(),
	outerNodes(),
	projectFile(projectFile),
	targetFile(),
	version(0)
{
	if (projectFile && !domainName.isEmpty())
	{
		targetFile = projectFile->GetSubDomainBNList(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "bnlist.14";
				projectFile->SetSubDomainBNList(domainName, targetFile);
			} else {
				// Throw a warning that the subdomain directory doesn't exist
			}
		} else {
			ReadFile();
		}
	}
}


void BNList14::SetInnerBoundaryNodes(std::vector<unsigned int> newNodes)
{
	innerNodes = newNodes;
}


void BNList14::SetOuterBoundaryNodes(std::vector<unsigned int> newNodes)
{
	outerNodes = newNodes;
}


void BNList14::SetSubdomainVersion(int v)
{
	version = v;
}


void BNList14::SaveFile()
{
	if (projectFile && !domainName.isNull())
	{
		std::ofstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			if (version == 1)
			{
				file << outerNodes.size() << "\t!cbn\n";
				for (std::vector<unsigned int>::iterator it = outerNodes.begin(); it != outerNodes.end(); ++it)
				{
					file << *it << "\n";
				}
				file << "0\t!obn\n";
				file << "0\t!ibn\n";
			}
			else if (version == 2)
			{
				file << "0\t!cbn\n";
				file << outerNodes.size() << "\t!obn\n";
				for (std::vector<unsigned int>::iterator it = outerNodes.begin(); it != outerNodes.end(); ++it)
				{
					file << *it << "\n";
				}
				file << innerNodes.size() << "\t!ibn\n";
				for (std::vector<unsigned int>::iterator it = innerNodes.begin(); it != innerNodes.end(); ++it)
				{
					file << *it << "\n";
				}
			}
			else
			{
				// Throw warning that version isn't set
			}
			file.close();

			projectFile->SetSubDomainBNList(domainName, targetFile);
		}
	}
}


QString BNList14::GetFilePath()
{
	if (projectFile && !domainName.isEmpty())
	{
		return projectFile->GetSubDomainBNList(domainName);
	}
	return QString();
}


std::vector<unsigned int> BNList14::GetInnerBoundaryNodes()
{
//	if (!innerNodes.size())
//		ReadFile();
	return innerNodes;
}


std::vector<unsigned int> BNList14::GetOuterBoundaryNodes()
{
//	if (!outerNodes.size())
//		ReadFile();
	return outerNodes;
}


unsigned int BNList14::GetNumInnerBoundaryNodes()
{
	return innerNodes.size();
}


unsigned int BNList14::GetNumOuterBoundaryNodes()
{
	return outerNodes.size();
}


void BNList14::ReadFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		std::ifstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			int cbn = 0;
			int numOuterNodes = 0;
			int numInnerNodes = 0;
			int i=0;
			innerNodes.clear();
			outerNodes.clear();
			std::string line;
			std::getline(file, line);
			std::stringstream(line) >> cbn;

			if (cbn == 0)
			{
				version = 2;
				std::getline(file, line);
				std::stringstream(line) >> numOuterNodes;
				unsigned int currNodeNum = 0;
				while (i < numOuterNodes)
				{
					file >> currNodeNum;
					outerNodes.push_back(currNodeNum);
					++i;
				}
				std::getline(file, line);
				std::getline(file, line);
				std::stringstream(line) >> numInnerNodes;
				i = 0;
				while (i < numInnerNodes)
				{
					file >> currNodeNum;
					innerNodes.push_back(currNodeNum);
					++i;
				}
			} else {
				version = 1;
				numOuterNodes = cbn;
				unsigned int currNodeNum = 0;
				while (i<numOuterNodes)
				{
					file >> currNodeNum;
					outerNodes.push_back(currNodeNum);
					++i;
				}
			}

			file.close();
		}
	}
}
