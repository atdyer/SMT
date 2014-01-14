#ifndef RECTANGLESEARCH_H
#define RECTANGLESEARCH_H

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

/**
 * @brief A tool used to search a Quadtree for Nodes or Elements that fall within a rectangle
 */
class RectangleSearch
{
	public:

		RectangleSearch();

		std::vector<Node*>	FindNodes(branch *root, float l, float r, float b, float t);
		std::vector<Element*>	FindElements(branch *root, float l, float r, float b, float t);

	private:

		float l;	/**< The left bound of the rectangle */
		float r;	/**< The right bound of the rectangle */
		float b;	/**< The bottom bound of the rectangle */
		float t;	/**< The top bound of the rectangle */

		/* Searching Lists */
		std::vector<Node*>	fullNodes;		/**< The list of Nodes inside of the rectangle */
		std::vector<Node*>	partialNodes;		/**< The list of Nodes that might fall inside of the rectangle */
		std::vector<Element*>	fullElements;		/**< The list of Elements inside of the rectangle */
		std::vector<Element*>	partialElements;	/**< The list of Elements that might fall inside of the rectange */

		/* Search Functions */
		void	SearchNodes(branch *currBranch);
		void	SearchNodes(leaf *currLeaf);
		void	SearchElements(branch *currBranch);
		void	SearchElements(leaf *currLeaf);
		void	BruteForceNodes();
		void	BruteForceElements();

		/* Algorithm Functions */
		int	CountCornersInsideRectangle(branch *currBranch);
		int	CountCornersInsideRectangle(leaf *currLeaf);
		bool	RectangleHasIntersection(branch *currBranch);
		bool	RectangleHasIntersection(leaf *currLeaf);

		/* Helper Functions */
		bool	PointIsInsideRectangle(float x, float y);
		bool	PointIsInsideSquare(Point point, branch *currBranch);
		bool	PointIsInsideSquare(Point point, leaf *currLeaf);

		/* List Functions */
		void	AddToFullNodes(branch *currBranch);
		void	AddToFullNodes(leaf *currLeaf);
		void	AddToPartialNodes(leaf *currLeaf);
		void	AddToFullElements(branch *currBranch);
		void	AddToFullElements(leaf *currLeaf);
		void	AddToPartialElements(leaf *currLeaf);
};

#endif // RECTANGLESEARCH_H
