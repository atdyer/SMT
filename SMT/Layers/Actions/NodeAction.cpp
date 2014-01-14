#include "NodeAction.h"

NodeAction::NodeAction()
{

}


/**
 * @brief Constructor used to create a new Action from a map of Nodes
 *
 * This constructor should be used when creating a new NodeAction. It stores
 * the the map of Nodes that will be used when performing actions.
 *
 * @param cNodes The map of Nodes used to perform actions
 */
NodeAction::NodeAction(std::map<unsigned int, Node *> cNodes)
{
	nodes = cNodes;
}


/**
 * @brief Selects all Nodes
 *
 * If the SelectionLayer has been set and the map of Nodes is not empty,
 * this function adds all of the Nodes in the map to the list of Nodes
 * to be drawn by the SelectionLayer
 *
 */
void NodeAction::RedoAction()
{
//	if (selectionLayer && nodes.size() > 0)
//	{
////		selectionLayer->SelectNodes(nodes);
//	}
}


/**
 * @brief Deselects all Nodes
 *
 * If the SelectionLayer has been set and the map of Nodes is not empty,
 * this function removes all of the Nodes in the map from the list of
 * Nodes to be drawn by the SelectionLayer
 *
 */
void NodeAction::UndoAction()
{
//	if (selectionLayer && nodes.size() > 0)
//	{
////		selectionLayer->DeselectNodes(nodes);
//	}
}
