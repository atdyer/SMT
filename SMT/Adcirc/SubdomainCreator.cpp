#include "SubdomainCreator.h"

SubdomainCreator::SubdomainCreator() :
//	bnList(0),
//	fort015Full(0),
//	fort015Sub(0),
	fullDomain(0),
//	innerBoundaryNodes(),
//	outerBoundaryNodes(),
//	py140(0),
//	py141(0),
	projectFile(0),
//	selectedElements(),
//	selectedNodes(),
	subdomainName()
{
}


SubdomainCreator::~SubdomainCreator()
{
//	if (bnList)
//		delete bnList;
//	if (fort015Full)
//		delete fort015Full;
//	if (fort015Sub)
//		delete fort015Sub;
//	if (py140)
//		delete py140;
//	if (py141)
//		delete py141;
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

	// Create the subdomain
	if (version == 1)
	{
		return CreateSubdomainVersion1(targetDir, recordFrequency);
	}
	else if (version == 2)
	{
		return CreateSubdomainVersion2(targetDir, recordFrequency);
	}

	return false;

//	// Make sure target directory exists
//	if (!QDir(targetDir).exists())
//	{
//		QDir().mkdir(targetDir);
//		std::cout << QDir(targetDir).exists() << std::endl;
//	} else {
//		if (CheckForExistingSubdomainFiles(targetDir))
//		{
//			if (!WarnSubdomainFilesExist(targetDir))
//				return false;
//		}
//	}

//	if (fullDomain && projectFile && !targetDir.isEmpty() && projectFile->AddSubdomain(subdomainName))
//	{
//		// Set the subdomain's target directory
//		projectFile->SetSubDomainDirectory(subdomainName, targetDir);

//		// Get the full domain fort.015 file
//		if (fort015Full)
//		{
//			fort015Full->WriteFile();
//			delete fort015Full;
//		}
//		fort015Full = new Fort015(projectFile);

//		// Check for version and record frequency compatibility
//		int fullVersion = fort015Full->GetSubdomainApproach();
//		int fullFreq = fort015Full->GetRecordFrequency();
//		if (fullVersion != 0 && fullVersion != version)
//		{
//			// Throw version mismatch warning
//		}
//		if (fullFreq != 0 && fullFreq != recordFrequency)
//		{
//			// Throw record frequency mismatch warningtar
//		}
//		fort015Full->SetSubdomainApproach(version);
//		fort015Full->SetRecordFrequency(recordFrequency);

//		// Get the subdomain fort.015
//		if (fort015Sub)
//		{
//			fort015Sub->WriteFile();
//			delete fort015Sub;
//		}
//		fort015Sub = new Fort015(subdomainName, projectFile);
//		fort015Sub->SetSubdomainApproach(version);
//		fort015Sub->SetRecordFrequency(recordFrequency);
//		fort015Sub->WriteFile();


//		// Get elements (and nodes) that are selected in the full domain
//		selectedElements = fullDomain->GetSelectedElements();
//		FindUniqueNodes();

//		// Map the subdomain nodes/elements to the full domain nodes/elements
//		MapOldToNewNodes();
//		MapOldToNewElements();

//		// Get the boundaries of the selected elements, write the bnlist.14 file,
//		// and add the nodes to the full fort.015 file
//		innerBoundaryNodes = fullDomain->GetInnerBoundaryNodes();
//		outerBoundaryNodes = fullDomain->GetOuterBoundaryNodes();
//		FindBoundaries(version);
//		if (version == 1)
//			fort015Full->AddBoundaryNodes(outerBoundaryNodes);
//		else if (version == 2)
//		{
//			fort015Full->AddInnerBoundaryNodes(innerBoundaryNodes);
//			fort015Full->AddOuterBoundaryNodes(outerBoundaryNodes);
//		}
//		fort015Full->WriteFile();

//		// Write new fort.14 file
//		return WriteFort14();
//	}
//	return false;
}


/**
 * @brief Creates a version 1 subdomain
 */
