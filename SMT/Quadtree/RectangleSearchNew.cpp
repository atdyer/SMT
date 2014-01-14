#include "RectangleSearchNew.h"

RectangleSearchNew::RectangleSearchNew()
{
}


void RectangleSearchNew::SetRectangleParameters(float l, float r, float b, float t)
{
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;
	shapeEdgePoint.x = l;
	shapeEdgePoint.y = b;
}


bool RectangleSearchNew::PointIsInsideShape(Point p1)
{
	return (p1.x <= r &&
		p1.x >= l &&
		p1.y <= t &&
		p1.y >= b);
}

bool RectangleSearchNew::ShapeIntersects(branch *currBranch)
{
	if (!(l >= currBranch->bounds[1] ||
	      r <= currBranch->bounds[0] ||
	      b >= currBranch->bounds[3] ||
	      t <= currBranch->bounds[2]))
	{
	     Point BottomLeft(currBranch->bounds[0], currBranch->bounds[2]);
	     Point BottomRight(currBranch->bounds[1], currBranch->bounds[2]);
	     Point TopLeft(currBranch->bounds[0], currBranch->bounds[3]);
	     Point TopRight(currBranch->bounds[1], currBranch->bounds[3]);
	     if (PointIsInsideShape(BottomLeft) &&
		 PointIsInsideShape(BottomRight) &&
		 PointIsInsideShape(TopLeft) &&
		 PointIsInsideShape(TopRight))
		  return false;
	     if (currBranch->contains(Point(b, l)) &&
		 currBranch->contains(Point(b, r)) &&
		 currBranch->contains(Point(t, l)) &&
		 currBranch->contains(Point(t, r)))
		  return false;
	     return true;
	}
	return false;
}

bool RectangleSearchNew::ShapeIntersects(leaf *currLeaf)
{
	if (!(l >= currLeaf->bounds[1] ||
	      r <= currLeaf->bounds[0] ||
	      b >= currLeaf->bounds[3] ||
	      t <= currLeaf->bounds[2]))
	{
	     Point BottomLeft(currLeaf->bounds[0], currLeaf->bounds[2]);
	     Point BottomRight(currLeaf->bounds[1], currLeaf->bounds[2]);
	     Point TopLeft(currLeaf->bounds[0], currLeaf->bounds[3]);
	     Point TopRight(currLeaf->bounds[1], currLeaf->bounds[3]);
	     if (PointIsInsideShape(BottomLeft) &&
		 PointIsInsideShape(BottomRight) &&
		 PointIsInsideShape(TopLeft) &&
		 PointIsInsideShape(TopRight))
		  return false;
	     if (currLeaf->contains(Point(b, l)) &&
		 currLeaf->contains(Point(b, r)) &&
		 currLeaf->contains(Point(t, l)) &&
		 currLeaf->contains(Point(t, r)))
		  return false;
	     return true;
	}
	return false;
}
