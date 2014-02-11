#include "Fort13.h"

/*
 * Default empty constructor
 */
Fort13::Fort13(QObject *parent) :
	QObject(parent),
	targetFile(),
	projectFile(0),
	domainName(),
	isFullDomain(false),
	headerLine(),
	numNodes(0),
	numAttributes(0),
	attributes()
{
}


/*
 * Full Domain constructor
 */
Fort13::Fort13(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	targetFile(),
	projectFile(projectFile),
	domainName(),
	isFullDomain(true),
	headerLine(),
	numNodes(0),
	numAttributes(0),
	attributes()
{
	if (projectFile)
	{
		targetFile = projectFile->GetFullDomainFort13();
	}
}


/*
 * Subdomain constructor
 */
Fort13::Fort13(QString domainName, ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	targetFile(),
	projectFile(projectFile),
	domainName(domainName),
	isFullDomain(false),
	headerLine(),
	numNodes(0),
	numAttributes(0),
	attributes()
{
	if (projectFile)
	{
		targetFile = projectFile->GetSubDomainFort13(domainName);
		if (targetFile.isEmpty())
		{
			// No fort.13 file is specified for the subdomain yet.
			// Check to see if the full domain has a fort.13 file and if it does,
			// the subdomain is going to need one as well
			QString fullDomainCheck = projectFile->GetFullDomainFort13();
			if (!fullDomainCheck.isEmpty())
			{
				QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
				if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
				{
					targetFile = targetDirectory + QDir::separator() + "fort.13";
					projectFile->SetSubDomainFort13(domainName, targetFile);
				}
			}
		}
	}
}


QString Fort13::GetHeaderLine()
{
	if (headerLine.isEmpty())
		ReadFile();
	return headerLine;
}


unsigned int Fort13::GetNumNodes()
{
	if (numNodes == 0)
		ReadFile();
	return numNodes;
}


unsigned int Fort13::GetNumAttributes()
{
	if (numAttributes == 0)
		ReadFile();
	return numAttributes;
}


std::vector<NodalAttribute> Fort13::GetNodalAttributes()
{
	if (attributes.empty())
		ReadFile();
	return attributes;
}


void Fort13::SaveFile()
{
	if (!isFullDomain)
		WriteFile();
}


void Fort13::SetHeaderLine(QString newLine)
{
	if (!isFullDomain)
		headerLine = newLine;
}


void Fort13::SetNumNodes(unsigned int newNumNodes)
{
	if (!isFullDomain)
		numNodes = newNumNodes;
}


/**
 * @brief Sets the nodal attributes of a subdomain
 *
 * The nodal attributes of the subdomain are carved from the full domain. Any subdomain
 * nodes that have non-default attribute values are put into the subdomain fort.13 file
 *
 * @param newAttributes The list of full domain attributes
 * @param py140 The py.140 file for converting from full to subdomain node numbers
 */
void Fort13::SetNodalAttributes(std::vector<NodalAttribute> newAttributes, Py140 *py140)
{
	if (!isFullDomain)
	{

	}
}


void Fort13::ReadFile()
{

}


void Fort13::WriteFile()
{

}
