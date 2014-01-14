#include "RectangleSearch.h"


/**
 * @brief Constructor
 */
RectangleSearch::RectangleSearch()
{
	l = 0.0;
	r = 0.0;
	b = 0.0;
	t = 0.0;
}


/**
 * @brief Finds all Nodes that fall within a rectangle
 *
 * Finds all Nodes that fall within a rectangle by searching the Nodes
 * in a Quadtree.
 *
 * @param root The highest level of the Quadtree to search
 * @param l The left bound of the rectangle
 * @param r The right bound of the rectangle
 * @param b The bottom bound of the rectangle
 * @param t The top bound of the rectangle
 * @return A list of Nodes that fall within the rectangle
 */
std::vector<Node*> RectangleSearch::FindNodes(branch *root, float l, float r, float b, float t)
{
	fullNodes.clear();
	partialNodes.clear();
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;

	SearchNodes(root);

	BruteForceNodes();

	return fullNodes;
}


/**
 * @brief Finds all Elements that fall within a rectangle
 *
 * Finds all Elements that fall within a rectangle by searching the Elements
 * in a Quadtree.
 *
 * @param root The highest level of the Quadtree to search
 * @param l The left bound of the rectangle
 * @param r The right bound of the rectangle
 * @param b The bottom bound of the rectangle
 * @param t The top bound of the rectangle
 * @return A list of Elements that fall within the rectangle
 */
std::vector<Element*> RectangleSearch::FindElements(branch *root, float l, float r, float b, float t)
{
	fullElements.clear();
	partialElements.clear();
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;

	SearchElements(root);

	BruteForceElements();

	return fullElements;
}


/**
 * @brief Recursively searches through the branch for Nodes that may be part of the selection
 *
 * Recursively searches through the branch for Nodes that may be part of the selection. Nodes
 * that are guaranteed to fall inside of the rectangle are added to the fullNodes list. Nodes
 * that could possibly fall inside of the rectangle are added to the partialNodes list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void RectangleSearch::SearchNodes(branch *currBranch)
{
	int branchCornersInsideRectange = CountCornersInsideRectangle(currBranch);
	if (branchCornersInsideRectange == 4)
	{
		AddToFullNodes(currBranch);
	}
	else if (branchCornersInsideRectange != 0 ||
		 RectangleHasIntersection(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchNodes(currBranch->branches[i]);
			}
			if (currBranch->leaves[i])
			{
				SearchNodes(currBranch->leaves[i]);
			}
		}
	}
}


/**
 * @brief Searches through the leaf for Nodes that may be a part of the selection
 *
 * Searches through the leaf for Nodes that may be a part of the selection. Nodes that are
 * guaranteed to fall inside of the rectangle are added to the fullNodes list. Nodes that
 * could possibly fall inside of the rectangle are added to the partialNodes list.
 *
 * @param currLeaf The leaf to search
 */
void RectangleSearch::SearchNodes(leaf *currLeaf)
{
	int leafCornersInsideRectangle = CountCornersInsideRectangle(currLeaf);
	if (leafCornersInsideRectangle == 4)
	{
		AddToFullNodes(currLeaf);
	}
	else if (leafCornersInsideRectangle != 0 ||
		 RectangleHasIntersection(currLeaf))
	{
		AddToFullNodes(currLeaf);
	}
}


/**
 * @brief Recursively searches through the branch for Elements that may be part of the selection
 *
 * Recursively searches through the branch for Elements that may be part of the selection. Elements
 * that are guaranteed to fall inside of the rectangle are added to the fullElements list. Elements
 * that could possibly fall inside of the rectangle are added to the partialElements list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void RectangleSearch::SearchElements(branch *currBranch)
{
	int branchCornersInsideRectange = CountCornersInsideRectangle(currBranch);
	if (branchCornersInsideRectange == 4)
	{
		AddToFullElements(currBranch);
	}
	else if (branchCornersInsideRectange != 0 ||
		 RectangleHasIntersection(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchElements(currBranch->branches[i]);
			}
			if (currBranch->leaves[i])
			{
				SearchElements(currBranch->leaves[i]);
			}
		}
	}
}


/**
 * @brief Searches through the leaf for Elements that may be a part of the selection
 *
 * Searches through the leaf for Elements that may be a part of the selection. Elements that are
 * guaranteed to fall inside of the rectangle are added to the fullElements list. Elements that
 * could possibly fall inside of the rectangle are added to the partialElements list.
 *
 * @param currLeaf The leaf to search
 */
