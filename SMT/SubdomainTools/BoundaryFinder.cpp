#include "BoundaryFinder.h"

BoundaryFinder::BoundaryFinder()
{
	currentBoundaries = new Boundaries();
	currentBoundaries->innerBoundaryNodes.clear();
	currentBoundaries->maxZ = -99999;
	currentBoundaries->minZ = 99999;
	currentBoundaries->numElements = 0;
	currentBoundaries->numNodes = 0;
	currentBoundaries->orderedInnerBoundaryNodes.clear();
	currentBoundaries->orderedOuterBoundaryNodes.clear();
	currentBoundaries->outerBoundaryNodes.clear();
}


BoundaryFinder::~BoundaryFinder()
{
	delete currentBoundaries;
}


void BoundaryFinder::ResetBoundaries()
{
	if (currentBoundaries)
	{
		currentBoundaries->innerBoundaryNodes.clear();
		currentBoundaries->maxZ = -99999;
		currentBoundaries->minZ = 99999;
		currentBoundaries->numElements = 0;
		currentBoundaries->numNodes = 0;
		currentBoundaries->orderedInnerBoundaryNodes.clear();
		currentBoundaries->orderedOuterBoundaryNodes.clear();
		currentBoundaries->outerBoundaryNodes.clear();
	}
}


std::vector<unsigned int> BoundaryFinder::FindBoundaries(ElementState *elementSelection)
{
	edgesMap.clear();
	nodeAdjacency.clear();
	edgesList.clear();
	FindEdges(elementSelection->GetState());
	CreateEdgesList();
	return edgesList;
}


Boundaries BoundaryFinder::FindAllBoundaries(std::vector<Element> *elements)
{
	Boundaries boundaries;
	if (elements)
	{
		std::cout << "Searching " << elements->size() << " elements" << std::endl;

		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

		Element *currElement;
		for(std::vector<Element>::iterator it = elements->begin(); it != elements->end(); ++it)
		{
			currElement = &(*it);
			if (currElement)
			{
				Edge edge1 (currElement->n1->nodeNumber, currElement->n2->nodeNumber);
				Edge edge2 (currElement->n1->nodeNumber, currElement->n3->nodeNumber);
				Edge edge3 (currElement->n2->nodeNumber, currElement->n3->nodeNumber);
				if (edgeInElementCount.count(edge1) == 0)
					edgeInElementCount[edge1] = 0;
				if (edgeInElementCount.count(edge2) == 0)
					edgeInElementCount[edge2] = 0;
				if (edgeInElementCount.count(edge3) == 0)
					edgeInElementCount[edge3] = 0;
				edgeInElementCount[edge1] += 1;
				edgeInElementCount[edge2] += 1;
				edgeInElementCount[edge3] += 1;
				elementsThatContainNode[currElement->n1->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n2->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n3->nodeNumber].push_back(currElement);
			}
		}

		for (std::map<Edge, int>::iterator it = edgeInElementCount.begin(); it != edgeInElementCount.end(); ++it)
		{
			if (it->second == 1)
			{
				if (it->first.n1 != it->first.n2)
				{
					boundaries.outerBoundaryNodes.insert(it->first.n1);
					boundaries.outerBoundaryNodes.insert(it->first.n2);
				}
			}
		}

		for (std::set<unsigned int>::iterator it=boundaries.outerBoundaryNodes.begin(); it != boundaries.outerBoundaryNodes.end(); ++it)
		{
			unsigned int currentNode = *it;
			std::vector<Element*> elementsWithCurrNode = elementsThatContainNode[currentNode];
			if (elementsWithCurrNode.size() > 0)
			{
				for (std::vector<Element*>::iterator eIt=elementsWithCurrNode.begin(); eIt != elementsWithCurrNode.end(); ++eIt)
				{
					Element *currentElement = *eIt;
					if (currentElement)
					{
						if (boundaries.outerBoundaryNodes.count(currentElement->n1->nodeNumber) != 1)
						{
							boundaries.innerBoundaryNodes.insert(currentElement->n1->nodeNumber);
						}
						if (boundaries.outerBoundaryNodes.count(currentElement->n2->nodeNumber) != 1)
						{
							boundaries.innerBoundaryNodes.insert(currentElement->n2->nodeNumber);
						}
						if (boundaries.outerBoundaryNodes.count(currentElement->n3->nodeNumber) != 1)
						{
							boundaries.innerBoundaryNodes.insert(currentElement->n3->nodeNumber);
						}
					}
				}
			}
		}
	}
	return boundaries;
}


