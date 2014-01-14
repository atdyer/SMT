#include "PolygonSearch.h"


/**
 * @brief Constructor
 */
PolygonSearch::PolygonSearch()
{

}


/**
 * @brief Finds all Nodes that fall within the polygon
 *
 * Finds all Nodes that fall within the polygon by searching the Nodes
 * in a Quadtree.
 *
 * @param root The highest level of the Quadtree to search
 * @param polyLine A list of points that define an arbitrarily shaped polygon
 * @return A list of Nodes that fall within the polygon
 */
std::vector<Node*> PolygonSearch::FindNodes(branch *root, std::vector<Point> polyLine)
{
	fullNodes.clear();
	partialNodes.clear();
	polygonPoints = polyLine;

	SearchNodes(root);

	BruteForceNodes();

	return fullNodes;
}


/**
 * @brief Finds all Elements that fall within the polygon
 *
 * Finds all Elements that fall within the polygon by searching the
 * Elements in a Quadtree
 *
 * @param root The highest level of the Quadtree to search
 * @param polyLine A list of points that define an arbitrarily shaped polygon
 * @return A list of Elements that fall within the polygon
 */
std::vector<Element*> PolygonSearch::FindElements(branch *root, std::vector<Point> polyLine)
{
	fullElements.clear();
	partialElements.clear();
	polygonPoints = polyLine;

	SearchElements(root);

	BruteForceElements();

	return fullElements;
}


/**
 * @brief Recursively searches through the branch for Nodes that may be part of the selection
 *
 * Recursively searches through the branch for Nodes that may be part of the selection. Nodes
 * that are guaranteed to fall inside of the polygon are added to the fullNodes list. Nodes
 * that could possibly fall inside of the polygon are added to the partialNodes list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void PolygonSearch::SearchNodes(branch *currBranch)
{
	int branchCornersInsidePolygon = CountCornersInsidePolygon(currBranch);
	if (branchCornersInsidePolygon == 4)
	{
		AddToFullNodes(currBranch);
	}
	else if (branchCornersInsidePolygon != 0 ||
		 PolygonHasPointsInside(currBranch) ||
		 PolygonHasEdgeIntersection(currBranch))
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
 * @brief Searches through the leaf for Nodes that may be part of the selection
 *
 * Searches through the leaf for Nodes that may be part of the selection. Nodes that are
 * guaranteed to fall inside of the polygon are added to the fullNodes list. Nodes that
 * could possibly fall inside of the polygon are added to the partialNodes list.
 *
 * @param currLeaf The leaf to search
 */
void PolygonSearch::SearchNodes(leaf *currLeaf)
{
	int leafCornersInsidePolygon = CountCornersInsidePolygon(currLeaf);
	if (leafCornersInsidePolygon == 4)
	{
		AddToFullNodes(currLeaf);
	}
	else if (leafCornersInsidePolygon != 0 ||
		 PolygonHasPointsInside(currLeaf) ||
		 PolygonHasEdgeIntersection(currLeaf))
	{
		AddToPartialNodes(currLeaf);
	}
}


/**
 * @brief Recursively searches through the branch for Elements that may be part of the selection
 *
 * Recursively searches through the branch for Elements that may be part of the selection. Elements
 * that are guaranteed to fall inside of the polygon are added to the fullElements list. Elements that
 * could possibly fall inside of the polygon are added to the partialElements list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void PolygonSearch::SearchElements(branch *currBranch)
{

	int branchCornersInsidePolygon = CountCornersInsidePolygon(currBranch);
	if (branchCornersInsidePolygon == 4)
	{
		AddToFullElements(currBranch);
	}
	else if (branchCornersInsidePolygon != 0 ||
		 PolygonHasPointsInside(currBranch) ||
		 PolygonHasEdgeIntersection(currBranch))
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
 * Searches through the leaf for Elements that may be a part of the selection. Elements
 * that are guaranteed to fall inside of the polygon are added to the fullElements list.
 * Elements that could possibly fall inside of the polygon are added to the partialElements
 * list.
 *
 * @param currLeaf The leaf to search
 */
void PolygonSearch::SearchElements(leaf *currLeaf)
{
	int leafCornersInsidePolygon = CountCornersInsidePolygon(currLeaf);
	if (leafCornersInsidePolygon == 4)
	{
		AddToFullElements(currLeaf);
	}
	else if (leafCornersInsidePolygon != 0 ||
		 PolygonHasPointsInside(currLeaf) ||
		 PolygonHasEdgeIntersection(currLeaf))
	{
		AddToPartialElements(currLeaf);
	}
}