void RectangleSearch::SearchElements(leaf *currLeaf)
{
	int leafCornersInsideRectangle = CountCornersInsideRectangle(currLeaf);
	if (leafCornersInsideRectangle == 4)
	{
		AddToFullElements(currLeaf);
	}
	else if (leafCornersInsideRectangle != 0 ||
		 RectangleHasIntersection(currLeaf))
	{
		AddToPartialElements(currLeaf);
	}
}


/**
 * @brief Determines if each individual Node in the partialNodes list falls within the rectangle
 *
 * Determines if each individual Node in the partialNodes list falls within the rectangle. If a
 * Node does fall within the rectangle, it is added to the fullNodes list.
 *
 */
void RectangleSearch::BruteForceNodes()
{
	Node *currNode = 0;
	for (std::vector<Node*>::iterator it = partialNodes.begin(); it != partialNodes.end(); ++it)
	{
		currNode = *it;
		if (PointIsInsideRectangle(currNode->normX, currNode->normY))
			fullNodes.push_back(currNode);
	}
}


/**
 * @brief Determines if each individual Element in the partialElements list falls within the rectangle
 *
 * Determines if each individual Element in the partialElements list falls within the rectangle. If
 * an Element does fall within the rectangle, it is added to the fullElements list.
 *
 */
void RectangleSearch::BruteForceElements()
{
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it = partialElements.begin(); it != partialElements.end(); ++it)
	{
		currElement = *it;
		if (PointIsInsideRectangle(currElement->n1->normX, currElement->n1->normY) ||
		    PointIsInsideRectangle(currElement->n2->normX, currElement->n2->normY) ||
		    PointIsInsideRectangle(currElement->n3->normX, currElement->n3->normY))
			fullElements.push_back(currElement);
	}
}


/**
 * @brief Counts the number of corners of a branch that are inside of the rectangle
 *
 * Counts the number of corners of a branch that are inside of the rectangle. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currBranch Pointer to the branch to test
 * @return The number of corners of the branch that are inside of the rectangle
 */