std::vector<unsigned int> BoundaryFinder::FindInnerBoundaries(ElementState *elementSelection)
{
	std::vector<unsigned int> innerBoundaryNodes;
	if (elementSelection)
	{
		std::vector<Element*> *selectedElements = elementSelection->GetState();
		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

		/* Make a map of the number of times every edge appears in the domain */
		/* Make a map of the list of Elements that every node is a part of */
		Element *currElement = 0;
		for (std::vector<Element*>::iterator it = selectedElements->begin(); it != selectedElements->end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				Edge edge1 (currElement->n1->nodeNumber, currElement->n2->nodeNumber);
				Edge edge2 (currElement->n1->nodeNumber, currElement->n3->nodeNumber);
				Edge edge3 (currElement->n2->nodeNumber, currElement->n3->nodeNumber);
				edgeInElementCount[edge1] += 1;
				edgeInElementCount[edge2] += 1;
				edgeInElementCount[edge3] += 1;
				elementsThatContainNode[currElement->n1->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n2->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n3->nodeNumber].push_back(currElement);
			} else {
				std::cout << "WARNING: Unable to dereference element" << std::endl;
			}
		}

		/* Make a list of the boundary nodes */
		std::set<unsigned int> boundaryNodes;
		for (std::map<Edge, int>::iterator it = edgeInElementCount.begin(); it != edgeInElementCount.end(); ++it)
		{
			if (it->second == 1)
			{
				if (it->first.n1 != it->first.n2)
				{
					boundaryNodes.insert(it->first.n1);
					boundaryNodes.insert(it->first.n2);
				}
			}
		}

		/* Boundary elements are the ones that contain a boundary node */
		/* Go through the list of boundary elements and find the nodes of those elements
		 * that are not on the edge. These are the inner boundary nodes */
		for (std::set<unsigned int>::iterator it=boundaryNodes.begin(); it != boundaryNodes.end(); ++it)
		{
			unsigned int currentNode = *it;
			std::vector<Element*> elementsWithCurrNode = elementsThatContainNode[currentNode];
			if (elementsWithCurrNode.size() > 0)
			{
				for (std::vector<Element*>::iterator eIt=elementsWithCurrNode.begin(); eIt != elementsWithCurrNode.end(); ++eIt)
				{
					Element *currentElement = *eIt;
					if (currentElement)
					{
						if (boundaryNodes.count(currentElement->n1->nodeNumber) != 1)
						{
							innerBoundaryNodes.push_back(currentElement->n1->nodeNumber);
						}
						if (boundaryNodes.count(currentElement->n2->nodeNumber) != 1)
						{
							innerBoundaryNodes.push_back(currentElement->n2->nodeNumber);
						}
						if (boundaryNodes.count(currentElement->n3->nodeNumber) != 1)
						{
							innerBoundaryNodes.push_back(currentElement->n3->nodeNumber);
						}
					}
				}
			}
		}
	}

	if (innerBoundaryNodes.size() > 0)
	{
		std::sort(innerBoundaryNodes.begin(), innerBoundaryNodes.end());
		std::vector<unsigned int>::iterator it = std::unique(innerBoundaryNodes.begin(), innerBoundaryNodes.end());
		innerBoundaryNodes.resize(std::distance(innerBoundaryNodes.begin(), it));
	}

	return innerBoundaryNodes;
}


Boundaries BoundaryFinder::FindOrderedBoundaries(std::vector<Element *> elements)
{
	Boundaries boundaryNodes;
	return RecursiveBoundarySearch(boundaryNodes, elements, true);
}


void BoundaryFinder::FindEdges(std::vector<Element *> *elements)
{
	for (std::vector<Element*>::iterator it = elements->begin(); it != elements->end(); ++it)
	{
		/* For each element, add the three edges to the list of edges */
		Edge newEdge1((*it)->n1->nodeNumber, (*it)->n2->nodeNumber);
		edgesMap[newEdge1] += 1;
		Edge newEdge2((*it)->n1->nodeNumber, (*it)->n3->nodeNumber);
		edgesMap[newEdge2] += 1;
		Edge newEdge3((*it)->n2->nodeNumber, (*it)->n3->nodeNumber);
		edgesMap[newEdge3] += 1;
	}

	for (std::map<Edge, int>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it)
	{
		if (it->second == 1)
		{
			nodeAdjacency[it->first.n1].push_back(it->first.n2);
			nodeAdjacency[it->first.n2].push_back(it->first.n1);
		}
	}
}


