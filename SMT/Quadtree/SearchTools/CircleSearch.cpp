#include "CircleSearch.h"


/**
 * @brief Constructor
 */
CircleSearch::CircleSearch()
{
	x = 0.0;
	y = 0.0;
	radius = 0.0;
}


/**
 * @brief Finds all Nodes that fall within a circle
 *
 * Finds all Nodes that fall within a circle by searching the Nodes
 * in a Quadtree.
 *
 * @param root The highest level of the Quadtree to search
 * @param x The x-coordinate of the circle center
 * @param y The y-coordinate of the circle center
 * @param radius The radius of the circle
 * @return A list of Nodes that fall within the circle
 */
std::vector<Node*> CircleSearch::FindNodes(branch *root, float x, float y, float radius)
{
	fullNodes.clear();
	partialNodes.clear();
	this->x = x;
	this->y = y;
	this->radius = radius;

	SearchNodes(root);

	BruteForceNodes();

	return fullNodes;
}


/**
 * @brief Finds all Elements that fall within a circle
 *
 * Finds all Elements that fall within a circle by searching the Elements
 * in a Quadtree.
 *
 * @param root The highest level of the Quadtree to search
 * @param x The x-coordinate of the circle center
 * @param y The y-coordinate of the circle center
 * @param radius The radius of the circle
 * @return A list of Elements that fall within the circle
 */
std::vector<Element*> CircleSearch::FindElements(branch *root, float x, float y, float radius)
{
	fullElements.clear();
	partialElements.clear();
	this->x = x;
	this->y = y;
	this->radius = radius;

	SearchElements(root);

	BruteForceElements();

	return fullElements;
}


/**
 * @brief Recursively searches through the branch for Nodes that may be part of the selection
 *
 * Recursively searches through the branch for Nodes that may be part of the selection. Nodes
 * that are guaranteed to fall inside of the circle are added to the fullNodes list. Nodes
 * that could possibly fall inside of the circle are added to the partialNodes list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void CircleSearch::SearchNodes(branch *currBranch)
{
	int branchCornersInsideCircle = CountCornersInsideCircle(currBranch);
	if (branchCornersInsideCircle == 4)
	{
		AddToFullNodes(currBranch);
	}
	else if (branchCornersInsideCircle != 0 ||
		 CircleIsInsideSquare(currBranch) ||
		 CircleHasEdgeIntersection(currBranch))
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
 * guaranteed to fall inside of the circle are added to the fullNodes list. Nodes that
 * could possibly fall inside of the circle are added to the partialNodes list.
 *
 * @param currLeaf The leaf to search
 */
void CircleSearch::SearchNodes(leaf *currLeaf)
{
	int leafCornersInsideCircle = CountCornersInsideCircle(currLeaf);
	if (leafCornersInsideCircle == 4)
	{
		AddToFullNodes(currLeaf);
	}
	else if (leafCornersInsideCircle != 0 ||
		 CircleIsInsideSquare(currLeaf) ||
		 CircleHasEdgeIntersection(currLeaf))
	{
		AddToPartialNodes(currLeaf);
	}
}


/**
 * @brief Recursively searches through the branch for Elements that may be part of the selection
 *
 * Recursively searches through the branch for Elements that may be part of the selection. Elements
 * that are guaranteed to fall inside of the circle are added to the fullElements list. Elements
 * that could possibly fall inside of the circle are added to the partialElements list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void CircleSearch::SearchElements(branch *currBranch)
{
	int branchCornersInsideCircle = CountCornersInsideCircle(currBranch);
	if (branchCornersInsideCircle == 4)
	{
		AddToFullElements(currBranch);
	}
	else if (branchCornersInsideCircle != 0 ||
		 CircleIsInsideSquare(currBranch) ||
		 CircleHasEdgeIntersection(currBranch))
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
 * guaranteed to fall inside of the circle are added to the fullElements list. Elements that
 * could possibly fall inside of the circle are added to the partialElements list.
 *
 * @param currLeaf The leaf to search
 */
