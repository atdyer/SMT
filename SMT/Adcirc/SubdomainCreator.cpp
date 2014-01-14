#include "SubdomainCreator.h"

SubdomainCreator::SubdomainCreator() :
	bnList(0),
	fort015Full(0),
	fort015Sub(0),
	fullDomain(0),
	innerBoundaryNodes(),
	outerBoundaryNodes(),
	py140(0),
	py141(0),
	projectFile(0),
	selectedElements(),
	selectedNodes(),
	subdomainName()
{
}


SubdomainCreator::~SubdomainCreator()
{
	if (bnList)
		delete bnList;
	if (fort015Full)
		delete fort015Full;
	if (fort015Sub)
		delete fort015Sub;
	if (py140)
		delete py140;
	if (py141)
		delete py141;
}


bool SubdomainCreator::CreateSubdomain(QString newName,
					   ProjectFile *projFile,
					   QString targetDir,
					   FullDomain *fDomain,
					   int version,
					   int recordFrequency)
{
	fullDomain = fDomain;
	projectFile = projFile;
	subdomainName = newName;

	// Make sure target directory exists
	if (!QDir(targetDir).exists())
	{
		QDir().mkdir(targetDir);
		std::cout << QDir(targetDir).exists() << std::endl;
	} else {
		if (CheckForExistingSubdomainFiles(targetDir))
		{
			if (!WarnSubdomainFilesExist(targetDir))
				return false;
		}
	}

	if (fullDomain && projectFile && !targetDir.isEmpty() && projectFile->AddSubdomain(subdomainName))
	{
		// Set the subdomain's target directory
		projectFile->SetSubDomainDirectory(subdomainName, targetDir);

		// Get the full domain fort.015 file
		if (fort015Full)
		{
			fort015Full->WriteFile();
			delete fort015Full;
		}
		fort015Full = new Fort015(projectFile);

		// Check for version and record frequency compatibility
		int fullVersion = fort015Full->GetSubdomainApproach();
		int fullFreq = fort015Full->GetRecordFrequency();
		if (fullVersion != 0 && fullVersion != version)
		{
			// Throw version mismatch warning
		}
		if (fullFreq != 0 && fullFreq != recordFrequency)
		{
			// Throw record frequency mismatch warning
		}
		fort015Full->SetSubdomainApproach(version);
		fort015Full->SetRecordFrequency(recordFrequency);

		// Get the subdomain fort.015
		if (fort015Sub)
		{
			fort015Sub->WriteFile();
			delete fort015Sub;
		}
		fort015Sub = new Fort015(subdomainName, projectFile);
		fort015Sub->SetSubdomainApproach(version);
		fort015Sub->SetRecordFrequency(recordFrequency);
		fort015Sub->WriteFile();


		// Get elements (and nodes) that are selected in the full domain
		selectedElements = fullDomain->GetSelectedElements();
		FindUniqueNodes();

		// Map the subdomain nodes/elements to the full domain nodes/elements
		MapOldToNewNodes();
		MapOldToNewElements();

		// Get the boundaries of the selected elements, write the bnlist.14 file,
		// and add the nodes to the full fort.015 file
		innerBoundaryNodes = fullDomain->GetInnerBoundaryNodes();
		outerBoundaryNodes = fullDomain->GetOuterBoundaryNodes();
		FindBoundaries(version);
		if (version == 1)
			fort015Full->AddBoundaryNodes(outerBoundaryNodes);
		else if (version == 2)
		{
			fort015Full->AddInnerBoundaryNodes(innerBoundaryNodes);
			fort015Full->AddOuterBoundaryNodes(outerBoundaryNodes);
		}
		fort015Full->WriteFile();

		// Write new fort.14 file
		return WriteFort14();
	}
	return false;
}


bool SubdomainCreator::CheckForExistingSubdomainFiles(QString targetDir)
{
	if (QFile(targetDir + QDir::separator() + "fort.14").exists())
		return true;
	if (QFile(targetDir + QDir::separator() + "fort.015").exists())
		return true;
	if (QFile(targetDir + QDir::separator() + "bnlist.14").exists())
		return true;
	return false;
}


