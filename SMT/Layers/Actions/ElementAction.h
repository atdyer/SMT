#ifndef ELEMENTACTION_H
#define ELEMENTACTION_H

#include "adcData.h"
#include "Action.h"
#include "Layers/SelectionLayer.h"
#include <map>


/**
 * @brief A subclass of Action that represents the selection/deselection of
 * Elements in the SelectionLayer
 */
class ElementAction : public Action
{
	public:
		ElementAction();
		ElementAction(std::map<unsigned int, Node*> cNodes, std::map<unsigned int, Element*> cElements);

		virtual void	RedoAction();
		virtual void	UndoAction();

	protected:

		std::map<unsigned int, Node*>		nodes;		/**< The map of all Nodes that are used in this Action */
		std::map<unsigned int, Element*>	elements;	/**< The map of all Elements that are used in this Action */
};

#endif // ELEMENTACTION_H
