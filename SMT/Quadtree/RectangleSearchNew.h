#ifndef RECTANGLESEARCHNEW_H
#define RECTANGLESEARCHNEW_H

#include "QuadtreeSearch.h"
class RectangleSearchNew : public QuadtreeSearch
{
	public:
		RectangleSearchNew();

		void	SetRectangleParameters(float l, float r, float b, float t);

	protected:

		virtual bool	PointIsInsideShape(Point p1);
		virtual bool	ShapeIntersects(branch *currBranch);
		virtual bool	ShapeIntersects(leaf *currLeaf);

	private:

		float l, r, b, t;


};

#endif // RECTANGLESEARCHNEW_H
