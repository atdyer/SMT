#include "ConvexCircleSearch.h"

ConvexCircleSearch::ConvexCircleSearch()
{
}


int ConvexCircleSearch::SearchElements(branch *currBranch)
{
	int cornerInShapeCount = 0;
	if (PointIsInsideShape(Point(currBranch->bounds[0], currBranch->bounds[3])))
		++cornerInShapeCount;
	if (PointIsInsideShape(Point(currBranch->bounds[0], currBranch->bounds[2])))
		++cornerInShapeCount;
	if (PointIsInsideShape(Point(currBranch->bounds[1], currBranch->bounds[3])))
		++cornerInShapeCount;
	if (PointIsInsideShape(Point(currBranch->bounds[1], currBranch->bounds[2])))
		++cornerInShapeCount;

	if (cornerInShapeCount == 4)
	{
		AddAll(currBranch, &finalNodes);
	}
	else if (cornerInShapeCount != 0)
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				if (SearchElements(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				if (SearchElements(currBranch->leaves[i]))
					return 1;
			}
		}
	}
	else if (currBranch->contains(shapeEdgePoint))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				if (SearchElements(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				if (SearchElements(currBranch->leaves[i]))
					return 1;
			}
		}
		return 1;
	}
	else if (ShapeIntersects(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				if (SearchElements(currBranch->branches[i]))
					return 1;
			}
		}
		for (int i=0; i<4; ++i)
		{
			if (currBranch->leaves[i])
			{
				if (SearchElements(currBranch->leaves[i]))
					return 1;
			}
		}
	}
	return 0;
}

int ConvexCircleSearch::SearchElements(leaf *currLeaf)
{
	int cornerInShapeCount = 0;
	if (PointIsInsideShape(Point(currLeaf->bounds[0], currLeaf->bounds[3])))
		++cornerInShapeCount;
	if (PointIsInsideShape(Point(currLeaf->bounds[0], currLeaf->bounds[2])))
		++cornerInShapeCount;
	if (PointIsInsideShape(Point(currLeaf->bounds[1], currLeaf->bounds[3])))
		++cornerInShapeCount;
	if (PointIsInsideShape(Point(currLeaf->bounds[1], currLeaf->bounds[2])))
		++cornerInShapeCount;

	if (cornerInShapeCount == 4)
	{
		AddAll(currLeaf, &finalNodes);
	}
	else if (cornerInShapeCount != 0)
	{
		AddAll(currLeaf, &partialNodes);
	}
	else if (currLeaf->contains(shapeEdgePoint))
	{
		AddAll(currLeaf, &partialNodes);
		return 1;
	}
	else if (ShapeIntersects(currLeaf))
	{
		AddAll(currLeaf, &partialNodes);
	}
	return 0;
}
