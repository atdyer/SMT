#include "BoundaryFinder.h"

BoundaryFinder::BoundaryFinder()
{
}


BoundaryFinder::~BoundaryFinder()
{
}


void BoundaryFinder::PerformBoundarySearch(std::vector<Element *> elements, Boundaries *boundaryData)
{
	if (boundaryData && !boundaryData->boundariesFound)
	{
		/*
		 * - elements is a list of all selected elements
		 * - boundaryData is the Boundaries object where the results will be stored
		 */

		std::map<unsigned int, std::vector<unsigned int> > nodeAdjacencyOuter;
		std::map<Edge, int> edgeCount;
		boundaryData->innerBoundaryNodes.clear();
		boundaryData->outerBoundaryNodes.clear();

		/*
		 * Count the number of times each edge appears in the selection
		 */

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
			/*
			 * If an edge only appears once, it is a boundary edge.
			 * This assumes that the are no elements in the interior of the selection
			 * that are not selected.
			 */
			for (std::map<Edge, int>::iterator it = edgeCount.begin(); it != edgeCount.end(); ++it)
			{
				if (it->second == 1)
				{
					nodeAdjacencyOuter[it->first.n1].push_back(it->first.n2);
					nodeAdjacencyOuter[it->first.n2].push_back(it->first.n1);
				}
			}


			if (nodeAdjacencyOuter.size() > 2)
			{

				/*
				 * Starting with the first node in the adjacency list, circle
				 * around the edge of the selected elements, creating a list
				 * of boundary nodes
				 */

				unsigned int previousNode;
				unsigned int currentNode = nodeAdjacencyOuter.begin()->first;
				unsigned int nextNode = nodeAdjacencyOuter[currentNode][0];


				for (unsigned int i=0; i<nodeAdjacencyOuter.size(); ++i)
				{
					// Push back current node
					boundaryData->outerBoundaryNodes.push_back(currentNode);

					// Set up next iteration
					previousNode = currentNode;
					currentNode = nextNode;

					if (nodeAdjacencyOuter[currentNode][0] == previousNode)
						nextNode = nodeAdjacencyOuter[currentNode][1];
					else
						nextNode = nodeAdjacencyOuter[currentNode][0];
				}


				/*
				 * Create an unordered list of inner boundary nodes
				 */

				std::set<unsigned int> innerBoundaryNodes;

				Element *currElement = 0;
				for (std::vector<Element*>::iterator elementIterator = elements.begin();
				     elementIterator != elements.end();
				     ++elementIterator)
				{
					currElement = *elementIterator;
					if (currElement)
					{
						int n1 = currElement->n1->nodeNumber;
						int n2 = currElement->n2->nodeNumber;
						int n3 = currElement->n3->nodeNumber;
						int n1i = nodeAdjacencyOuter.count(n1);
						int n2i = nodeAdjacencyOuter.count(n2);
						int n3i = nodeAdjacencyOuter.count(n3);
						int sum = n1i+n2i+n3i;

						if (sum > 0 && sum < 3)
						{
							if (n1i == 0)
							{
								innerBoundaryNodes.insert(n1);
							}
							if (n2i == 0)
							{
								innerBoundaryNodes.insert(n2);
							}
							if (n3i == 0)
							{
								innerBoundaryNodes.insert(n3);
							}
						}
					}
				}

				for (std::set<unsigned int>::iterator it = innerBoundaryNodes.begin();
				     it != innerBoundaryNodes.end();
				     ++it)
				{
					boundaryData->innerBoundaryNodes.push_back(*it);
				}

				boundaryData->boundariesFound = true;

			} else {
				boundaryData->boundariesFound = false;
			}

		} else {
			boundaryData->boundariesFound = false;
		}
	}
}


/////////////    Pretty good code for ordered inner boundary    ///////////////
//
//				/*
//				 * Find inner boundary nodes in a similar manner. If an element
//				 * has only one node on the outer boundary, both of the other
//				 * two are on the inner boundary.
//				 */

//				Element *currElement = 0;
//				for (std::vector<Element*>::iterator elementIterator = elements.begin();
//				     elementIterator != elements.end();
//				     ++elementIterator)
//				{
//					currElement = *elementIterator;
//					if (currElement)
//					{
//						int n1 = currElement->n1->nodeNumber;
//						int n2 = currElement->n2->nodeNumber;
//						int n3 = currElement->n3->nodeNumber;
//						int n1i = nodeAdjacencyOuter.count(n1);
//						int n2i = nodeAdjacencyOuter.count(n2);
//						int n3i = nodeAdjacencyOuter.count(n3);
//						int sum = n1i+n2i+n3i;

//						if (sum == 1)
//						{
//							if (n1i == 1)
//							{
//								nodeAdjacencyInner[n2].push_back(n3);
//								nodeAdjacencyInner[n3].push_back(n2);
//							}
//							if (n2i == 1)
//							{
//								nodeAdjacencyInner[n1].push_back(n3);
//								nodeAdjacencyInner[n3].push_back(n1);
//							}
//							if (n3i == 1)
//							{
//								nodeAdjacencyInner[n2].push_back(n1);
//								nodeAdjacencyInner[n1].push_back(n2);
//							}
//						}
//					}
//				}


//				/*
//				 * Circle around the domain the same way we did with the outer nodes
//				 */

//				currentNode = nodeAdjacencyInner.begin()->first;
//				nextNode = nodeAdjacencyInner[currentNode][0];

//				for (unsigned int i=0; i<nodeAdjacencyInner.size(); ++i)
//				{
//					// Push back current node
//					boundaryData->innerBoundaryNodes.push_back(currentNode);

//					// Set up next iteration
//					previousNode = currentNode;
//					currentNode = nextNode;
//					if (nodeAdjacencyInner[currentNode][0] == previousNode)
//						nextNode = nodeAdjacencyInner[currentNode][1];
//					else
//						nextNode = nodeAdjacencyInner[currentNode][0];
//				}
