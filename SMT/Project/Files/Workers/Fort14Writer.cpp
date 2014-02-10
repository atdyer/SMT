#include "Fort14Writer.h"

Fort14Writer::Fort14Writer(QString fileLoc,
			   std::vector<Node> *nodeList,
			   std::vector<Element> *elementList,
			   QObject *parent) :

	QObject(parent),
	currentProgress(0),
	elements(elementList),
	nodes(nodeList),
	targetFile(fileLoc)
{
}


void Fort14Writer::SaveFile()
{
	emit FinishedWriting(WriteFile());
}


/**
 * @brief Writes nodal data to the fort.14 file
 * @return true if write was successful
 * @return false if write was unsuccessful
 */
bool Fort14Writer::WriteFile()
{
	if (QFile(targetFile).exists())
	{
		// Create temporary file
		QString tempFilePath = targetFile + "_tmp";
		std::ofstream tempFile (tempFilePath.toStdString().data());
		std::ifstream oldFile (targetFile.toStdString().data());

		std::cout << "Writing to temporary file: " << tempFilePath.toStdString().data() << std::endl;

		if (tempFile.is_open() && oldFile.is_open())
		{
			// Copy header from original file
			std::string currLine;
			std::getline(oldFile, currLine);
			tempFile << currLine.data() << "\n";
			std::getline(oldFile, currLine);
			tempFile << currLine.data() << "\n";

			// Write nodes
			for (std::vector<Node>::iterator it = nodes->begin(); it != nodes->end(); ++it)
			{
				tempFile << "\t" <<
					      (*it).nodeNumber << "\t" <<
					      (*it).xDat << "\t" <<
					      (*it).yDat << "\t" <<
					      (*it).zDat << "\n";
			}

			// Skip nodes in existing file
			for (unsigned int i=0; i<nodes->size(); ++i)
				std::getline(oldFile, currLine);

			// Copy everything else
			while (oldFile.good())
			{
				std::getline(oldFile, currLine);
				tempFile << currLine.data() << "\n";
			}

			// Overwrite old file
			oldFile.close();
			tempFile.close();
			remove(targetFile.toStdString().data());
			rename(tempFilePath.toStdString().data(), targetFile.toStdString().data());

			return true;
		} else {
			std::cout << "Error opening files in Fort14Writer" << std::endl;
			return false;
		}
	} else {
		return false;
	}
}
