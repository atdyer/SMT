#include "ElementState.h"


ElementState::ElementState()
{
	elements.clear();
}

ElementState::ElementState(std::vector<Element *> elementsList)
{
	elements = elementsList;
}

std::vector<Element*>* ElementState::GetState()
{
	return &elements;
}
