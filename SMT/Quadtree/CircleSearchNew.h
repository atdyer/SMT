#ifndef CIRCLESEARCHNEW_H
#define CIRCLESEARCHNEW_H

#include "QuadtreeSearch.h"

class CircleSearchNew : public QuadtreeSearch
{
	public:
		CircleSearchNew();

		void	SetCircleParameters(float x, float y, float radius);

	protected:

		virtual bool	PointIsInsideShape(Point p1);
		virtual bool	ShapeIntersects(branch *b);
		virtual bool	ShapeIntersects(leaf *l);

	private:

		float	x, y, r;

		bool	EdgeIntersectsCircle(Point p1, Point p2);
		float	DistanceSquared(float x1, float y1, float x2, float y2);
};

#endif // CIRCLESEARCHNEW_H