void CircleSearch::SearchElements(leaf *currLeaf)
{
	int leafCornersInsideCircle = CountCornersInsideCircle(currLeaf);
	if (leafCornersInsideCircle == 4)
	{
		AddToFullElements(currLeaf);
	}
	else if (leafCornersInsideCircle != 0 ||
		 CircleIsInsideSquare(currLeaf) ||
		 CircleHasEdgeIntersection(currLeaf))
	{
		AddToPartialElements(currLeaf);
	}
}


/**
 * @brief Determines if each individual Node in the partialNodes list falls within the circle
 *
 * Determines if each individual Node in the partialNodes list falls within the circle. If a
 * Node does fall within the circle, it is added to the fullNodes list.
 *
 */
void CircleSearch::BruteForceNodes()
{
	Node *currNode = 0;
	for (std::vector<Node*>::iterator it = partialNodes.begin(); it != partialNodes.end(); ++it)
	{
		currNode = *it;
		if (PointIsInsideCircle(currNode->normX, currNode->normY))
			fullNodes.push_back(currNode);
	}
}


/**
 * @brief Determines if each individual Element in the partialElements list falls within the circle
 *
 * Determines if each individual Element in the partialElements list falls within the circle. If
 * an Element does fall within the circle, it is added to the fullElements list.
 *
 */
void CircleSearch::BruteForceElements()
{
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it = partialElements.begin(); it != partialElements.end(); ++it)
	{
		currElement = *it;
		if (PointIsInsideCircle(currElement->n1->normX, currElement->n1->normY) ||
		    PointIsInsideCircle(currElement->n2->normX, currElement->n2->normY) ||
		    PointIsInsideCircle(currElement->n3->normX, currElement->n3->normY))
			fullElements.push_back(currElement);
	}
}


/**
 * @brief Counts the number of corners of a branch that are inside of the circle
 *
 * Counts the number of corners of a branch that are inside of the circle. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currBranch Pointer to the branch to test
 * @return The number of corners of the branch that are inside of the circle
 */