/**
 * @brief Determines if each individual Node in the partialNodes list falls within the polygon
 *
 * Determines if each individual Node in the partialNodes list falls within the polygon. If a
 * Node does fall within the polygon, it is added to the fullNodes list.
 *
 */
void PolygonSearch::BruteForceNodes()
{
	Node *currNode = 0;
	for (std::vector<Node*>::iterator it = partialNodes.begin(); it != partialNodes.end(); ++it)
	{
		currNode = *it;
		if (PointIsInsidePolygon(currNode->normX, currNode->normY))
			fullNodes.push_back(currNode);
	}
}


/**
 * @brief Determines if each individual Element in the partialElements list falls within the polygon
 *
 * Determines if each individual Element in the partialElements list falls within the polygon. If
 * an Element does fall within the polygon, it is added to the fullElements list.
 *
 */
void PolygonSearch::BruteForceElements()
{
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it = partialElements.begin(); it != partialElements.end(); ++it)
	{
		currElement = *it;
		if (PointIsInsidePolygon(currElement->n1->normX, currElement->n1->normY) ||
		    PointIsInsidePolygon(currElement->n2->normX, currElement->n2->normY) ||
		    PointIsInsidePolygon(currElement->n3->normX, currElement->n3->normY))
			fullElements.push_back(currElement);
	}
}


/**
 * @brief Counts the number of corners of a branch that are inside of the polygon
 *
 * Counts the number of corners of a branch that are inside of the polygon. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currBranch Pointer to the branch to test
 * @return The number of corners of the branch that are inside of the polygon
 */