void BoundaryFinder::CreateEdgesList()
{
	if (nodeAdjacency.size() > 0)
	{
		unsigned int firstNode = nodeAdjacency.begin()->first;
		unsigned int secondNode = 0;
		while(true)
		{
			edgesList.push_back(firstNode);
			if (nodeAdjacency[firstNode][0] != 0)
			{
				secondNode = nodeAdjacency[firstNode][0];
				if (nodeAdjacency[secondNode][0] == firstNode)
					nodeAdjacency[secondNode][0] = 0;
				else
					nodeAdjacency[secondNode][1] = 0;
				nodeAdjacency[firstNode][0] = 0;
			}
			else if (nodeAdjacency[firstNode][1] != 0)
			{
				secondNode = nodeAdjacency[firstNode][1];
				if (nodeAdjacency[secondNode][0] == firstNode)
					nodeAdjacency[secondNode][0] = 0;
				else
					nodeAdjacency[secondNode][1] = 0;
				nodeAdjacency[firstNode][1] = 0;
			} else {
				break;
			}
			firstNode = secondNode;
		}
	}
}


Boundaries BoundaryFinder::RecursiveBoundarySearch(Boundaries boundaryNodes, std::vector<Element *> elements, bool recurse)
{
	if (elements.size())
	{
		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

		/*
		 * Count the number of times each Edge appears. Edges that appear only once are on
		 * the boundary. Also create a map of node numbers to the elements that contain them
		 * for use in finding inner boundary nodes.
		 */
		Element *currElement;
		for (std::vector<Element*>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				Edge edge1 (currElement->n1->nodeNumber, currElement->n2->nodeNumber);
				Edge edge2 (currElement->n1->nodeNumber, currElement->n3->nodeNumber);
				Edge edge3 (currElement->n2->nodeNumber, currElement->n3->nodeNumber);
				if (edgeInElementCount.count(edge1) == 0)
					edgeInElementCount[edge1] = 0;
				if (edgeInElementCount.count(edge2) == 0)
					edgeInElementCount[edge2] = 0;
				if (edgeInElementCount.count(edge3) == 0)
					edgeInElementCount[edge3] = 0;
				edgeInElementCount[edge1] += 1;
				edgeInElementCount[edge2] += 1;
				edgeInElementCount[edge3] += 1;
				elementsThatContainNode[currElement->n1->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n2->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n3->nodeNumber].push_back(currElement);
			}
		}


		/*
		 * Create an adjacency table for nodes that fall on the outer boundary
		 */
		std::map<unsigned int, AdjacentNodes> sortingTable;
		for (std::map<Edge, int>::iterator it = edgeInElementCount.begin(); it != edgeInElementCount.end(); ++it)
		{
			if (it->second == 1)
			{
				unsigned int n1 = it->first.n1;
				unsigned int n2 = it->first.n2;
				if (n1 != n2)
				{
					if (recurse)
					{
						boundaryNodes.outerBoundaryNodes.insert(n1);
						boundaryNodes.outerBoundaryNodes.insert(n2);
					} else {
						boundaryNodes.innerBoundaryNodes.insert(n1);
						boundaryNodes.innerBoundaryNodes.insert(n2);
					}

					if (sortingTable[n1].n1 == 0)
						sortingTable[n1].n1 = n2;
					else
						sortingTable[n1].n2 = n2;

					if (sortingTable[n2].n1 == 0)
						sortingTable[n2].n1 = n1;
					else
						sortingTable[n2].n2 = n1;
				}
			}
		}

		/*
		 * Step through the adjacency table, starting with the first node in the table
		 */
		int maxCount = sortingTable.size();
		int currCount = 0;

		unsigned int firstNode = sortingTable.begin()->first;
		unsigned int lastNode = 0;
		unsigned int currNode = firstNode;
		unsigned int nextNode = sortingTable[currNode].n1;
		while (currCount < maxCount)
		{
			if (recurse)
				boundaryNodes.orderedOuterBoundaryNodes.push_back(currNode);
			else
				boundaryNodes.orderedInnerBoundaryNodes.push_back(currNode);

			if (nextNode == firstNode)
				break;

			++currCount;
			lastNode = currNode;
			currNode = nextNode;
			if (sortingTable[currNode].n1 != lastNode)
				nextNode = sortingTable[currNode].n1;
			else
				nextNode = sortingTable[currNode].n2;
		}


		if (recurse)
		{
			/*
			 * Create a new set of elements, removing all elements that contain an outer boundary node
			 */
			std::vector<Element*> innerElements;
			for (std::vector<Element*>::iterator it = elements.begin(); it != elements.end(); ++it)
			{
				Element *currElement = *it;
				if (boundaryNodes.outerBoundaryNodes.count(currElement->n1->nodeNumber) == 0 &&
				    boundaryNodes.outerBoundaryNodes.count(currElement->n2->nodeNumber) == 0 &&
				    boundaryNodes.outerBoundaryNodes.count(currElement->n3->nodeNumber) == 0)
				{
					innerElements.push_back(currElement);
				}
			}

			return RecursiveBoundarySearch(boundaryNodes, innerElements, false);
		}
	}

	return boundaryNodes;
}


