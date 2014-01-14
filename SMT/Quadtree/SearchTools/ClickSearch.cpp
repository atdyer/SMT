#include "ClickSearch.h"

ClickSearch::ClickSearch()
{
	x = 0.0;
	y = 0.0;
}


Node* ClickSearch::FindNode(branch *root, float x, float y)
{
	this->x = x;
	this->y = y;

	return SearchNodes(root);
}


Element* ClickSearch::FindElement(branch *root, float x, float y)
{
	this->x = x;
	this->y = y;

	return SearchElements(root);
}


Node* ClickSearch::SearchNodes(branch *currBranch)
{
	if (PointIsInsideSquare(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				Node *result = SearchNodes(currBranch->branches[i]);
				if (result)
				{
					return result;
				}
			}
			if (currBranch->leaves[i])
			{
				Node *result = SearchNodes(currBranch->leaves[i]);
				if (result)
				{
					return result;
				}
			}
		}
	}
	return 0;
}


Node* ClickSearch::SearchNodes(leaf *currLeaf)
{
	if (PointIsInsideSquare(currLeaf))
	{
		bool itStarted = false;
		Node *currClosest = 0;
		float recordDistance = 0.0;
		float currentDistance = 0.0;
		for (std::vector<Node*>::iterator it = currLeaf->nodes.begin(); it != currLeaf->nodes.end(); ++it)
		{
			if (!itStarted)
			{
				itStarted = true;
				currClosest = *it;
				recordDistance = Distance(*it);
			} else {
				currentDistance = Distance(*it);
				if (currentDistance < recordDistance)
				{
					currClosest = *it;
				}
			}
		}
		return currClosest;
	}
	return 0;
}


Element* ClickSearch::SearchElements(branch *currBranch)
{
	if (PointIsInsideSquare(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				Element *result = SearchElements(currBranch->branches[i]);
				if (result)
				{
					return result;
				}
			}
			if (currBranch->leaves[i])
			{
				Element *result = SearchElements(currBranch->leaves[i]);
				if (result)
				{
					return result;
				}
			}
		}
	}
	return 0;
}


Element* ClickSearch::SearchElements(leaf *currLeaf)
{
	if (PointIsInsideSquare(currLeaf))
	{
		for (std::vector<Element*>::iterator it = currLeaf->elements.begin(); it != currLeaf->elements.end(); ++it)
		{
			if (PointIsInsideElement(*it))
			{
				return *it;
			}
		}
	}
	return 0;
}


/**
 * @brief Determines if the point is inside the bounding box of a branch
 *
 * Determines if the point is inside the bounding box of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the point falls within the bounds of the branch
 * @return false if the point does not fall within the bounds of the branch
 */
bool ClickSearch::PointIsInsideSquare(branch *currBranch)
{
	if (x >= currBranch->bounds[0] &&
	    x <= currBranch->bounds[1] &&
	    y >= currBranch->bounds[2] &&
	    y <= currBranch->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if the point is inside the bounding box of a leaf
 *
 * Determines if the point is inside the bounding box of a leaf.
 *
 * @param currLeaf The leaf to test
 * @return true if the point falls within the bounds of the leaf
 * @return false if the point does not fall within the bounds of the leaf
 */
bool ClickSearch::PointIsInsideSquare(leaf *currLeaf)
{
	if (x >= currLeaf->bounds[0] &&
	    x <= currLeaf->bounds[1] &&
	    y >= currLeaf->bounds[2] &&
	    y <= currLeaf->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if the point is inside of an Element
 *
 * Determines if the point is inside of an Element using
 * barycentric coordinates.
 *
 * @param currElement The Element to test
 * @return true if the point falls inside of the Element
 * @return false if the point does not fall inside of the Element
 */
bool ClickSearch::PointIsInsideElement(Element *currElement)
{
	Node *n1 = currElement->n1;
	Node *n2 = currElement->n2;
	Node *n3 = currElement->n3;
	float a = (n2->normX - n1->normX)*(y - n1->normY) - (x - n1->normX)*(n2->normY - n1->normY);
	float b = (n3->normX - n2->normX)*(y - n2->normY) - (x - n2->normX)*(n3->normY - n2->normY);
	float c = (n1->normX - n3->normX)*(y - n3->normY) - (x - n3->normX)*(n1->normY - n3->normY);


	if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0))
		return true;
	return false;
}


/**
 * @brief Calculates the distance between the point and a Node
 *
 * Calculates the distance between the point and a Node.
 *
 * @param currNode The Node to find the distance to
 * @return The distance between the point and the Node
 */
float ClickSearch::Distance(Node *currNode)
{
	return sqrt(pow(x - currNode->normX, 2.0) + pow(y - currNode->normY, 2.0));
}


float ClickSearch::Distance(Element *currElement)
{
	return (Distance(currElement->n1) + Distance(currElement->n2) + Distance(currElement->n3)) / 3.0;
}
