#ifndef ELEMENTSTATEACTION_H
#define ELEMENTSTATEACTION_H

#include "adcData.h"
#include <vector>

class ElementState
{
	public:
		// Constructors
		ElementState();
		ElementState(std::vector<Element*> elementsList);

		// Access Function
		std::vector<Element*>*	GetState();

	protected:

		std::vector<Element*>	elements;
};

#endif // ELEMENTSTATEACTION_H
