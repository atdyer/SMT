#include "Layer.h"

// Initialize static members
unsigned int	Layer::layerCount = 0;
unsigned int	Layer::nextID = 1;


/**
 * @brief Constructor for the Layer class
 *
 * Increments the layerCount and assigns the new layer a unique layerID
 *
 */
Layer::Layer(QObject *parent) : QObject(parent)
{
	layerCount++;
	layerID = nextID;
	nextID++;

	camera = 0;
}


/**
 * @brief Destructor for the Layer class
 *
 * Decrements the layerCount and emits the finishedThreading signal
 *
 */
Layer::~Layer()
{
	layerCount--;
}


/**
 * @brief Returns the unique ID associated with the Layer object
 * @return The unique ID associated with the Layer object
 */
unsigned int Layer::GetID()
{
	return layerID;
}


/**
 * @brief Returns the total number of layers that currently exist
 * @return The number of Layer objects that currently exist
 */
unsigned int Layer::GetNumLayers()
{
	return layerCount;
}