bool SubdomainCreator::CreateSubdomainVersion1(QString targetDir, int recordFrequency)
{
	// fullDomain set by SubdomainCreator::CreateSubdomain()
	// projectFile set by SubdomainCreator::CreateSubdomain()
	// subdomainName set by SubdomainCreator::CreateSubdomain()


	// Make sure the target directory (it will always be the name
	// of the subdomain) exists.
	QString newDirPath = targetDir;
	if (QDir(newDirPath).exists())
	{
		// The directory exists. Check for ADCIRC files.
		if (CheckForExistingSubdomainFiles(newDirPath))
		{
			// ADCIRC files exist in the directory. Ask the user
			// if they'd like to overwrite.
			if (!WarnSubdomainFilesExist(newDirPath))
				return false;
		}
	} else {
		// The directory doesn't exist, so create it.
		QDir().mkdir(newDirPath);
	}

	// Create the subdomain in the project file
	if (!projectFile->AddSubdomain(subdomainName))
		return false;
	projectFile->SetSubDomainDirectory(subdomainName, newDirPath);


	// Start creating the subdomain by getting the fort.015 file from the full domain
	// Checks are performed here to ensure that the subdomain version/record frequency
	// are matched between the full and subdomain
	Fort015 *fort015Full = GetFullDomainFort015(1, recordFrequency);

	if (!fort015Full)
	{
		std::cout << "Error getting full domain fort.015 file" << std::endl;
		return false;
	}


	// Create the subdomain fort.015 file
	Fort015 *fort015Sub = new Fort015(subdomainName, projectFile);
	if (fort015Sub)
	{
		// The version 1 subdomain has a very simple fort.015 file.
		// All we need to do is set the subdomain version and record
		// frequency, and we are able to write the file and close it.
		fort015Sub->SetSubdomainApproach(1);
		fort015Sub->SetRecordFrequency(recordFrequency);
		fort015Sub->WriteFile();
		delete fort015Sub;
	} else {
		std::cout << "Error creating the subdomain fort.015 file" << std::endl;
		return false;
	}


	// Get the selected elements from the full domain and determine the selected
	// set of nodes.
	if (!fullDomain)
	{
		std::cout << "Error accessing the full domain" << std::endl;
		return false;
	}
	std::vector<Element*> selectedElements = fullDomain->GetSelectedElements();
	std::vector<Node*> selectedNodes = GetSelectedNodes(selectedElements);


	// Map the subdomain nodes/elements to the full domain nodes/elements
	// This process creates the py.140 and py.141 files
	Py140 *py140 = CreatePy140(selectedNodes);
	Py141 *py141 = CreatePy141(selectedElements);
	if (!py140)
	{
		std::cout << "Error creating the py.140 file for the new subdomain" << std::endl;
		return false;
	}
	if (!py141)
	{
		std::cout << "Error creating the py.141 file for the new subdomain" << std::endl;
		return false;
	}


	// Get the boundary nodes from the selection and create the bnlist.14 file
	std::vector<unsigned int> boundaryNodes = fullDomain->GetOuterBoundaryNodes();
	bool bnListCreated = CreateBNListVersion1(boundaryNodes, py140);
	if (!bnListCreated)
	{
		std::cout << "Error creating the bnlist.14 file for the new subdomain" << std::endl;
		return false;
	}


	// Add the boundary nodes to the full domain fort.015 file
	fort015Full->AddBoundaryNodes(boundaryNodes);
	fort015Full->WriteFile();


	// Create a fort.13 file for the subdomain (if it's needed)
	Fort13 *fort13 = CreateFort13(py140);
	if (fort13)
		std::cout << "Subdomain fort.13 file created" << std::endl;


	// Create a fort.14 file for the subdomain
	if (!CreateFort14(selectedNodes, selectedElements, py140, py141))
	{
		std::cout << "Error creating the fort.14 file for the new subdomain" << std::endl;
		return false;
	}

	return true;
}