int PolygonSearch::CountCornersInsidePolygon(branch *currBranch)
{
	int count = 0;
	count += PointIsInsidePolygon(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsidePolygon(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Counts the number of corners of a leaf are inside of the polygon
 *
 * Counts the number of corners of a leaf that are inside of the polygon. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currLeaf Pointer to the leaf to test
 * @return The number of corners of the leaf that are inside of the polygon
 */
int PolygonSearch::CountCornersInsidePolygon(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsidePolygon(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsidePolygon(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Determines if any of the polygon points are inside of a branch
 *
 * Determines if any of the polygon points are inside of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the polygon has at least one point inside of the branch
 * @return false if the polygon has no points inside of the branch
 */
bool PolygonSearch::PolygonHasPointsInside(branch *currBranch)
{
	for (unsigned int i=0; i<polygonPoints.size(); ++i)
		if (PointIsInsideSquare(polygonPoints[i], currBranch))
			return true;
	return false;
}


/**
 * @brief Determines if any of the polygon points are inside of a leaf
 *
 * Determines if any of the polygon points are inside of a leaf.
 *
 * @param currLeaf The leaf to test
 * @return true if the polygon has at least one point inside of the leaf
 * @return false if the polygon has no points inside of the branch
 */
bool PolygonSearch::PolygonHasPointsInside(leaf *currLeaf)
{
	for (unsigned int i=0; i<polygonPoints.size(); ++i)
		if (PointIsInsideSquare(polygonPoints[i], currLeaf))
			return true;
	return false;
}


/**
 * @brief Determines if any edge of the polygon intersects with any edge of a branch
 *
 * Determines if any edge of the polygon intersects with any edge of a branch.
 *
 * @param currBranch The branch to test
 * @return true if any edge of the polygon intersects with any edge of the branch
 * @return false if none of the edges of the polygon intersect with any edge of the branch
 */
bool PolygonSearch::PolygonHasEdgeIntersection(branch *currBranch)
{
	Point point1(currBranch->bounds[0], currBranch->bounds[2]);	/* Bottom Left */
	Point point2(currBranch->bounds[1], currBranch->bounds[2]);	/* Bottom Right */
	Point point3(currBranch->bounds[0], currBranch->bounds[3]);	/* Top Left */
	Point point4(currBranch->bounds[1], currBranch->bounds[3]);	/* Top Right */
	unsigned int pointCount = polygonPoints.size();

	for (unsigned int i=0; i<pointCount-1; ++i)
		if (EdgesIntersect(point1, point2, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point1, point3, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point3, point4, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point2, point4, polygonPoints[i], polygonPoints[i+1]))
			return true;
	if (EdgesIntersect(point1, point2, polygonPoints[0], polygonPoints[pointCount-1]) ||
	    EdgesIntersect(point1, point3, polygonPoints[0], polygonPoints[pointCount-1]) ||
	    EdgesIntersect(point3, point4, polygonPoints[0], polygonPoints[pointCount-1]) ||
	    EdgesIntersect(point2, point4, polygonPoints[0], polygonPoints[pointCount-1]))
		return true;
	return false;
}


/**
 * @brief Determines if any edge of the polygon intersects with any edge of a branch
 * @param currLeaf
 * @return
 */
bool PolygonSearch::PolygonHasEdgeIntersection(leaf *currLeaf)
{
	Point point1(currLeaf->bounds[0], currLeaf->bounds[2]);	/* Bottom Left */
	Point point2(currLeaf->bounds[1], currLeaf->bounds[2]);	/* Bottom Right */
	Point point3(currLeaf->bounds[0], currLeaf->bounds[3]);	/* Top Left */
	Point point4(currLeaf->bounds[1], currLeaf->bounds[3]);	/* Top Right */
	unsigned int pointCount = polygonPoints.size();

	for (unsigned int i=0; i<pointCount-1; ++i)
		if (EdgesIntersect(point1, point2, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point1, point3, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point3, point4, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point2, point4, polygonPoints[i], polygonPoints[i+1]))
			return true;
	if (EdgesIntersect(point1, point2, polygonPoints[0], polygonPoints[pointCount-1]) ||
	    EdgesIntersect(point1, point3, polygonPoints[0], polygonPoints[pointCount-1]) ||
	    EdgesIntersect(point3, point4, polygonPoints[0], polygonPoints[pointCount-1]) ||
	    EdgesIntersect(point2, point4, polygonPoints[0], polygonPoints[pointCount-1]))
		return true;
	return false;
}


void PolygonSearch::SetPolygonTemp(std::vector<Point> polyLine)
{
	polygonPoints = polyLine;
}


/**
 * @brief Determines if a point is inside of the polygon
 *
 * Determines if a point is inside of the polygon using the ray casting algorithm. Casts
 * a ray out in the positive x-direction, counting the number of edges it crosses.
 * Specific code found at: http://www.ecse.rpi.edu/~wrf/Research/Short_Notes/pnpoly.html
 *
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @return true if the point lies within the polygon
 * @return false if the point does not lie within the polygon
 */
bool PolygonSearch::PointIsInsidePolygon(float x, float y)
{
	bool pointIsInside = false;
	unsigned int i, j = 0;
	for (i=0, j=polygonPoints.size()-1; i<polygonPoints.size(); j = i++)
	{
		if (((polygonPoints[i].y > y) != (polygonPoints[j].y > y)) &&
		    (x < (polygonPoints[j].x-polygonPoints[i].x) * (y-polygonPoints[i].y) / (polygonPoints[j].y-polygonPoints[i].y) + polygonPoints[i].x))
			pointIsInside = !pointIsInside;
	}

	return pointIsInside;
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
bool PolygonSearch::PointIsInsideSquare(Point point, branch *currBranch)
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
bool PolygonSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	if (point.x >= currLeaf->bounds[0] &&
	    point.x <= currLeaf->bounds[1] &&
	    point.y >= currLeaf->bounds[2] &&
	    point.y <= currLeaf->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if the line segment defined by (pointA, pointB) intersects with the
 * line segment formed by the line segment defined by (pointC, pointD)
 *
 * Determines if the line segment defined by (pointA, pointB) intersects with the
 * line segment formed by the line segment defined by (pointC, pointD).
 *
 * @param pointA The first point of the first line segment
 * @param pointB The second point of the first line segment
 * @param pointC The first point of the second line segment
 * @param pointD The second point of the second line segment
 * @return true if the two line segments intersect
 * @return false if the two line segments do not intersect
 */
bool PolygonSearch::EdgesIntersect(Point pointA, Point pointB, Point pointC, Point pointD)
{
	if (IsCCW(pointA, pointC, pointD) == IsCCW(pointB, pointC, pointD))
	{
		return false;
	}
	else if (IsCCW(pointA, pointB, pointC) == IsCCW(pointA, pointB, pointD))
	{
		return false;
	} else {
		return true;
	}
}


/**
 * @brief Determines if the given list of points is in counter-clockwise order
 *
 * Determines if the given list of points is in counter-clockwise order.
 *
 * @param A The first point
 * @param B The second point
 * @param C The third point
 * @return true if the points are given in counter-clockwise order
 * @return false if the points are not give in counter-clockwise order
 */
bool PolygonSearch::IsCCW(Point A, Point B, Point C)
{
	if ((C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x))
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
void PolygonSearch::AddToFullNodes(branch *currBranch)
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
void PolygonSearch::AddToFullNodes(leaf *currLeaf)
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
void PolygonSearch::AddToPartialNodes(leaf *currLeaf)
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
void PolygonSearch::AddToFullElements(branch *currBranch)
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
void PolygonSearch::AddToFullElements(leaf *currLeaf)
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
void PolygonSearch::AddToPartialElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		partialElements.insert(partialElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}
