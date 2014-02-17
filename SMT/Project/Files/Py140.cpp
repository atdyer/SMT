#include "Py140.h"


/*
 * Default constructor
 */
Py140::Py140(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0),
	targetFile(),
	newToOldNodes(),
	oldToNewNodes()
{

}


/*
 * Subdomain constructor
 */
Py140::Py140(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile),
	targetFile(),
	newToOldNodes(),
	oldToNewNodes()
{
	if (projectFile && !domainName.isEmpty())
	{
		targetFile = projectFile->GetSubDomainPy140(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "py.140";
				std::cout << "Setting py.140 to: " << targetFile.toStdString().data() << std::endl;
				projectFile->SetSubDomainPy140(domainName, targetFile);
			} else {
				std::cout << "Subdomain directory does not exist" << std::endl;
				std::cout << targetDirectory.toStdString().data() << std::endl;
				// Throw a warning that the subdomain directory doesn't exist
			}
		} else {
			ReadFile();
		}
	}
}


void Py140::SaveFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		std::ofstream file (projectFile->GetSubDomainPy140(domainName).toStdString().data());
		if (file.is_open())
		{
			file << "new old " << newToOldNodes.size() << "\n";
			if (newToOldNodes.size() > 0)
			{
				for (std::map<unsigned int, unsigned int>::iterator it = newToOldNodes.begin(); it != newToOldNodes.end(); ++it)
				{
					file << it->first << " " << it->second << "\n";
				}
			}
			file.close();
		} else {
			std::cout << "Unable to write py.140: " << projectFile->GetSubDomainPy140(domainName).toStdString() << std::endl;
		}
		projectFile->SetSubDomainPy140(domainName, targetFile);
	}
}


void Py140::SetNewToOld(std::map<unsigned int, unsigned int> newMap)
{
	newToOldNodes = newMap;
	oldToNewNodes.clear();
	if (newToOldNodes.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = newToOldNodes.begin(); it != newToOldNodes.end(); ++it)
		{
			oldToNewNodes[it->second] = it->first;
		}
	}
}


void Py140::SetOldToNew(std::map<unsigned int, unsigned int> newMap)
{
	oldToNewNodes = newMap;
	newToOldNodes.clear();
	if (oldToNewNodes.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = oldToNewNodes.begin(); it != oldToNewNodes.end(); ++it)
		{
			newToOldNodes[it->second] = it->first;
		}
	}
}


unsigned int Py140::ConvertNewToOld(unsigned int newNum)
{
	if (!newToOldNodes.size())
		ReadFile();
	return newToOldNodes[newNum];
}


std::vector<unsigned int> Py140::ConvertNewToOld(std::vector<unsigned int> newVector)
{
	if (!newToOldNodes.size())
		ReadFile();
	std::vector<unsigned int> oldVals;
	for (std::vector<unsigned int>::iterator it = newVector.begin(); it != newVector.end(); ++it)
	{
		oldVals.push_back(newToOldNodes[*it]);
	}
	return oldVals;
}


unsigned int Py140::ConvertOldToNew(unsigned int oldNum)
{
	if (!oldToNewNodes.size())
		ReadFile();
	return oldToNewNodes[oldNum];
}


std::vector<unsigned int> Py140::ConvertOldToNew(std::vector<unsigned int> oldVector)
{
	if (!oldToNewNodes.size())
		ReadFile();
	std::vector<unsigned int> newVals;
	for (std::vector<unsigned int>::iterator it = oldVector.begin(); it != oldVector.end(); ++it)
	{
		newVals.push_back(oldToNewNodes[*it]);
	}
	return newVals;
}


QString	Py140::GetFilePath()
{
	if (projectFile && !domainName.isEmpty())
	{
		return projectFile->GetSubDomainPy140(domainName);
	}
	return QString();
}


std::map<unsigned int, unsigned int> Py140::GetNewToOld()
{
	if (!newToOldNodes.size())
		ReadFile();
	return newToOldNodes;
}


unsigned int Py140::GetNumNodes()
{
	return newToOldNodes.size();
}


std::map<unsigned int, unsigned int> Py140::GetOldToNew()
{
	if (!oldToNewNodes.size())
		ReadFile();
	return oldToNewNodes;
}


bool Py140::HasOldNode(unsigned int nodeNum)
{
	if (oldToNewNodes.count(nodeNum))
		return true;
	return false;
}


void Py140::ReadFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		QString targetFile = projectFile->GetSubDomainPy140(domainName);
		std::ifstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			newToOldNodes.clear();
			oldToNewNodes.clear();
			std::string line;
			std::getline(file, line);
			int currOld, currNew;
			while (std::getline(file, line))
			{
				std::stringstream(line) >> currNew >> currOld;
				newToOldNodes[currNew] = currOld;
				oldToNewNodes[currOld] = currNew;
			}
			file.close();
		}
	}
}