bool SubdomainCreator::CreateBNListVersion1(std::vector<unsigned int> boundaryNodes, Py140 *py140)
{
	// Version 1 boundary list only needs outer boundaries
	BNList14 *file = new BNList14(subdomainName, projectFile);

	if (file && py140)
	{
		file->SetSubdomainVersion(1);

		std::vector<unsigned int> boundaries;
		for (std::vector<unsigned int>::iterator it=boundaryNodes.begin(); it != boundaryNodes.end(); ++it)
		{
			boundaries.push_back(py140->ConvertOldToNew(*it));
		}

		file->SetOuterBoundaryNodes(boundaries);
		file->SaveFile();
		delete file;
		return true;
	}
	return false;
}


bool SubdomainCreator::CreateSubdomainVersion2(QString targetDir, int recordFrequency)
{
	// fullDomain set by SubdomainCreator::CreateSubdomain()
	// projectFile set by SubdomainCreator::CreateSubdomain()
	// subdomainName set by SubdomainCreator::CreateSubdomain()


	// Make sure the target directory (it will always be the name
	// of the subdomain) exists.
	QString newDirPath = targetDir + QDir::separator() + subdomainName;
	std::cout << "Target Directory: " << newDirPath.toStdString().data() << std::endl;
	if (QDir(newDirPath).exists())
	{
		// The directory exists. Check for ADCIRC files.
		if (CheckForExistingSubdomainFiles(newDirPath))
		{
			// ADCIRC files exist in the directory. Ask the user
			// if they'd like to overwrite.
			if (!WarnSubdomainFilesExist(newDirPath))
				return false;
		}
	} else {
		// The directory doesn't exist, so create it.
		QDir().mkdir(newDirPath);
	}

	// Create the subdomain in the project file
	if (!projectFile->AddSubdomain(subdomainName))
		return false;
	projectFile->SetSubDomainDirectory(subdomainName, newDirPath);


	// Start creating the subdomain by getting the fort.015 file from the full domain
	// Checks are performed here to ensure that the subdomain version/record frequency
	// are matched between the full and subdomain
	Fort015 *fort015Full = GetFullDomainFort015(2, recordFrequency);

	if (!fort015Full)
	{
		std::cout << "Error getting full domain fort.015 file" << std::endl;
		return false;
	}


	// Create the subdomain fort.015 file
	Fort015 *fort015Sub = new Fort015(subdomainName, projectFile);
	if (fort015Sub)
	{
		// The version 1 subdomain has a very simple fort.015 file.
		// All we need to do is set the subdomain version and record
		// frequency, and we are able to write the file and close it.
		fort015Sub->SetSubdomainApproach(1);
		fort015Sub->SetRecordFrequency(recordFrequency);
		fort015Sub->WriteFile();
		delete fort015Sub;
	} else {
		std::cout << "Error creating the subdomain fort.015 file" << std::endl;
		return false;
	}


	// Get the selected elements from the full domain and determine the selected
	// set of nodes.
	if (!fullDomain)
	{
		std::cout << "Error accessing the full domain" << std::endl;
		return false;
	}
	std::vector<Element*> selectedElements = fullDomain->GetSelectedElements();
	std::vector<Node*> selectedNodes = GetSelectedNodes(selectedElements);


	// Map the subdomain nodes/elements to the full domain nodes/elements
	// This process creates the py.140 and py.141 files
	Py140 *py140 = CreatePy140(selectedNodes);
	Py141 *py141 = CreatePy141(selectedElements);
	if (!py140)
	{
		std::cout << "Error creating the py.140 file for the new subdomain" << std::endl;
		return false;
	}
	if (!py141)
	{
		std::cout << "Error creating the py.141 file for the new subdomain" << std::endl;
		return false;
	}


	// Get the boundary nodes from the selection and create the bnlist.14 file
	std::vector<unsigned int> outerBoundaries = fullDomain->GetOuterBoundaryNodes();
	std::vector<unsigned int> innerBoundaries = fullDomain->GetInnerBoundaryNodes();
	bool bnListCreated = CreateBNListVersion2(innerBoundaries, outerBoundaries, py140);
	if (!bnListCreated)
	{
		std::cout << "Error creating the bnlist.14 file for the new subdomain" << std::endl;
		return false;
	}


	// Add the boundary nodes to the full domain fort.015 file
	fort015Full->AddOuterBoundaryNodes(outerBoundaries);
	fort015Full->AddInnerBoundaryNodes(innerBoundaries);
	fort015Full->WriteFile();


	// Create a fort.13 file for the subdomain (if it's needed)
	Fort13 *fort13 = CreateFort13(py140);
	if (fort13)
		std::cout << "Subdomain fort.13 file created" << std::endl;


	// Create a fort.14 file for the subdomain
	if (!CreateFort14(selectedNodes, selectedElements, py140, py141))
	{
		std::cout << "Error creating the fort.14 file for the new subdomain" << std::endl;
		return false;
	}

	return true;
}


