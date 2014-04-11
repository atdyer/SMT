#include "Fort015.h"

/*
 * Default empty constructor
 */
Fort015::Fort015(QObject *parent) :
	QObject(parent),
	domainName(),
	boundaryNodes(),
	enforceBN(0),
	innerBoundaryNodes(),
	isFullDomainFile(false),
	outerBoundaryNodes(),
	projectFile(0),
	recordFrequency(0),
	subdomainApproach(0),
	targetFile()
{
}


/*
 * Full Domain constructor
 */
Fort015::Fort015(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	boundaryNodes(),
	enforceBN(0),
	innerBoundaryNodes(),
	isFullDomainFile(true),
	outerBoundaryNodes(),
	projectFile(projectFile),
	recordFrequency(0),
	subdomainApproach(0),
	targetFile()
{
	targetFile = projectFile->GetFullDomainFort015();
	if (targetFile.isEmpty())
	{
		targetFile = projectFile->GetFullDomainDirectory() + QDir::separator() + "fort.015";
	} else {
		ReadFile();
	}
}


/*
 * Subdomain constructor
 */
Fort015::Fort015(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	boundaryNodes(),
	enforceBN(0),
	innerBoundaryNodes(),
	isFullDomainFile(false),
	outerBoundaryNodes(),
	projectFile(projectFile),
	recordFrequency(0),
	subdomainApproach(0),
	targetFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		targetFile = projectFile->GetSubDomainFort015(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "fort.015";
				projectFile->SetSubDomainFort015(domainName, targetFile);
			} else {
				// Throw a warning that the subdomain directory doesn't exist
			}
		} else {
			ReadFile();
		}
	}


	targetFile = projectFile->GetSubDomainFort015(domainName);
	ReadFile();
}


void Fort015::AddBoundaryNodes(std::vector<unsigned int> newNodes)
{
	for (std::vector<unsigned int>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
	{
		boundaryNodes.insert(*it);
	}
}


void Fort015::AddInnerBoundaryNodes(std::vector<unsigned int> newNodes)
{
	for (std::vector<unsigned int>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
	{
		innerBoundaryNodes.insert(*it);
	}
}


void Fort015::AddOuterBoundaryNodes(std::vector<unsigned int> newNodes)
{
	for (std::vector<unsigned int>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
	{
		outerBoundaryNodes.insert(*it);
	}
}


bool Fort015::FileExists()
{
	return QFile(targetFile).exists();
}


std::set<unsigned int> Fort015::GetBoundaryNodes()
{
	return boundaryNodes;
}


std::set<unsigned int> Fort015::GetInnerBoundaryNodes()
{
	return innerBoundaryNodes;
}


std::set<unsigned int> Fort015::GetOuterBoundaryNodes()
{
	return outerBoundaryNodes;
}


int Fort015::GetRecordFrequency()
{
	return recordFrequency;
}


int Fort015::GetSubdomainApproach()
{
	return subdomainApproach;
}


void Fort015::SetSubdomainApproach(int approach)
{
	subdomainApproach = approach;
}


void Fort015::SetRecordFrequency(int frequency)
{
	recordFrequency = frequency;
}


void Fort015::WriteFile()
{
	std::ofstream file (targetFile.toStdString().data());
	if (file.is_open())
	{
		if (isFullDomainFile)
		{
			file << subdomainApproach << "\t!NOUTGS\n";
			file << recordFrequency << "\t!NSPOOLGS\n";
			file << "0\t!enforceBN\n";
			file << boundaryNodes.size() << "\t!ncbnr\n";
			for (std::set<unsigned int>::iterator it = boundaryNodes.begin(); it != boundaryNodes.end(); ++it)
			{
				file << *it << "\n";
			}
			file << "0\t!nobnr\n";
			file << "0\t!nibnr\n";
			file.close();
			projectFile->SetFullDomainFort015(targetFile);
		} else {
			file << "0\t!NOUTGS\n";
			file << "0\t!NSPOOLGS\n";
			file << subdomainApproach << "\t!enforceBN\n";
			file << "0\n";
			file << "0\n";
			file << "0\n";
			file.close();
			projectFile->SetSubDomainFort015(domainName, targetFile);
		}
	} else {
		std::cout << "Unable to write fort.015 file" << std::endl;
	}
}


void Fort015::ReadFile()
{
	std::ifstream file (targetFile.toStdString().data());
	if (file.is_open() && isFullDomainFile)
	{
		innerBoundaryNodes.clear();
		outerBoundaryNodes.clear();

		std::string line;
		std::getline(file, line);
		std::stringstream(line) >> subdomainApproach;
		std::getline(file, line);
		std::stringstream(line) >> recordFrequency;
		std::getline(file, line);
		std::stringstream(line) >> enforceBN;

		if (subdomainApproach == 1)
		{
			std::getline(file, line);
			int numBN;
			std::stringstream(line) >> numBN;
			int currNode;
			for (int i=0; i<numBN; ++i)
			{
				file >> currNode;
				boundaryNodes.insert(currNode);
			}
		}
		else if (subdomainApproach == 2)
		{
			std::getline(file, line);
			int numOuterBN;
			std::stringstream(line) >> numOuterBN;
			int currNode;
			for (int i=0; i<numOuterBN; ++i)
			{
				file >> currNode;
				outerBoundaryNodes.insert(currNode);
			}
			std::getline(file, line);
			std::getline(file, line);
			int numInnerBN;
			std::stringstream(line) >> numInnerBN;
			for (int i=0; i<numInnerBN; ++i)
			{
				file >> currNode;
				innerBoundaryNodes.insert(currNode);
			}
		}

		file.close();
	}
	else if (file.is_open() && !isFullDomainFile)
	{
		std::string line;
		std::getline(file, line);
		std::getline(file, line);
		file >> subdomainApproach;
		file.close();
	}
}
