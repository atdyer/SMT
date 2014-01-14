#include "QuadtreeSearch.h"

QuadtreeSearch::QuadtreeSearch()
{

}


std::vector<Node*> QuadtreeSearch::FindNodes(branch *root)
{
	finalNodes.clear();
	partialNodes.clear();
	numFollowed = 0;
	numLeaves = 0;

	int earlyExit = SearchNodes(root);
	BruteForceNodes();

//	if (earlyExit)
//		std::cout << "Early exit occurred" << std::endl;
//	else
//		std::cout << "Did not early exit" << std::endl;

	return finalNodes;
}


std::vector<Element*> QuadtreeSearch::FindElements(branch *root)
{
	finalElements.clear();
	partialElements.clear();
	numFollowed = 0;

	int earlyExit = SearchElements(root);
	BruteForceElements();

//	if (earlyExit)
//		std::cout << "Early exit occurred" << std::endl;
//	else
//		std::cout << "Did not early exit" << std::endl;

	return finalElements;
}


std::vector< std::vector<Node*>*> QuadtreeSearch::FindNodesLists(branch *root)
{
	finalNodesLists.clear();
	partialNodesLists.clear();

	SearchNodesPtrs(root);
	BruteForceNodesLists();

	return finalNodesLists;
}


std::vector< std::vector<Element*>*> QuadtreeSearch::FindElementsLists(branch *root)
{
	finalElementsLists.clear();
	partialElementsLists.clear();

	SearchElementsPtrs(root);
	BruteForceElementsLists();

	return finalElementsLists;
}


int QuadtreeSearch::SearchNodes(branch *currBranch)
{
//	++numFollowed;
	if (ShapeIntersects(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				++numFollowed;
				if (SearchNodes(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				++numFollowed;
//				++numLeaves;
				if (SearchNodes(currBranch->leaves[i]))
					return 1;
			}
		}
	}
	else if (currBranch->contains(shapeEdgePoint))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				++numFollowed;
				if (SearchNodes(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				++numFollowed;
//				++numLeaves;
				if (SearchNodes(currBranch->leaves[i]))
					return 1;
			}
		}
		return 1;
	}
	else
	{
		if (PointIsInsideShape(Point(currBranch->bounds[1], currBranch->bounds[3])))
		{
			AddAll(currBranch, &finalNodes);
		}
	}
	return 0;
}


int QuadtreeSearch::SearchNodes(leaf *currLeaf)
{
//	++numLeaves;
//	++numFollowed;
	if (ShapeIntersects(currLeaf))
	{
		AddAll(currLeaf, &partialNodes);
	}
	else if (currLeaf->contains(shapeEdgePoint))
	{
		AddAll(currLeaf, &partialNodes);
		return 1;
	}
	else if (PointIsInsideShape(Point(currLeaf->bounds[1], currLeaf->bounds[3])))
	{
		AddAll(currLeaf, &finalNodes);
	}
	return 0;
}


int QuadtreeSearch::SearchElements(branch *currBranch)
{
	if (ShapeIntersects(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				++numFollowed;
				if (SearchElements(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				++numFollowed;
				++numLeaves;
				if (SearchElements(currBranch->leaves[i]))
					return 1;
			}
		}
	}
	else if (currBranch->contains(shapeEdgePoint))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				++numFollowed;
				if (SearchElements(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				++numFollowed;
				++numLeaves;
				if (SearchElements(currBranch->leaves[i]))
					return 1;
			}
		}
		return 1;
	}
	else if (PointIsInsideShape(Point(currBranch->bounds[1], currBranch->bounds[3])))
	{
		AddAll(currBranch, &finalElements);
	}
	return 0;
}


int QuadtreeSearch::SearchElements(leaf *currLeaf)
{
	if (ShapeIntersects(currLeaf))
	{
		AddAll(currLeaf, &partialElements);
	}
	else if (currLeaf->contains(shapeEdgePoint))
	{
		AddAll(currLeaf, &partialElements);
		return 1;
	}
	else if (PointIsInsideShape(Point(currLeaf->bounds[1], currLeaf->bounds[3])))
	{
		AddAll(currLeaf, &finalElements);
	}
	return 0;
}


void QuadtreeSearch::BruteForceNodes()
{
	for (std::vector<Node*>::iterator currNode = partialNodes.begin();
	     currNode != partialNodes.end();
	     ++currNode)
	{
		if (PointIsInsideShape(Point((*currNode)->normX, (*currNode)->normY)))
		{
			finalNodes.push_back(*currNode);
		}
	}
}


void QuadtreeSearch::BruteForceElements()
{
	for (std::vector<Element*>::iterator currElement = partialElements.begin();
	     currElement != partialElements.end();
	     ++currElement)
	{
		if (PointIsInsideShape(Point((*currElement)->n1->normX, (*currElement)->n1->normY)) ||
		    PointIsInsideShape(Point((*currElement)->n2->normX, (*currElement)->n2->normY)) ||
		    PointIsInsideShape(Point((*currElement)->n3->normX, (*currElement)->n3->normY)))
		{
			finalElements.push_back(*currElement);
		}
	}
}


void QuadtreeSearch::AddAll(branch *currBranch, std::vector<Node*>* nodeList)
{
//	++numFollowed;
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
		{
			++numFollowed;
			AddAll(currBranch->branches[i], nodeList);
		}
		if (currBranch->leaves[i])
		{
			++numFollowed;
//			++numLeaves;
			AddAll(currBranch->leaves[i], nodeList);
		}
	}
}


void QuadtreeSearch::AddAll(leaf *currLeaf, std::vector<Node*>* nodeList)
{
//	++numFollowed;
	if (currLeaf->nodes.size() > 0)
	{
		nodeList->insert(nodeList->end(), currLeaf->nodes.begin(), currLeaf->nodes.end());
		++numLeaves;
	}
}