Boundaries* BoundaryFinder::NewBoundarySearch(std::vector<Element*> elements)
{
	ResetBoundaries();
	Boundaries *boundaryNodes = currentBoundaries;
//	return boundaryNodes;

	boundaryNodes->numElements = elements.size();
	boundaryNodes->numNodes = 0;
	boundaryNodes->minZ = 99999.0;
	boundaryNodes->maxZ = -99999.0;

	if (elements.size())
	{
		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

		/*
		 * Count the number of times each Edge appears. Edges that appear only once are on
		 * the boundary. Also create a map of node numbers to the elements that contain them
		 * for use in finding inner boundary nodes.
		 */
		Element *currElement;
		for (std::vector<Element*>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				Edge edge1 (currElement->n1->nodeNumber, currElement->n2->nodeNumber);
				Edge edge2 (currElement->n1->nodeNumber, currElement->n3->nodeNumber);
				Edge edge3 (currElement->n2->nodeNumber, currElement->n3->nodeNumber);
				if (edgeInElementCount.count(edge1) == 0)
					edgeInElementCount[edge1] = 0;
				if (edgeInElementCount.count(edge2) == 0)
					edgeInElementCount[edge2] = 0;
				if (edgeInElementCount.count(edge3) == 0)
					edgeInElementCount[edge3] = 0;
				edgeInElementCount[edge1] += 1;
				edgeInElementCount[edge2] += 1;
				edgeInElementCount[edge3] += 1;
				elementsThatContainNode[currElement->n1->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n2->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n3->nodeNumber].push_back(currElement);

				if (currElement->n1->z < boundaryNodes->minZ)
					boundaryNodes->minZ = currElement->n1->z;
				else if (currElement->n1->z > boundaryNodes->maxZ)
					boundaryNodes->maxZ = currElement->n1->z;
				if (currElement->n2->z < boundaryNodes->minZ)
					boundaryNodes->minZ = currElement->n2->z;
				else if (currElement->n2->z > boundaryNodes->maxZ)
					boundaryNodes->maxZ = currElement->n2->z;
				if (currElement->n3->z < boundaryNodes->minZ)
					boundaryNodes->minZ = currElement->n3->z;
				else if (currElement->n3->z > boundaryNodes->maxZ)
					boundaryNodes->maxZ = currElement->n3->z;
			}
		}

		boundaryNodes->numNodes = elementsThatContainNode.size();


		/*
		 * Create an adjacency table for nodes that fall on the outer boundary
		 */
		std::map<unsigned int, AdjacentNodes> sortingTable;
		for (std::map<Edge, int>::iterator it = edgeInElementCount.begin(); it != edgeInElementCount.end(); ++it)
		{
			if (it->second == 1)
			{
				unsigned int n1 = it->first.n1;
				unsigned int n2 = it->first.n2;
				if (n1 != n2)
				{
					boundaryNodes->outerBoundaryNodes.insert(n1);
					boundaryNodes->outerBoundaryNodes.insert(n2);
					/*			     */

					if (sortingTable[n1].n1 == 0)
						sortingTable[n1].n1 = n2;
					else
						sortingTable[n1].n2 = n2;

					if (sortingTable[n2].n1 == 0)
						sortingTable[n2].n1 = n1;
					else
						sortingTable[n2].n2 = n1;
				}
			}
		}

		/*
		 * Step through the adjacency table, starting with the first node in the table
		 */
		int maxCount = sortingTable.size();
		int currCount = 0;

		unsigned int firstNode = sortingTable.begin()->first;
		unsigned int lastNode = 0;
		unsigned int currNode = firstNode;
		unsigned int nextNode = sortingTable[currNode].n1;
		while (currCount < maxCount)
		{
			/* Node stuff */
			boundaryNodes->orderedOuterBoundaryNodes.push_back(currNode);

			if (nextNode == firstNode)
				break;

			++currCount;
			lastNode = currNode;
			currNode = nextNode;
			if (sortingTable[currNode].n1 != lastNode)
				nextNode = sortingTable[currNode].n1;
			else
				nextNode = sortingTable[currNode].n2;
		}


		std::vector<Element*> orderedBoundaryElements;

		/* Add the first element (the one that contains the first two outer boundary nodes */
		unsigned int n1 = boundaryNodes->orderedOuterBoundaryNodes.at(0);
		unsigned int n2 = boundaryNodes->orderedOuterBoundaryNodes.at(1);
		for (std::vector<Element*>::iterator n1Elements = elementsThatContainNode[n1].begin();
		     n1Elements != elementsThatContainNode[n1].end();
		     ++n1Elements)
		{
			for (std::vector<Element*>::iterator n2Elements = elementsThatContainNode[n2].begin();
			     n2Elements != elementsThatContainNode[n2].end();
			     ++n2Elements)
			{
				if (*n1Elements == *n2Elements)
				{
					orderedBoundaryElements.push_back(*n1Elements);
				}
			}
		}

		Element *lastElement;
		std::vector<Element*> elementsWithCurrOBN;
		for (std::vector<unsigned int>::iterator currOBN = boundaryNodes->orderedOuterBoundaryNodes.begin()+1;
		     currOBN != boundaryNodes->orderedOuterBoundaryNodes.end();
		     ++currOBN)
		{
			lastElement = orderedBoundaryElements.back();

			elementsWithCurrOBN = elementsThatContainNode[*currOBN];
			int numElementsToAdd = elementsWithCurrOBN.size()-1;
			int numElementsAdded = 0;

			for (unsigned int i=0; i<elementsWithCurrOBN.size(); ++i)
			{
				if (elementsWithCurrOBN[i])
				{
					int commonNodeCount = 0;
					if ((lastElement->n1->nodeNumber == elementsWithCurrOBN[i]->n1->nodeNumber) ||
					    (lastElement->n1->nodeNumber == elementsWithCurrOBN[i]->n2->nodeNumber) ||
					    (lastElement->n1->nodeNumber == elementsWithCurrOBN[i]->n3->nodeNumber))
						++commonNodeCount;
					if ((lastElement->n2->nodeNumber == elementsWithCurrOBN[i]->n1->nodeNumber) ||
					    (lastElement->n2->nodeNumber == elementsWithCurrOBN[i]->n2->nodeNumber) ||
					    (lastElement->n2->nodeNumber == elementsWithCurrOBN[i]->n3->nodeNumber))
						++commonNodeCount;
					if ((lastElement->n3->nodeNumber == elementsWithCurrOBN[i]->n1->nodeNumber) ||
					    (lastElement->n3->nodeNumber == elementsWithCurrOBN[i]->n2->nodeNumber) ||
					    (lastElement->n3->nodeNumber == elementsWithCurrOBN[i]->n3->nodeNumber))
						++commonNodeCount;

					if (commonNodeCount == 3)
					{
						elementsWithCurrOBN[i] = 0;
					}
				}
			}

			while (numElementsAdded < numElementsToAdd)
			{
				for (unsigned int i=0; i<elementsWithCurrOBN.size(); ++i)
				{
					if (elementsWithCurrOBN[i])
					{
						int commonNodeCount = 0;
						if ((lastElement->n1->nodeNumber == elementsWithCurrOBN[i]->n1->nodeNumber) ||
						    (lastElement->n1->nodeNumber == elementsWithCurrOBN[i]->n2->nodeNumber) ||
						    (lastElement->n1->nodeNumber == elementsWithCurrOBN[i]->n3->nodeNumber))
							++commonNodeCount;
						if ((lastElement->n2->nodeNumber == elementsWithCurrOBN[i]->n1->nodeNumber) ||
						    (lastElement->n2->nodeNumber == elementsWithCurrOBN[i]->n2->nodeNumber) ||
						    (lastElement->n2->nodeNumber == elementsWithCurrOBN[i]->n3->nodeNumber))
							++commonNodeCount;
						if ((lastElement->n3->nodeNumber == elementsWithCurrOBN[i]->n1->nodeNumber) ||
						    (lastElement->n3->nodeNumber == elementsWithCurrOBN[i]->n2->nodeNumber) ||
						    (lastElement->n3->nodeNumber == elementsWithCurrOBN[i]->n3->nodeNumber))
							++commonNodeCount;

						if (commonNodeCount == 2)
						{
							orderedBoundaryElements.push_back(elementsWithCurrOBN[i]);
							lastElement = orderedBoundaryElements.back();
							elementsWithCurrOBN[i] = 0;
							++numElementsAdded;
						}
					}
				}
			}
		}

		unsigned int n3;
		for (std::vector<Element*>::iterator currOuterElement = orderedBoundaryElements.begin();
		     currOuterElement != orderedBoundaryElements.end();
		     ++currOuterElement)
		{
			n1 = (*currOuterElement)->n1->nodeNumber;
			n2 = (*currOuterElement)->n2->nodeNumber;
			n3 = (*currOuterElement)->n3->nodeNumber;
			if (!boundaryNodes->orderedInnerBoundaryNodes.size())
			{
				if (!boundaryNodes->outerBoundaryNodes.count(n1))
					boundaryNodes->orderedInnerBoundaryNodes.push_back(n1);
				if (!boundaryNodes->outerBoundaryNodes.count(n2))
					boundaryNodes->orderedInnerBoundaryNodes.push_back(n2);
				if (!boundaryNodes->outerBoundaryNodes.count(n3))
					boundaryNodes->orderedInnerBoundaryNodes.push_back(n3);
			} else {
				lastNode = boundaryNodes->orderedInnerBoundaryNodes.back();
				if (!boundaryNodes->outerBoundaryNodes.count(n1) && (n1 != lastNode))
					boundaryNodes->orderedInnerBoundaryNodes.push_back(n1);
				if (!boundaryNodes->outerBoundaryNodes.count(n2) && (n2 != lastNode))
					boundaryNodes->orderedInnerBoundaryNodes.push_back(n2);
				if (!boundaryNodes->outerBoundaryNodes.count(n3) && (n3 != lastNode))
					boundaryNodes->orderedInnerBoundaryNodes.push_back(n3);
			}
		}

		return boundaryNodes;
	}
}


