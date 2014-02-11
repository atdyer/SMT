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
	if (!isFullDomain && py140)
	{
		// Loop through the full domain attributes
		for (std::vector<NodalAttribute>::iterator currAttribute = newAttributes.begin();
		     currAttribute != newAttributes.end();
		     ++currAttribute)
		{
			// Create the corresponding subdomain attribute
			NodalAttribute newAttribute;
			newAttribute.attributeName = (*currAttribute).attributeName;
			newAttribute.units = (*currAttribute).units;
			newAttribute.valuesPerNode = (*currAttribute).valuesPerNode;
			newAttribute.defaultValues = (*currAttribute).defaultValues;

			// Loop through the nodes of the full domain attribute
			for (std::vector<QString>::iterator currNodeLine = (*currAttribute).nonDefaultLines.begin();
			     currNodeLine != (*currAttribute).nonDefaultLines.end();
			     ++currNodeLine)
			{
				// Split the string and retrieve the node number
				QStringList splitString = (*currNodeLine).split(QRegExp("\\s+"));
				if (!splitString.empty())
				{
					unsigned int nodeNumber = splitString.at(0).toUInt();

					// If the node number has a subdomain counterpart, it needs to be added
					// to the subdomain fort.13 file
					if (py140->HasOldNode(nodeNumber))
					{
						// Convert to the subdomain node number
						unsigned int subdomainNodeNumber = py140->ConvertOldToNew(nodeNumber);

						// Build the text line for the subdomain fort.13
						QString newLine = QString::number(subdomainNodeNumber);
						for (int i=1; i<splitString.size(); ++i)
						{
							newLine += "\t";
							newLine += splitString.at(i);
						}

						// Add the text line to the subdomain attribute
						newAttribute.nonDefaultLines.push_back(newLine);
					}
				}
			}
		}

		// Set the number of nodes
		numNodes = py140->GetNumNodes();

		// Set the number of attributes
		numAttributes = attributes.size();
	}
}


void Fort13::ReadFile()
{
	if (!targetFile.isEmpty())
	{
		attributes.clear();

		std::ifstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			std::string currentLine;

			// Read the header line
			std::getline(file, currentLine);
			headerLine = QString(currentLine.data());

			// Read the number of nodes line
			std::getline(file, currentLine);
			std::stringstream(currentLine) >> numNodes;

			// Read the number of attributes line
			std::getline(file, currentLine);
			std::stringstream(currentLine) >> numAttributes;

			// Read the attribute properties
			for (unsigned int i=0; i<numAttributes; ++i)
			{
				NodalAttribute newAttribute;

				// Read the attribute name
				std::getline(file, currentLine);
				newAttribute.attributeName = QString(currentLine.data());

				// Read the attribute units
				std::getline(file, currentLine);
				newAttribute.units = QString(currentLine.data());

				// Read the number of values per node for the attribute
				std::getline(file, currentLine);
				newAttribute.valuesPerNode = QString(currentLine.data());

				// Read the default values for the attribute
				std::getline(file, currentLine);
				newAttribute.defaultValues = QString(currentLine.data());

				// Add the new attribute to the list of attributes
				attributes.push_back(newAttribute);
			}

			// Read non-default values for each attribute
			for (std::vector<NodalAttribute>::iterator currAttribute = attributes.begin();
			     currAttribute != attributes.end();
			     ++currAttribute)
			{
				// Skip the attribute name line
				std::getline(file, currentLine);

				// Get the number of non-default nodes
				int numNonDefault;
				std::getline(file, currentLine);
				std::stringstream(currentLine) >> numNonDefault;

				// Get all of the non-default values
				for (int i=0; i<numNonDefault; ++i)
				{
					std::getline(file, currentLine);
					(*currAttribute).nonDefaultLines.push_back(QString(currentLine.data()));
				}
			}

			// Close the file
			file.close();
		}
	}
}


void Fort13::WriteFile()
{
	// Only allow modification of subdomain files
	if (!isFullDomain && !targetFile.isEmpty())
	{
		std::ofstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			// Write the header line
			file << headerLine.toStdString().data() << '\n';

			// Write the number of nodes
			file << numNodes << '\n';

			// Write the number of attributes
			file << numAttributes << '\n';

			// Write the attribute properties
			for (std::vector<NodalAttribute>::iterator currAttribute = attributes.begin();
			     currAttribute != attributes.end();
			     ++currAttribute)
			{
				// Write the attribute name
				file << (*currAttribute).attributeName.toStdString().data() << '\n';

				// Write the attribute units
				file << (*currAttribute).units.toStdString().data() << '\n';

				// Write the number of values per node
				file << (*currAttribute).valuesPerNode.toStdString().data() << '\n';

				// Write the default attribute values
				file << (*currAttribute).defaultValues.toStdString().data() << '\n';
			}

			// Write the non-default nodal attribute values
			for (std::vector<NodalAttribute>::iterator currAttribute = attributes.begin();
			     currAttribute != attributes.end();
			     ++currAttribute)
			{
				// Write the attribute name
				file << (*currAttribute).attributeName.toStdString().data() << '\n';

				// Write the number of non-default values
				file << (*currAttribute).nonDefaultLines.size() << '\n';

				// Write each of the non-default value lines
				for (std::vector<QString>::iterator currLine = (*currAttribute).nonDefaultLines.begin();
				     currLine != (*currAttribute).nonDefaultLines.end();
				     ++currLine)
				{
					file << (*currLine).toStdString().data();
				}
			}

			// Close the file
			file.close();
		}
	}
}
