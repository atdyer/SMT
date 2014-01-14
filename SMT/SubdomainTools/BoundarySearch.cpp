#include "BoundarySearch.h"

BoundarySearch::BoundarySearch()
{
}


Boundaries BoundarySearch::FindBoundaries(std::vector<Element *> elements)
{
	Boundaries boundaryNodes;

	if (elements.size())
	{
		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

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

		for (std::map<Edge, int>::iterator it = edgeInElementCount.begin(); it != edgeInElementCount.end(); ++it)
		{
			if (it->second == 1)
			{
				if (it->first.n1 != it->first.n2)
				{
					boundaryNodes.outerBoundaryNodes.insert(it->first.n1);
					boundaryNodes.outerBoundaryNodes.insert(it->first.n2);
				}
			}
		}

		for (std::set<unsigned int>::iterator it=boundaryNodes.outerBoundaryNodes.begin(); it != boundaryNodes.outerBoundaryNodes.end(); ++it)
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
						if (boundaryNodes.outerBoundaryNodes.count(currentElement->n1->nodeNumber) != 1)
						{
							boundaryNodes.innerBoundaryNodes.insert(currentElement->n1->nodeNumber);
						}
						if (boundaryNodes.outerBoundaryNodes.count(currentElement->n2->nodeNumber) != 1)
						{
							boundaryNodes.innerBoundaryNodes.insert(currentElement->n2->nodeNumber);
						}
						if (boundaryNodes.outerBoundaryNodes.count(currentElement->n3->nodeNumber) != 1)
						{
							boundaryNodes.innerBoundaryNodes.insert(currentElement->n3->nodeNumber);
						}
					}
				}
			}
		}
	}

	return boundaryNodes;
}


Boundaries BoundarySearch::FindBoundaries(std::vector<Element> *elements)
{
	Boundaries boundaryNodes;

	if (elements)
	{
		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

		Element *currElement;
		for (std::vector<Element>::iterator it = elements->begin(); it != elements->end(); ++it)
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
					boundaryNodes.outerBoundaryNodes.insert(it->first.n1);
					boundaryNodes.outerBoundaryNodes.insert(it->first.n2);
				}
			}
		}

		for (std::set<unsigned int>::iterator it=boundaryNodes.outerBoundaryNodes.begin(); it != boundaryNodes.outerBoundaryNodes.end(); ++it)
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
						if (boundaryNodes.outerBoundaryNodes.count(currentElement->n1->nodeNumber) != 1)
						{
							boundaryNodes.innerBoundaryNodes.insert(currentElement->n1->nodeNumber);
						}
						if (boundaryNodes.outerBoundaryNodes.count(currentElement->n2->nodeNumber) != 1)
						{
							boundaryNodes.innerBoundaryNodes.insert(currentElement->n2->nodeNumber);
						}
						if (boundaryNodes.outerBoundaryNodes.count(currentElement->n3->nodeNumber) != 1)
						{
							boundaryNodes.innerBoundaryNodes.insert(currentElement->n3->nodeNumber);
						}
					}
				}
			}
		}
	}

	return boundaryNodes;
}