Boundaries *BoundaryFinder::FindOuterBoundaries(std::vector<Element *> elements)
{

	ResetBoundaries();
	nodeAdjacency.clear();

	std::map<Edge, int> edgeCount;

	Element *currElement = 0;
	for (std::vector<Element*>::iterator elementIterator = elements.begin();
	     elementIterator != elements.end();
	     ++elementIterator)
	{
		currElement = *elementIterator;
		if (currElement)
		{
			Edge edge1 (currElement->n1->nodeNumber, currElement->n2->nodeNumber);
			Edge edge2 (currElement->n1->nodeNumber, currElement->n3->nodeNumber);
			Edge edge3 (currElement->n2->nodeNumber, currElement->n3->nodeNumber);
			if (edgeCount.count(edge1) == 0)
				edgeCount[edge1] = 0;
			if (edgeCount.count(edge2) == 0)
				edgeCount[edge2] = 0;
			if (edgeCount.count(edge3) == 0)
				edgeCount[edge3] = 0;
			edgeCount[edge1] += 1;
			edgeCount[edge2] += 1;
			edgeCount[edge3] += 1;
		}
	}

	if (edgeCount.size() > 2)
	{
		for (std::map<Edge, int>::iterator it = edgeCount.begin(); it != edgeCount.end(); ++it)
		{
			if (it->second == 1)
			{
				nodeAdjacency[it->first.n1].push_back(it->first.n2);
				nodeAdjacency[it->first.n2].push_back(it->first.n1);
			}
		}

		if (nodeAdjacency.size() > 2)
		{

			unsigned int previousNode;
			unsigned int currentNode = nodeAdjacency.begin()->first;
			unsigned int nextNode = nodeAdjacency[currentNode][0];

			for (unsigned int i=0; i<nodeAdjacency.size(); ++i)
			{
				// Push back current node
				currentBoundaries->orderedOuterBoundaryNodes.push_back(currentNode);

				// Set up next iteration
				previousNode = currentNode;
				currentNode = nextNode;
				if (nodeAdjacency[currentNode][0] == previousNode)
					nextNode = nodeAdjacency[currentNode][1];
				else
					nextNode = nodeAdjacency[currentNode][0];
			}
		}
	}

	return currentBoundaries;
}