void SubdomainCreator::FindBoundaries(int version)
{
	bnList = new BNList14(subdomainName, projectFile);
	bnList->SetSubdomainVersion(version);

	if (bnList && py140)
	{
//		BoundarySearch boundarySearch;
//		Boundaries selectedBoundaries = boundarySearch.FindBoundaries(selectedElements);

		std::vector<unsigned int> innerBoundaries;
		std::vector<unsigned int> outerBoundaries;

		for (std::vector<unsigned int>::iterator it=innerBoundaryNodes.begin();
		     it != innerBoundaryNodes.end();
		     ++it)
		{
			innerBoundaries.push_back(py140->ConvertOldToNew(*it));
		}

		for (std::vector<unsigned int>::iterator it=outerBoundaryNodes.begin();
		     it != outerBoundaryNodes.end();
		     ++it)
		{
			outerBoundaries.push_back(py140->ConvertOldToNew(*it));
		}

		bnList->SetInnerBoundaryNodes(innerBoundaries);
		bnList->SetOuterBoundaryNodes(outerBoundaries);
		bnList->SaveFile();
	}
}


void SubdomainCreator::FindUniqueNodes()
{
	std::set<Node*> selectedSet;
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
	{
		currElement = *it;
		selectedSet.insert(currElement->n1);
		selectedSet.insert(currElement->n2);
		selectedSet.insert(currElement->n3);
	}

	selectedNodes.reserve(selectedSet.size());
	for (std::set<Node*>::iterator it=selectedSet.begin(); it != selectedSet.end(); ++it)
	{
		selectedNodes.push_back(*it);
	}
}


void SubdomainCreator::MapOldToNewElements()
{
	py141 = new Py141(subdomainName, projectFile);

	if (selectedElements.size())
	{
		Element *currElement = 0;
		unsigned int nextNewElement = 1;
		std::map<unsigned int, unsigned int> oldToNew;
		for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				oldToNew[currElement->elementNumber] = nextNewElement++;
			}
		}
		py141->SetOldToNew(oldToNew);
		py141->SaveFile();
	}
}


void SubdomainCreator::MapOldToNewNodes()
{
	py140 = new Py140(subdomainName, projectFile);

	if (selectedNodes.size())
	{
		Node *currNode = 0;
		unsigned int nextNewNode = 1;
		std::map<unsigned int, unsigned int> oldToNew;
		for (std::vector<Node*>::iterator it=selectedNodes.begin(); it != selectedNodes.end(); ++it)
		{
			currNode = *it;
			if (currNode)
			{
				oldToNew[currNode->nodeNumber] = nextNewNode++;
			}
		}
		py140->SetOldToNew(oldToNew);
		py140->SaveFile();
	}
}


bool SubdomainCreator::WriteFort14()
{
	QString targetPath = projectFile->GetSubDomainDirectory(subdomainName) + QDir::separator() + "fort.14";
	std::ofstream fort14File (targetPath.toStdString().data());
	if (fort14File.is_open() && py140 && py141)
	{
		// Write title line
		fort14File << subdomainName.toStdString().data() << "\n";

		// Write info line
		fort14File << selectedElements.size() << " " << selectedNodes.size() << "\n";

		// Write nodes
		Node *currNode = 0;
		for (std::vector<Node*>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); ++it)
		{
			currNode = *it;
			if (currNode)
			{
				fort14File << "\t" <<
					      py140->ConvertOldToNew(currNode->nodeNumber) << "\t" <<
					      currNode->xDat << "\t" <<
					      currNode->yDat << "\t" <<
					      currNode->zDat << "\n";
			}
		}

		// Write elements
		Element *currElement = 0;
		for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				fort14File << py141->ConvertOldToNew(currElement->elementNumber) << "\t3\t" <<
					      py140->ConvertOldToNew(currElement->n1->nodeNumber) << "\t" <<
					      py140->ConvertOldToNew(currElement->n2->nodeNumber) << "\t" <<
					      py140->ConvertOldToNew(currElement->n3->nodeNumber) << "\n";
			}
		}

		// Write boundaries
		fort14File << "0\t!no. of open boundary segments\n";
		fort14File << "0\t!no. of open boundary nodes\n";
		fort14File << "0\t!no. of land boundary segments\n";
		fort14File << "0\t!no. of land boundary nodes\n";

		// Close the file
		fort14File.close();

		projectFile->SetSubDomainFort14(subdomainName, targetPath);

		return true;
	} else {
		return false;
	}
}


bool SubdomainCreator::WarnSubdomainFilesExist(QString targetDir)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("The directory " + targetDir + " already contains ADCIRC files. Would you like to overwrite them?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	switch (msgBox.exec())
	{
		case QMessageBox::Yes:
			/* Overwrite the file */
			return true;
		case QMessageBox::No:
			/* Do not overwrite the file */
			return false;
		default:
			/* Should never be reached */
			return false;
	}
}
