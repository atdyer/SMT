#include "Fort14Reader.h"

Fort14Reader::Fort14Reader(QString fileLoc,
			   std::vector<Node> *nodeList,
			   std::vector<Element> *elementList,
			   std::vector<std::vector<unsigned int> > *elevationBoundaryList,
			   std::vector<std::vector<unsigned int> > *flowBoundaryList,
			   bool normalize,
			   QObject *parent) :
	QObject(parent),
	currProgress(0),
	elements(elementList),
	elevationBoundaries(elevationBoundaryList),
	flowBoundaries(flowBoundaryList),
	fullProgress(0),
	maxX(-99999.0),
	maxY(-99999.0),
	maxZ(-99999.0),
	minX(99999.0),
	minY(99999.0),
	minZ(99999.0),
	nodes(nodeList),
	normalizeCoordinates(normalize),
	targetFile(fileLoc)
{

}


Fort14Reader::~Fort14Reader()
{
	std::cout << "Deleting fort.14 reader" << std::endl;
}


void Fort14Reader::ReadFile()
{
	std::cout << "Reading on thread: " << this->thread() << std::endl;

	emit StartedReading();

	std::ifstream fort14 (targetFile.toStdString().data());

	if (fort14.is_open() && nodes && elements)
	{
		nodes->clear();
		elements->clear();

		std::string infoLine, line;
		int numElements, numNodes;

		std::getline(fort14, infoLine);
		std::getline(fort14, line);
		std::stringstream(line) >> numElements >> numNodes;

		emit FoundNumElements(numElements);
		emit FoundNumNodes(numNodes);

		fullProgress = numNodes + numElements;

		nodes->reserve(numNodes);
		elements->reserve(numElements);

		std::cout << "Reading nodes" << std::endl;

		if (ReadNodalData(numNodes, &fort14))
		{
			std::cout << "Reading elements" << std::endl;
			if (ReadElementalData(numElements, &fort14))
			{
//				std::cout << "Reading boundaries" << std::endl;
//				ReadBoundaries(&fort14);
				if (normalizeCoordinates)
				{
					std::cout << "Normalizing coordinates" << std::endl;
					NormalizeCoordinates();
				}
				emit FinishedReading();
			}
		}
		fort14.close();
	}

	emit FinishedReading();
}


Node* Fort14Reader::GetNode(unsigned int nodeNumber)
{
	if (nodes->size() > 0 && nodeNumber <= nodes->size() && nodeNumber == (*nodes)[nodeNumber-1].nodeNumber)
		return &(*nodes)[nodeNumber-1];
	else
		for (unsigned int i=0; i<nodes->size(); i++)
			if ((*nodes)[i].nodeNumber == nodeNumber)
				return &(*nodes)[i];
	return 0;
}


void Fort14Reader::NormalizeCoordinates()
{
	float midX = minX + (maxX - minX) / 2.0;
	float midY = minY + (maxY - minY) / 2.0;
	float max = fmax(maxX-minX, maxY-minY);
	for (std::vector<Node>::iterator currNode = (*nodes).begin(); currNode != (*nodes).end(); ++currNode)
	{
		(*currNode).normX = ((*currNode).x - midX)/max;
		(*currNode).normY = ((*currNode).y - midY)/max;
		(*currNode).normZ = (*currNode).z / (maxZ-minZ);
	}
}


void Fort14Reader::ReadBoundaries(std::ifstream *fileStream)
{
	int numEleBoundaries, numFlowBoundaries;
	int numEleNodes, numFlowNodes;
	int numSegmentNodes;
	unsigned int currNode;
	std::string currLine;
	std::getline(*fileStream, currLine);
	std::getline(*fileStream, currLine);
	std::stringstream(currLine) >> numEleBoundaries;
	std::getline(*fileStream, currLine);
	std::stringstream(currLine) >> numEleNodes;

	for (int i=0; i<numEleBoundaries; ++i)
	{
		std::getline(*fileStream, currLine);
		std::stringstream(currLine) >> numSegmentNodes;

		std::vector<unsigned int> currSegment;
		for (int j=0; j<numSegmentNodes; ++j)
		{
			std::getline(*fileStream, currLine);
			std::stringstream(currLine) >> currNode;
			currSegment.push_back(currNode);
		}
		elevationBoundaries->push_back(currSegment);
	}

	std::getline(*fileStream, currLine);
	std::getline(*fileStream, currLine);
	std::stringstream(currLine) >> numFlowBoundaries;
	std::getline(*fileStream, currLine);
	std::stringstream(currLine) >> numFlowNodes;

	for (int i=0; i<numFlowBoundaries; ++i)
	{
		unsigned int boundaryType;
		std::getline(*fileStream, currLine);
		std::stringstream(currLine) >> numSegmentNodes >> boundaryType;

		std::vector<unsigned int> currSegment;
		currSegment.push_back(boundaryType);
		for (int j=0; j<numSegmentNodes; ++j)
		{
			std::getline(*fileStream, currLine);
			std::stringstream(currLine) >> currNode;
			currSegment.push_back(currNode);
		}
		flowBoundaries->push_back(currSegment);
	}
}


bool Fort14Reader::ReadElementalData(int numElements, std::ifstream *fileStream)
{
	Element currElement;
	unsigned int trash, n1, n2, n3;
	std::string currLine;
	for (int i=0; i<numElements; ++i)
	{
		std::getline(*fileStream, currLine);
		std::stringstream(currLine) >> currElement.elementNumber >>
					       trash >>
					       n1 >>
					       n2 >>
					       n3;
		currElement.n1 = GetNode(n1);
		currElement.n2 = GetNode(n2);
		currElement.n3 = GetNode(n3);
		elements->push_back(currElement);
		emit Progress(100*(++currProgress)/fullProgress);
	}
	return true;
}


bool Fort14Reader::ReadNodalData(int numNodes, std::ifstream *fileStream)
{	
	minX = minY = minZ = 99999.0;
	maxX = maxY = maxZ -99999.0;

	Node currNode;
	std::string currLine;
	for (int i=0; i<numNodes; ++i)
	{
		std::getline(*fileStream, currLine);
		std::stringstream(currLine) >> currNode.nodeNumber >>
					       currNode.xDat >>
					       currNode.yDat >>
					       currNode.zDat;
		currNode.x = atof(currNode.xDat.data());
		currNode.y = atof(currNode.yDat.data());
		currNode.z = atof(currNode.zDat.data());

		if (currNode.x < minX)
			minX = currNode.x;
		else if (currNode.x > maxX)
			maxX = currNode.x;
		if (currNode.y < minY)
			minY = currNode.y;
		else if (currNode.y > maxY)
			maxY = currNode.y;
		currNode.z *= -1.0;
		if (currNode.z < minZ)
			minZ = currNode.z;
		else if (currNode.z > maxZ)
			maxZ = currNode.z;

		nodes->push_back(currNode);
		emit Progress(100*(++currProgress)/fullProgress);
	}

	emit FoundDomainBounds(minX, minY, minZ, maxX, maxY, maxZ);

	return true;
}



