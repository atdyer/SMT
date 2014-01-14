#ifndef CLICKSEARCH_H
#define CLICKSEARCH_H

#include <math.h>

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

class ClickSearch
{
	public:
		ClickSearch();

		Node*		FindNode(branch *root, float x, float y);
		Element*	FindElement(branch *root, float x, float y);

//	private:

		/* Point Properties */
		float	x;
		float	y;

		/* Search Functions */
		Node*		SearchNodes(branch *currBranch);
		Node*		SearchNodes(leaf *currLeaf);
		Element*	SearchElements(branch *currBranch);
		Element*	SearchElements(leaf *currLeaf);

		/* Helper Functions */
		bool	PointIsInsideSquare(branch *currBranch);
		bool	PointIsInsideSquare(leaf *currLeaf);
		bool	PointIsInsideElement(Element* currElement);
		float	Distance(Node *currNode);
		float	Distance(Element *currElement);
};

#endif // CLICKSEARCH_H
