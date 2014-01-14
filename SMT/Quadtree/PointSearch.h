#ifndef POINTSEARCH_H
#define POINTSEARCH_H

#include "QuadtreeSearch.h"

class PointSearch : public QuadtreeSearch
{
	public:
		PointSearch();

		void			SetPointParameters(float x, float y);
		std::vector<Node*>	FindNodes(branch *root);
		std::vector<Element*>	FindElements(branch *root);

	protected:

		virtual bool	PointIsInsideShape(Point p1);
		virtual bool	ShapeIntersects(branch *currBranch);
		virtual bool	ShapeIntersects(leaf *currLeaf);

	private:

		float x, y;

		void		CustomBruteForceNodes();
		void		CustomBruteForceElements();
		float		DistanceSquared(float nodeX, float nodeY);
		bool		PointIsInsideElement(Element *currElement);
};

#endif // POINTSEARCH_H