int CircleSearch::CountCornersInsideCircle(branch *currBranch)
{
	int count = 0;
	count += PointIsInsideCircle(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsideCircle(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Counts the number of corners of a leaf that are inside of the circle
 *
 * Counts the number of corners of a leaf that are inside of the circle. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currLeaf Pointer to the leaf to test
 * @return The number of corners of the leaf that are inside of the circle
 */
int CircleSearch::CountCornersInsideCircle(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsideCircle(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsideCircle(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Determines if the circle is entirely inside of the bounding square of a branch
 *
 * Determines if the circle is entirely inside of the bounding square of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the circle is entirely inside of the bounding square of the branch
 * @return false if the circle is not entirely inside of the bounding square of the branch
 */
bool CircleSearch::CircleIsInsideSquare(branch *currBranch)
{
	if (PointIsInsideSquare(Point(x-radius, y), currBranch) &&
	    PointIsInsideSquare(Point(x+radius, y), currBranch) &&
	    PointIsInsideSquare(Point(x, y-radius), currBranch) &&
	    PointIsInsideSquare(Point(x, y+radius), currBranch))
		return true;
	return false;
}


/**
 * @brief Determines if the circle is entirely inside of the bounding square of a leaf
 *
 * Determines if the circle is entirely inside of the bounding square of a leaf.
 *
 * @param currLeaf The leaf to test
 * @return true if the circle is entirely inside of the bounding square of the leaf
 * @return false if the circle is not entirely inside of the bounding square of the leaf
 */
bool CircleSearch::CircleIsInsideSquare(leaf *currLeaf)
{
	if (PointIsInsideSquare(Point(x-radius, y), currLeaf) &&
	    PointIsInsideSquare(Point(x+radius, y), currLeaf) &&
	    PointIsInsideSquare(Point(x, y-radius), currLeaf) &&
	    PointIsInsideSquare(Point(x, y+radius), currLeaf))
		return true;
	return false;
}


/**
 * @brief Determines if the circle has an intersection with any edge of a branch
 *
 * Determines if the circle has an intersection with any edge of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the circle intersects any edge of the branch
 * @return false if the circle does not intersect any edge of the branch
 */
bool CircleSearch::CircleHasEdgeIntersection(branch *currBranch)
{
	Point BottomLeft(currBranch->bounds[0], currBranch->bounds[2]);
	Point BottomRight(currBranch->bounds[1], currBranch->bounds[2]);
	Point TopLeft(currBranch->bounds[0], currBranch->bounds[3]);
	Point TopRight(currBranch->bounds[1], currBranch->bounds[3]);

	if (CircleIntersectsEdge(BottomLeft, BottomRight) ||
	    CircleIntersectsEdge(BottomRight, TopRight) ||
	    CircleIntersectsEdge(TopRight, TopLeft) ||
	    CircleIntersectsEdge(TopLeft, BottomLeft))
		return true;

	return false;
}


/**
 * @brief Determines if the circle has an intersection with any edge of a leaf
 *
 * Determines if the circle has an intersection with any edge of a leaf.
 *
 * @param currLeaf The leaf to test
 * @return true if the circle intersects any edge of the leaf
 * @return false if the circle does not intersect any edge of the leaf
 */
bool CircleSearch::CircleHasEdgeIntersection(leaf *currLeaf)
{
	Point BottomLeft(currLeaf->bounds[0], currLeaf->bounds[2]);
	Point BottomRight(currLeaf->bounds[1], currLeaf->bounds[2]);
	Point TopLeft(currLeaf->bounds[0], currLeaf->bounds[3]);
	Point TopRight(currLeaf->bounds[1], currLeaf->bounds[3]);

	if (CircleIntersectsEdge(BottomLeft, BottomRight) ||
	    CircleIntersectsEdge(BottomRight, TopRight) ||
	    CircleIntersectsEdge(TopRight, TopLeft) ||
	    CircleIntersectsEdge(TopLeft, BottomLeft))
		return true;

	return false;
}


/**
 * @brief Determines if a point is inside of the circle
 *
 * Determines if a point is inside of the circle.
 *
 * @param pointX The x-coordinate of the point
 * @param pointY The y-coordinate of the point
 * @return true if the point falls inside of the circle
 * @return false if the point does not fall inside of the circle
 */
bool CircleSearch::PointIsInsideCircle(float pointX, float pointY)
{
	return (pow(pointX - x, 2.0) + (pow(pointY - y, 2.0))) < pow(radius, 2.0);
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
bool CircleSearch::PointIsInsideSquare(Point point, branch *currBranch)
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
bool CircleSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	if (point.x >= currLeaf->bounds[0] &&
	    point.x <= currLeaf->bounds[1] &&
	    point.y >= currLeaf->bounds[2] &&
	    point.y <= currLeaf->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if the circle intersects with an edge
 *
 * Determines if the circle intersects with an edge. Only guaranteed to work
 * for a horizontal or vertical edge.
 *
 * @param A The first point of the edge
 * @param B The second point of the edge
 * @return true if the circle intersects the edge
 * @return false if the circle does not intersect the edge
 */
bool CircleSearch::CircleIntersectsEdge(Point A, Point B)
{
	float left = A.x < B.x ? A.x : B.x;
	float right = A.x < B.x ? B.x : A.x;
	float bottom = A.y < B.y ? A.y : B.y;
	float top = A.y < B.y ? B.y : A.y;

	if (left-radius < x &&
	    x < right + radius &&
	    bottom-radius < y &&
	    y < top + radius)
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
void CircleSearch::AddToFullNodes(branch *currBranch)
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
void CircleSearch::AddToFullNodes(leaf *currLeaf)
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
void CircleSearch::AddToPartialNodes(leaf *currLeaf)
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
void CircleSearch::AddToFullElements(branch *currBranch)
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
void CircleSearch::AddToFullElements(leaf *currLeaf)
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
void CircleSearch::AddToPartialElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		partialElements.insert(partialElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}