bool SubdomainCreator::CreateBNListVersion2(std::vector<unsigned int> innerBoundaryNodes, std::vector<unsigned int> outerBoundaryNodes, Py140 *py140)
{
	// Version 2 boundary list needs inner and outer boundaries
	BNList14 *file = new BNList14(subdomainName, projectFile);

	if (file && py140)
	{
		file->SetSubdomainVersion(2);

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

		file->SetInnerBoundaryNodes(innerBoundaries);
		file->SetOuterBoundaryNodes(outerBoundaries);
		file->SaveFile();

		delete file;
		return true;
	}
	return false;
}


Fort015* SubdomainCreator::GetFullDomainFort015(int version, int recordFrequency)
{
	//
	// Retrieve the full domain fort.015. If one does not exist, create one
	// and set the version and record frequency. If one does exist,
	// check the requested version and record frequency against the existing
	// values. If there is a mismatch, warn the user and ask them if
	// they want to continue. If they do not want to continue, return 0.
	//

	if (projectFile)
	{
		// Get the fort.015 file path from the project file
		QString fileLoc = projectFile->GetFullDomainFort015();

		// Create the fort.015 file object
		Fort015 *fullFort015 = new Fort015(projectFile);
		if (fullFort015)
		{
			// If the file path existed before creating the object,
			// check for version mismatch
			if (fileLoc.isEmpty())
			{
				// File didn't exist, so create and return it
				fullFort015->SetSubdomainApproach(version);
				fullFort015->SetRecordFrequency(recordFrequency);
				fullFort015->WriteFile();
				return fullFort015;
			} else {
				// File exists, so check that the user requested the appropriate values
				int currentVersion = fullFort015->GetSubdomainApproach();
				int currentFrequency = fullFort015->GetRecordFrequency();
				if (currentVersion != version || currentFrequency != recordFrequency)
				{
					if (WarnFort015Mismatch(version, currentVersion,
								recordFrequency, currentFrequency))
						return fullFort015;
				} else {
					return fullFort015;
				}
			}
		}
	}

	// Unable to retrieve or create the full domain fort.015, so return 0
	return 0;
}


std::vector<Node*> SubdomainCreator::GetSelectedNodes(std::vector<Element*> selectedElements)
{
	//
	// Use a standard library set to find the unique set of selected nodes
	//
	std::set<Node*> selectedSet;
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
	{
		currElement = *it;
		if (currElement)
		{
			selectedSet.insert(currElement->n1);
			selectedSet.insert(currElement->n2);
			selectedSet.insert(currElement->n3);
		}
	}

	//
	// Transfer the items of the set into a vector
	//
	std::vector<Node*> selectedNodes;
	selectedNodes.reserve(selectedSet.size());
	for (std::set<Node*>::iterator it=selectedSet.begin(); it != selectedSet.end(); ++it)
	{
		selectedNodes.push_back(*it);
	}

	// Return the list of nodes
	return selectedNodes;
}


