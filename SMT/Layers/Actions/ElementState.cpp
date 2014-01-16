#include "ElementState.h"


ElementState::ElementState()
{
	elements.clear();
	boundaryData = new Boundaries();
	numNodes = 0;
	numElements = 0;
	minZ = 99999;
	maxZ = -99999;
}


ElementState::ElementState(std::vector<Element *> elementsList)
{
	elements.clear();
	boundaryData = new Boundaries();
	numNodes = 0;
	numElements = 0;
	minZ = 99999;
	maxZ = -99999;
	elements = elementsList;
}


ElementState::~ElementState()
{
	if (boundaryData)
		delete boundaryData;
}


bool ElementState::BoundariesFound()
{
	if (boundaryData)
		return boundaryData->boundariesFound;
	return false;
}


std::vector<Element*>* ElementState::GetState()
{
	return &elements;
}


Boundaries* ElementState::GetBoundaries()
{
	if (!boundaryData->boundariesFound)
	{
		FindBoundaries();
	}

	return boundaryData;
}


void ElementState::FindBoundaries()
{
	BoundaryFinder searchTool;
	searchTool.PerformBoundarySearch(elements, boundaryData);
}
