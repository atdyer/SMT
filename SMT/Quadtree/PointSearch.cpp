#include "PointSearch.h"

PointSearch::PointSearch()
{
}


std::vector<Node*> PointSearch::FindNodes(branch *root)
{
	finalNodes.clear();
	partialNodes.clear();

	SearchNodes(root);
	CustomBruteForceNodes();

	return finalNodes;
}


std::vector<Element*> PointSearch::FindElements(branch *root)
{
	finalElements.clear();
	partialElements.clear();

	SearchElements(root);
	CustomBruteForceElements();

	return finalElements;
}


void PointSearch::SetPointParameters(float x, float y)
{
	this->x = x;
	this->y = y;
	shapeEdgePoint.x = x;
	shapeEdgePoint.y = y;
}


bool PointSearch::PointIsInsideShape(Point p1)
{
	return false;
}


bool PointSearch::ShapeIntersects(branch *currBranch)
{
	return	(x >= currBranch->bounds[0] &&
		 x <= currBranch->bounds[1] &&
		 y >= currBranch->bounds[2] &&
		 y <= currBranch->bounds[3]);
}


bool PointSearch::ShapeIntersects(leaf *currLeaf)
{
	return	(x >= currLeaf->bounds[0] &&
		 x <= currLeaf->bounds[1] &&
		 y >= currLeaf->bounds[2] &&
		 y <= currLeaf->bounds[3]);
}


void PointSearch::CustomBruteForceNodes()
{
	if (partialNodes.size())
	{
		Node *currClosest = partialNodes[0];
		float currDistance = DistanceSquared(currClosest->normX, currClosest->normY);
		for (std::vector<Node*>::iterator currNode = partialNodes.begin() + 1;
		     currNode != partialNodes.end();
		     ++currNode)
		{
			float newDistance = DistanceSquared((*currNode)->normX, (*currNode)->normY);
			if (newDistance < currDistance)
			{
				currClosest = *currNode;
				currDistance = newDistance;
			}
		}
		finalNodes.push_back(currClosest);
	}
}



void PointSearch::CustomBruteForceElements()
{
	if (partialElements.size())
	{
		for (std::vector<Element*>::iterator currElement = partialElements.begin();
		     currElement != partialElements.end();
		     ++currElement)
		{
			if (PointIsInsideElement(*currElement))
			{
				finalElements.push_back(*currElement);
				return;
			}
		}
	}
}


float PointSearch::DistanceSquared(float nodeX, float nodeY)
{
	return ((nodeX-x)*(nodeX-x) + (nodeY-y)*(nodeY-y));
}


bool PointSearch::PointIsInsideElement(Element *currElement)
{
	Node *n1 = currElement->n1;
	Node *n2 = currElement->n2;
	Node *n3 = currElement->n3;
	float a = (n2->normX - n1->normX)*(y - n1->normY) - (x - n1->normX)*(n2->normY - n1->normY);
	float b = (n3->normX - n2->normX)*(y - n2->normY) - (x - n2->normX)*(n3->normY - n2->normY);
	float c = (n1->normX - n3->normX)*(y - n3->normY) - (x - n3->normX)*(n1->normY - n3->normY);


	if ((a > 0 && b > 0 && c > 0) || (a < 0 && b < 0 && c < 0))
		return true;
	return false;
}