Fort13* SubdomainCreator::CreateFort13(Py140 *py140)
{
	// Create the full domain fort.13 object
	QString fullFort13 = projectFile->GetFullDomainFort13();
	if (!fullFort13.isEmpty())
	{
		Fort13 *full = new Fort13(projectFile);
		if (full && py140)
		{
			// Get the full domain attributes
			QString fullHdr = full->GetHeaderLine();
			std::vector<NodalAttribute> fullAttr = full->GetNodalAttributes();

			// Create the subdomain fort.13 object
			Fort13 *sub = new Fort13(subdomainName, projectFile);
			if (sub)
			{
				sub->SetHeaderLine(fullHdr);
				sub->SetNodalAttributes(fullAttr, py140);
				sub->SaveFile();
				delete full;
				return sub;
			}
			delete full;
		}
	}
	return 0;
}


bool SubdomainCreator::CreateFort14(std::vector<Node*> selectedNodes, std::vector<Element*> selectedElements, Py140 *py140, Py141 *py141)
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


Py140* SubdomainCreator::CreatePy140(std::vector<Node*> selectedNodes)
{
	Py140 *newFile = new Py140(subdomainName, projectFile);
	if (newFile)
	{
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
			newFile->SetOldToNew(oldToNew);
			newFile->SaveFile();
			return newFile;
		}
	}
	return 0;
}


Py141* SubdomainCreator::CreatePy141(std::vector<Element*> selectedElements)
{
	Py141 *newFile = new Py141(subdomainName, projectFile);
	if (newFile)
	{
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
			newFile->SetOldToNew(oldToNew);
			newFile->SaveFile();
			return newFile;
		}
	}
	return 0;
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


//void SubdomainCreator::FindBoundaries(int version)
//{
//	bnList = new BNList14(subdomainName, projectFile);
//	bnList->SetSubdomainVersion(version);

//	if (bnList && py140)
//	{
////		BoundarySearch boundarySearch;
////		Boundaries selectedBoundaries = boundarySearch.FindBoundaries(selectedElements);

//		std::vector<unsigned int> innerBoundaries;
//		std::vector<unsigned int> outerBoundaries;

//		for (std::vector<unsigned int>::iterator it=innerBoundaryNodes.begin();
//		     it != innerBoundaryNodes.end();
//		     ++it)
//		{
//			innerBoundaries.push_back(py140->ConvertOldToNew(*it));
//		}

//		for (std::vector<unsigned int>::iterator it=outerBoundaryNodes.begin();
//		     it != outerBoundaryNodes.end();
//		     ++it)
//		{
//			outerBoundaries.push_back(py140->ConvertOldToNew(*it));
//		}

//		bnList->SetInnerBoundaryNodes(innerBoundaries);
//		bnList->SetOuterBoundaryNodes(outerBoundaries);
//		bnList->SaveFile();
//	}
//}


//void SubdomainCreator::FindUniqueNodes()
//{
//	std::set<Node*> selectedSet;
//	Element *currElement = 0;
//	for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
//	{
//		currElement = *it;
//		selectedSet.insert(currElement->n1);
//		selectedSet.insert(currElement->n2);
//		selectedSet.insert(currElement->n3);
//	}

//	selectedNodes.reserve(selectedSet.size());
//	for (std::set<Node*>::iterator it=selectedSet.begin(); it != selectedSet.end(); ++it)
//	{
//		selectedNodes.push_back(*it);
//	}
//}


//void SubdomainCreator::MapOldToNewElements()
//{
//	py141 = new Py141(subdomainName, projectFile);

//	if (selectedElements.size())
//	{
//		Element *currElement = 0;
//		unsigned int nextNewElement = 1;
//		std::map<unsigned int, unsigned int> oldToNew;
//		for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
//		{
//			currElement = *it;
//			if (currElement)
//			{
//				oldToNew[currElement->elementNumber] = nextNewElement++;
//			}
//		}
//		py141->SetOldToNew(oldToNew);
//		py141->SaveFile();
//	}
//}