void QuadtreeSearch::AddAll(branch *currBranch, std::vector<Element*>* elementList)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
		{
			++numFollowed;
			AddAll(currBranch->branches[i], elementList);
		}
		if (currBranch->leaves[i])
		{
			++numFollowed;
			AddAll(currBranch->leaves[i], elementList);
		}
	}
}


void QuadtreeSearch::AddAll(leaf *currLeaf, std::vector<Element*>* elementList)
{
	if (currLeaf->elements.size() > 0)
		elementList->insert(elementList->end(), currLeaf->elements.begin(), currLeaf->elements.end());
}







/* Experimental Below Here */



void QuadtreeSearch::SearchNodesPtrs(branch *currBranch)
{
	if (ShapeIntersects(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchNodesPtrs(currBranch->branches[i]);
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				SearchNodesPtrs(currBranch->leaves[i]);
			}
		}
	}
	else if (currBranch->contains(shapeEdgePoint))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchNodesPtrs(currBranch->branches[i]);
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				SearchNodesPtrs(currBranch->leaves[i]);
			}
		}
	}
	else
	{
		if (PointIsInsideShape(Point(currBranch->bounds[1], currBranch->bounds[3])))
		{
			AddAllPtrs(currBranch, &finalNodesLists);
		}
	}
}


void QuadtreeSearch::SearchNodesPtrs(leaf *currLeaf)
{
	if (ShapeIntersects(currLeaf))
	{
		AddAllPtrs(currLeaf, &partialNodesLists);
	}
	else if (currLeaf->contains(shapeEdgePoint))
	{
		AddAllPtrs(currLeaf, &partialNodesLists);
	}
	else if (PointIsInsideShape(Point(currLeaf->bounds[1], currLeaf->bounds[3])))
	{
		AddAllPtrs(currLeaf, &finalNodesLists);
	}
}


void QuadtreeSearch::SearchElementsPtrs(branch *currBranch)
{
	if (ShapeIntersects(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchElementsPtrs(currBranch->branches[i]);
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				SearchElementsPtrs(currBranch->leaves[i]);
			}
		}
	}
	else if (currBranch->contains(shapeEdgePoint))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchElementsPtrs(currBranch->branches[i]);
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				SearchElementsPtrs(currBranch->leaves[i]);
			}
		}
	}
	else if (PointIsInsideShape(Point(currBranch->bounds[1], currBranch->bounds[3])))
	{
		AddAllPtrs(currBranch, &finalElementsLists);
	}
}


void QuadtreeSearch::SearchElementsPtrs(leaf *currLeaf)
{
	if (ShapeIntersects(currLeaf))
	{
		AddAllPtrs(currLeaf, &partialElementsLists);
	}
	else if (currLeaf->contains(shapeEdgePoint))
	{
		AddAllPtrs(currLeaf, &partialElementsLists);
	}
	else if (PointIsInsideShape(Point(currLeaf->bounds[1], currLeaf->bounds[3])))
	{
		AddAllPtrs(currLeaf, &finalElementsLists);
	}
}


void QuadtreeSearch::BruteForceNodesLists()
{
	finalNodes.clear();
	for (std::vector<std::vector<Node*>*>::iterator currList=partialNodesLists.begin();
	     currList != partialNodesLists.end();
	     ++currList)
	{
		for (std::vector<Node*>::iterator currNode = (*currList)->begin();
		     currNode != (*currList)->end();
		     ++currNode)
		{
			if (PointIsInsideShape(Point((*currNode)->normX, (*currNode)->normY)))
			{
				finalNodes.push_back(*currNode);
			}
		}
	}
	finalNodesLists.push_back(&finalNodes);
}


void QuadtreeSearch::BruteForceElementsLists()
{
	finalElements.clear();
	for (std::vector<std::vector<Element*>*>::iterator currList=partialElementsLists.begin();
	     currList != partialElementsLists.end();
	     ++currList)
	{
		for (std::vector<Element*>::iterator currElement = (*currList)->begin();
		     currElement != (*currList)->end();
		     ++currElement)
		{
			if (PointIsInsideShape(Point((*currElement)->n1->normX, (*currElement)->n1->normY)) ||
			    PointIsInsideShape(Point((*currElement)->n2->normX, (*currElement)->n2->normY)) ||
			    PointIsInsideShape(Point((*currElement)->n3->normX, (*currElement)->n3->normY)))
			{
				finalElements.push_back(*currElement);
			}
		}
	}
	finalElementsLists.push_back(&finalElements);
}


void QuadtreeSearch::AddAllPtrs(branch *currBranch, std::vector<std::vector<Node*>*> *nodeList)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
			AddAllPtrs(currBranch->branches[i], nodeList);
		if (currBranch->leaves[i])
			AddAllPtrs(currBranch->leaves[i], nodeList);
	}
}


void QuadtreeSearch::AddAllPtrs(leaf *currLeaf, std::vector<std::vector<Node*>*> *nodeList)
{
	if (currLeaf->nodes.size() > 0)
		nodeList->push_back(&currLeaf->nodes);
}


void QuadtreeSearch::AddAllPtrs(branch *currBranch, std::vector<std::vector<Element*>*> *elementList)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
			AddAllPtrs(currBranch->branches[i], elementList);
		if (currBranch->leaves[i])
			AddAllPtrs(currBranch->leaves[i], elementList);
	}
}


void QuadtreeSearch::AddAllPtrs(leaf *currLeaf, std::vector<std::vector<Element*>*> *elementList)
{
	if (currLeaf->elements.size() > 0)
		elementList->push_back(&currLeaf->elements);
}



