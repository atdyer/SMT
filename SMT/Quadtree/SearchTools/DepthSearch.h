#ifndef DEPTHSEARCH_H
#define DEPTHSEARCH_H

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

/**
 * @brief A tool used to search a Quadtree for Nodes or Elements that are within a certain depth of the Quadtree
 */
class DepthSearch
{
	public:

		DepthSearch();

		std::vector< std::vector<Element*> *>	FindElements(branch *root, int depth);
		std::vector< std::vector<Element*> *>	FindElements(branch *root, int depth, float l, float r, float b, float t);

	private:

		float l;	/**< The left bound of the rectangle */
		float r;	/**< The right bound of the rectangle */
		float b;	/**< The bottom bound of the rectangle */
		float t;	/**< The top bound of the rectangle */

		/* Searching Lists */
		std::vector< std::vector<Element*> *>	listOfElementLists;	/**< List of pointers to the vectors containing the desired Elements */

		/* Search Functions */
		void	SearchElements(branch *currBranch, int depth);
		void	SearchElements(leaf *currLeaf);

		/* Algorithm Functions */
		void	RetrieveElements(branch *currBranch, int depth);
		int	CountCornersInsideRectangle(branch *currBranch);
		int	CountCornersInsideRectangle(leaf *currLeaf);
		bool	RectangleHasIntersection(branch *currBranch);
		bool	RectangleHasIntersection(leaf *currLeaf);

		/* Helper Functions */
		bool	PointIsInsideRectangle(float x, float y);
		bool	PointIsInsideSquare(float x, float y, branch *currBranch);
		bool	PointIsInsideSquare(float x, float y, leaf *currLeaf);

		/* List Functions */
		void	AddToListOfElements(leaf *currLeaf);
};

#endif // DEPTHSEARCH_H
