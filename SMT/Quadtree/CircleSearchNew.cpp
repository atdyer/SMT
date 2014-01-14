#include "CircleSearchNew.h"

CircleSearchNew::CircleSearchNew()
{
}


void CircleSearchNew::SetCircleParameters(float x, float y, float radius)
{
	this->x = x;
	this->y = y;
	this->r = radius;
	shapeEdgePoint.x = x+radius;
	shapeEdgePoint.y = y;
}


bool CircleSearchNew::PointIsInsideShape(Point p1)
{
	return DistanceSquared(x, y, p1.x, p1.y) < r*r;
}


bool CircleSearchNew::ShapeIntersects(branch *b)
{
	Point BottomLeft(b->bounds[0], b->bounds[2]);
	Point BottomRight(b->bounds[1], b->bounds[2]);
	Point TopLeft(b->bounds[0], b->bounds[3]);
	Point TopRight(b->bounds[1], b->bounds[3]);

	return EdgeIntersectsCircle(BottomLeft, BottomRight) ||
	       EdgeIntersectsCircle(BottomRight, TopRight) ||
	       EdgeIntersectsCircle(TopRight, TopLeft) ||
	       EdgeIntersectsCircle(TopLeft, BottomLeft);
}


bool CircleSearchNew::ShapeIntersects(leaf *l)
{
	Point BottomLeft(l->bounds[0], l->bounds[2]);
	Point BottomRight(l->bounds[1], l->bounds[2]);
	Point TopLeft(l->bounds[0], l->bounds[3]);
	Point TopRight(l->bounds[1], l->bounds[3]);

	return EdgeIntersectsCircle(BottomLeft, BottomRight) ||
	       EdgeIntersectsCircle(BottomRight, TopRight) ||
	       EdgeIntersectsCircle(TopRight, TopLeft) ||
	       EdgeIntersectsCircle(TopLeft, BottomLeft);
}


bool CircleSearchNew::EdgeIntersectsCircle(Point p1, Point p2)
{
	float dist1 = DistanceSquared(x, y, p1.x, p1.y);
	float dist2 = DistanceSquared(x, y, p2.x, p2.y);
	bool p1Inside = dist1 <= r*r;
	bool p2Inside = dist2 <= r*r;
	if (p1Inside && p2Inside)
	{
		return false;
	}
	else if (p1Inside != p2Inside)
	{
		return true;
	}

	float xDiff = p2.x - p1.x;
	float yDiff = p2.y - p1.y;
	float d2 = xDiff*xDiff + yDiff*yDiff;
	float u = (xDiff * (x - p1.x) + yDiff * (y - p1.y)) / d2;
	if (u >= 0.0 && u <= 1.0)
	{
		float closestX = p1.x + u*xDiff;
		float closestY = p1.y + u*yDiff;
		return DistanceSquared(x, y, closestX, closestY) <= r*r;
	} else {
		return (DistanceSquared(x, y, p1.x, p1.y) <= r*r ||
			DistanceSquared(x, y, p2.x, p2.y) <= r*r);
	}
}


float CircleSearchNew::DistanceSquared(float x1, float y1, float x2, float y2)
{
	return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
}
