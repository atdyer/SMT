#ifndef POLYGONSEARCHNEW_H
#define POLYGONSEARCHNEW_H

#include "QuadtreeSearch.h"

class PolygonSearchNew : public QuadtreeSearch
{
	public:
		PolygonSearchNew();

		void	SetPolygonParameters(std::vector<Point> polyLine);

	protected:

		virtual bool	PointIsInsideShape(Point p1);
		virtual bool	ShapeIntersects(branch *currBranch);
		virtual bool	ShapeIntersects(leaf *currLeaf);

	private:

		std::vector<Point>	polygonPoints;

		bool	EdgesIntersect(Point pointA, Point pointB, Point pointC, Point pointD);
		bool	IsCCW(Point A, Point B, Point C);
};

#endif // POLYGONSEARCHNEW_H
