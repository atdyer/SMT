#include "PolygonSearchNew.h"

PolygonSearchNew::PolygonSearchNew()
{
}

void PolygonSearchNew::SetPolygonParameters(std::vector<Point> polyLine)
{
	polygonPoints = polyLine;
	shapeEdgePoint = polyLine[0];
}


bool PolygonSearchNew::PointIsInsideShape(Point p1)
{
	bool pointIsInside = false;
	unsigned int i, j = 0;
	for (i=0, j=polygonPoints.size()-1; i<polygonPoints.size(); j = i++)
	{
		if (((polygonPoints[i].y > p1.y) != (polygonPoints[j].y > p1.y)) &&
		    (p1.x < (polygonPoints[j].x-polygonPoints[i].x) * (p1.y-polygonPoints[i].y) / (polygonPoints[j].y-polygonPoints[i].y) + polygonPoints[i].x))
			pointIsInside = !pointIsInside;
	}

	return pointIsInside;
}


bool PolygonSearchNew::ShapeIntersects(branch *currBranch)
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


bool PolygonSearchNew::ShapeIntersects(leaf *currLeaf)
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


bool PolygonSearchNew::EdgesIntersect(Point pointA, Point pointB, Point pointC, Point pointD)
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


bool PolygonSearchNew::IsCCW(Point A, Point B, Point C)
{
	if ((C.y-A.y) * (B.x-A.x) > (B.y-A.y) * (C.x-A.x))
		return true;
	return false;
}
