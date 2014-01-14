#ifndef POLYGONSEARCH_H
#define POLYGONSEARCH_H

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

/**
 * @brief A tool used to search a Quadtree for Nodes or Elements that fall within a polygon
 */
class PolygonSearch
{
	public:

		PolygonSearch();

		std::vector<Node*>	FindNodes(branch *root, std::vector<Point> polyLine);
		std::vector<Element*>	FindElements(branch *root, std::vector<Point> polyLine);
		void	SetPolygonTemp(std::vector<Point> polyLine);
		bool	PointIsInsidePolygon(float x, float y);

	private:

		std::vector<Point>	polygonPoints;		/**< The list of points that make up the polygon */
		std::vector<Node*>	fullNodes;		/**< The list of Nodes inside of the polygon */
		std::vector<Node*>	partialNodes;		/**< The list of Nodes that might fall inside of the polygon */
		std::vector<Element*>	fullElements;		/**< The list of Elements inside of the polygon */
		std::vector<Element*>	partialElements;	/**< The list of Elements that might fall inside of the polygon */

		/* Search Functions */
		void	SearchNodes(branch *currBranch);
		void	SearchNodes(leaf *currLeaf);
		void	SearchElements(branch *currBranch);
		void	SearchElements(leaf *currLeaf);
		void	BruteForceNodes();
		void	BruteForceElements();

		/* Algorithm Functions */
		int	CountCornersInsidePolygon(branch *currBranch);
		int	CountCornersInsidePolygon(leaf *currLeaf);

		bool	PolygonHasPointsInside(branch *currBranch);
		bool	PolygonHasPointsInside(leaf *currLeaf);
		bool	PolygonHasEdgeIntersection(branch *currBranch);
		bool	PolygonHasEdgeIntersection(leaf* currLeaf);

		/* Helper Functions */
//		bool	PointIsInsidePolygon(float x, float y);
		bool	PointIsInsideSquare(Point point, branch *currBranch);
		bool	PointIsInsideSquare(Point point, leaf *currLeaf);
		bool	EdgesIntersect(Point pointA, Point pointB, Point pointC, Point pointD);
		bool	IsCCW(Point A, Point B, Point C);

		/* List Functions */
		void	AddToFullNodes(branch *currBranch);
		void	AddToFullNodes(leaf *currLeaf);
		void	AddToPartialNodes(leaf *currLeaf);
		void	AddToFullElements(branch *currBranch);
		void	AddToFullElements(leaf *currLeaf);
		void	AddToPartialElements(leaf *currLeaf);

};

#endif // POLYGONSEARCH_H
