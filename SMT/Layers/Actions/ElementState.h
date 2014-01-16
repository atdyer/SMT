#ifndef ELEMENTSTATEACTION_H
#define ELEMENTSTATEACTION_H

#include "adcData.h"
#include "SubdomainTools/BoundaryFinder.h"
#include <vector>

class ElementState
{
	public:
		// Constructors
		ElementState();
		ElementState(std::vector<Element*> elementsList);
		~ElementState();

		// Access Function
		bool			BoundariesFound();
		std::vector<Element*>*	GetState();
		Boundaries*		GetBoundaries();

	protected:

		std::vector<Element*>	elements;
		Boundaries*		boundaryData;
		int			numNodes;
		int			numElements;
		float			minZ;
		float			maxZ;

	private:

		void	FindBoundaries();
};

#endif // ELEMENTSTATEACTION_H