int RectangleSearch::CountCornersInsideRectangle(branch *currBranch)
{
	int count = 0;
	count += PointIsInsideRectangle(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Counts the number of corners of a leaf that are inside of the rectangle
 *
 * Counts the number of corners of a leaf that are inside of the rectangle. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currLeaf Pointer to the leaf to test
 * @return The number of corners of the leaf that are inside of the rectangle
 */
int RectangleSearch::CountCornersInsideRectangle(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsideRectangle(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Determines if the rectangle has an intersection with any edge of a branch
 *
 * Determines if the rectangle has an intersection with any edge of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the rectangle intersects any edge of the branch
 * @return false if the rectangle does not intersect any edge of the branch
 */
bool RectangleSearch::RectangleHasIntersection(branch *currBranch)
{
	return		!(l >= currBranch->bounds[1] ||
			  r <= currBranch->bounds[0] ||
			  b >= currBranch->bounds[3] ||
			  t <= currBranch->bounds[2]);
}


/**
 * @brief Determines if the rectangle has an intersection with any edge of a leaf
 *
 * Determines if the rectangle has an intersection with any edge of a leaf.
 *
 * @param currLeaf The leaf to test
 * @return true if the rectangle intersects any edge of the leaf
 * @return false if the rectangle does not intersect any edge of the leaf
 */
bool RectangleSearch::RectangleHasIntersection(leaf *currLeaf)
{
	return		!(l >= currLeaf->bounds[1] ||
			  r <= currLeaf->bounds[0] ||
			  b >= currLeaf->bounds[3] ||
			  t <= currLeaf->bounds[2]);
}


/**
 * @brief Determines if a point is inside of the rectangle
 *
 * Determines if a point is inside of the rectangle.
 *
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @return true if the point falls inside of the rectangle
 * @return false if the point does not fall inside of the rectangle
 */
bool RectangleSearch::PointIsInsideRectangle(float x, float y)
{
	if (l <= x &&
	    x <= r &&
	    b <= y &&
	    y <= t)
		return true;
	return false;
}


/**
 * @brief Determines if a point is inside the bounding box of a branch
 *
 * Determines if a point is inside the bounding box of a branch.
 *
 * @param point The point to test
 * @param currBranch The branch to test
 * @return true if the point falls within the bounds of the branch
 * @return false if the point does not fall within the bounds of the branch
 */
bool RectangleSearch::PointIsInsideSquare(Point point, branch *currBranch)
{
	if (point.x >= currBranch->bounds[0] &&
	    point.x <= currBranch->bounds[1] &&
	    point.y >= currBranch->bounds[2] &&
	    point.y <= currBranch->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if a point is inside the bounding box of a leaf
 *
 * Determines if a point is inside the bounding box of a leaf.
 *
 * @param point The point to test
 * @param currLeaf The leaf to test
 * @return true if the point falls within the bounds of the leaf
 * @return false if the point does not fall within the bounds of the leaf
 */
bool RectangleSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	if (point.x >= currLeaf->bounds[0] &&
	    point.x <= currLeaf->bounds[1] &&
	    point.y >= currLeaf->bounds[2] &&
	    point.y <= currLeaf->bounds[3])
		return true;
	return false;
}


/**
 * @brief Adds all Nodes below the branch to the fullNodes list
 *
 * Adds all Nodes below the branch to the fullNodes list.
 *
 * @param currBranch The branch below which all Nodes will be added
 */
void RectangleSearch::AddToFullNodes(branch *currBranch)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
		{
			AddToFullNodes(currBranch->branches[i]);
		}
		if (currBranch->leaves[i])
		{
			AddToFullNodes(currBranch->leaves[i]);
		}
	}
}


/**
 * @brief Adds all Nodes in the leaf to the fullNodes list
 *
 * Adds all Nodes in the leaf to the fullNodes list.
 *
 * @param currLeaf The leaf whose Nodes will be added
 */
void RectangleSearch::AddToFullNodes(leaf *currLeaf)
{
	if (currLeaf->nodes.size() > 0)
		fullNodes.insert(fullNodes.end(), currLeaf->nodes.begin(), currLeaf->nodes.end());
}


/**
 * @brief Adds all Nodes in the leaf to the partialNodes list
 *
 * Adds all Nodes in the leaf to the partialNodes list.
 *
 * @param currLeaf The leaf whose Nodes will be added
 */
void RectangleSearch::AddToPartialNodes(leaf *currLeaf)
{
	if (currLeaf->nodes.size() > 0)
		partialNodes.insert(partialNodes.end(), currLeaf->nodes.begin(), currLeaf->nodes.end());
}


/**
 * @brief Adds all Elements below the branch to the fullElements list
 *
 * Adds all Elements below the branch to the fullElements list.
 *
 * @param currBranch The branch below which all Elements will be added
 */
void RectangleSearch::AddToFullElements(branch *currBranch)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
		{
			AddToFullElements(currBranch->branches[i]);
		}
		if (currBranch->leaves[i])
		{
			AddToFullElements(currBranch->leaves[i]);
		}
	}
}


/**
 * @brief Adds all Elements in the leaf to the fullElements list
 *
 * Adds all Elements in the leaf to the fullElements list.
 *
 * @param currLeaf The leaf whose Elements will be added
 */
void RectangleSearch::AddToFullElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		fullElements.insert(fullElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}


/**
 * @brief Adds all Elements in the leaf to the partialElements list
 *
 * Adds all Elements in the leaf to the partialElements list
 *
 * @param currLeaf The leaf whose Elements will be added
 */
void RectangleSearch::AddToPartialElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		partialElements.insert(partialElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}
