#include "SelectionLayer.h"


/**
 * @brief Constructor
 *
 * Initializes all variables to default values.
 *
 */
SelectionLayer::SelectionLayer(QObject *parent) :
	Layer(parent),
	glLoaded(false),
	camera(0),
	VAOId(0),
	VBOId(0),
	IBOId(0),
	mousePressed(false)
{
}


/**
 * @brief Returns the number of selected Nodes
 *
 * Returns the number of selected Nodes. If not overridden in a subclass,
 * always returns 0.
 *
 * @return 0
 */
unsigned int SelectionLayer::GetNumNodesSelected()
{
	return 0;
}


/**
 * @brief Returns the number of selected Elements
 *
 * Returns the number of selected Elements. If not overridden in a subclass,
 * always returns 0.
 *
 * @return 0
 */
unsigned int SelectionLayer::GetNumElementsSelected()
{
	return 0;
}
