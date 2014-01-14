#ifndef QUADTREESEARCH_H
#define QUADTREESEARCH_H

#include "adcData.h"
#include "QuadtreeData.h"
#include <iostream>

class QuadtreeSearch
{
	public:
		QuadtreeSearch();

		std::vector<Node*>	FindNodes(branch *root);
		std::vector<Element*>	FindElements(branch *root);

		// Experimental
		std::vector< std::vector<Node*>*>	FindNodesLists(branch *root);
		std::vector< std::vector<Element*>*>	FindElementsLists(branch *root);
		int	numFollowed;
		int	numLeaves;

	protected:

		Point		shapeEdgePoint;

		virtual bool	PointIsInsideShape(Point p1) = 0;
		virtual bool	ShapeIntersects(branch *currBranch) = 0;
		virtual bool	ShapeIntersects(leaf *currLeaf) = 0;

		std::vector<Node*>	finalNodes;
		std::vector<Node*>	partialNodes;
		std::vector<Element*>	finalElements;
		std::vector<Element*>	partialElements;

		// Experimental
		std::vector<std::vector<Node*>*> finalNodesLists;
		std::vector<std::vector<Node*>*> partialNodesLists;
		std::vector<std::vector<Element*>*> finalElementsLists;
		std::vector<std::vector<Element*>*> partialElementsLists;

		int	SearchNodes(branch *currBranch);
		int	SearchNodes(leaf *currLeaf);
		int	SearchElements(branch *currBranch);
		int	SearchElements(leaf *currLeaf);
		void	BruteForceNodes();
		void	BruteForceElements();
		void	AddAll(branch *currBranch, std::vector<Node*>* nodeList);
		void	AddAll(leaf *currLeaf, std::vector<Node*>* nodeList);
		void	AddAll(branch *currBranch, std::vector<Element*>* elementList);
		void	AddAll(leaf *currLeaf, std::vector<Element*>* elementList);

		// Experimental
		void	SearchNodesPtrs(branch *currBranch);
		void	SearchNodesPtrs(leaf *currLeaf);
		void	SearchElementsPtrs(branch *currBranch);
		void	SearchElementsPtrs(leaf *currLeaf);
		void	BruteForceNodesLists();
		void	BruteForceElementsLists();
		void	AddAllPtrs(branch *currBranch, std::vector<std::vector<Node*>*> *nodeList);
		void	AddAllPtrs(leaf *currLeaf, std::vector<std::vector<Node*>*> *nodeList);
		void	AddAllPtrs(branch *currBranch, std::vector<std::vector<Element*>*> *elementList);
		void	AddAllPtrs(leaf *currLeaf, std::vector<std::vector<Element*>*> *elementList);
};

#endif // QUADTREESEARCH_H