//void SubdomainCreator::MapOldToNewNodes()
//{
//	py140 = new Py140(subdomainName, projectFile);

//	if (selectedNodes.size())
//	{
//		Node *currNode = 0;
//		unsigned int nextNewNode = 1;
//		std::map<unsigned int, unsigned int> oldToNew;
//		for (std::vector<Node*>::iterator it=selectedNodes.begin(); it != selectedNodes.end(); ++it)
//		{
//			currNode = *it;
//			if (currNode)
//			{
//				oldToNew[currNode->nodeNumber] = nextNewNode++;
//			}
//		}
//		py140->SetOldToNew(oldToNew);
//		py140->SaveFile();
//	}
//}


//bool SubdomainCreator::WriteFort14()
//{
//	QString targetPath = projectFile->GetSubDomainDirectory(subdomainName) + QDir::separator() + "fort.14";
//	std::ofstream fort14File (targetPath.toStdString().data());
//	if (fort14File.is_open() && py140 && py141)
//	{
//		// Write title line
//		fort14File << subdomainName.toStdString().data() << "\n";

//		// Write info line
//		fort14File << selectedElements.size() << " " << selectedNodes.size() << "\n";

//		// Write nodes
//		Node *currNode = 0;
//		for (std::vector<Node*>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); ++it)
//		{
//			currNode = *it;
//			if (currNode)
//			{
//				fort14File << "\t" <<
//					      py140->ConvertOldToNew(currNode->nodeNumber) << "\t" <<
//					      currNode->xDat << "\t" <<
//					      currNode->yDat << "\t" <<
//					      currNode->zDat << "\n";
//			}
//		}

//		// Write elements
//		Element *currElement = 0;
//		for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); ++it)
//		{
//			currElement = *it;
//			if (currElement)
//			{
//				fort14File << py141->ConvertOldToNew(currElement->elementNumber) << "\t3\t" <<
//					      py140->ConvertOldToNew(currElement->n1->nodeNumber) << "\t" <<
//					      py140->ConvertOldToNew(currElement->n2->nodeNumber) << "\t" <<
//					      py140->ConvertOldToNew(currElement->n3->nodeNumber) << "\n";
//			}
//		}

//		// Write boundaries
//		fort14File << "0\t!no. of open boundary segments\n";
//		fort14File << "0\t!no. of open boundary nodes\n";
//		fort14File << "0\t!no. of land boundary segments\n";
//		fort14File << "0\t!no. of land boundary nodes\n";

//		// Close the file
//		fort14File.close();

//		projectFile->SetSubDomainFort14(subdomainName, targetPath);

//		return true;
//	} else {
//		return false;
//	}
//}


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
	return false;
}


bool SubdomainCreator::WarnFort015Mismatch(int requestedVersion, int actualVersion, int requestedFrequency, int actualFrequency)
{
	std::cout << "Warning: fort.015 mismatch" << std::endl;

	bool version = requestedVersion == actualVersion;
	bool frequency = requestedFrequency == actualFrequency;
	QMessageBox msg;
	msg.setWindowTitle("SMT");
	if (version && frequency)
	{
		msg.setText("The full domain fort.015 file already exists. The subdomain version is set to version " +
			    QString::number(actualVersion) + ", and the record frequency is set to " +
			    QString::number(actualFrequency) + ". Would you like to continue using these values?");
	}
	else if (version)
	{
		msg.setText("The full domain fort.015 file already exists. The subdomain version is set to version " +
			    QString::number(actualVersion) + ". Would you like to continue using this value?");
	}
	else if (frequency)
	{
		msg.setText("The full domain fort.015 file already exists. The record frequency is set to " +
			    QString::number(actualVersion) + ". Would you like to continue using this value?");
	}
	msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

	switch (msg.exec())
	{
		case QMessageBox::Ok:
			return true;
		case QMessageBox::Cancel:
			return false;
		default:
			return false;
	}
	return false;
}
