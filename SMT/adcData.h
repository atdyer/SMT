/** @file */


#ifndef ADCDATA_H
#define ADCDATA_H

#include <iostream>


/** QT_DEBUG Macro used for displaying debug output
 *
 * This macro will only print output if compiling/running in debug mode in Qt Creator.
 * It accepts any number of variables separated by the << operator.
 *
 * If you aren't using Qt Creator, you can still print debug output by adding
 * the line
 *
 *    #define QT_DEBUG
 *
 * before this definition.
 *
 */
#ifdef QT_DEBUG
#define DEBUG(x) do {std::cout << x << std::endl;} while(0)
#else
#define DEBUG(x) do {std::cout << x << std::endl;} while(0)
#endif


#define EARTH_RADIUS 6378206.4


/**
 * @brief Defines an ADCIRC node (See fort.14 in ADCIRC manual)
 *
 * Typically, Nodes will be stored in a std::vector in order from lowest node number to highest.
 * ADCIRC currently requires that node numbers are the ordered set from 1 to n, where n is the total
 * number of Nodes in the dataset, so accessing a specific Node by node number is as simple as
 * accessing node [nodeNumber-1].
 *
 */
struct Node
{
		unsigned int nodeNumber;	/**< The node number as defined in fort.14 file */
		float x;	/**< First location coordinate defined in fort.14 file (typically longitude) */
		float y;	/**< Second location coordinate defined in fort.14 file (typically latitude) */
		float z;	/**< Third location coordinate defined in fort.14 file (depth with respect to GEOID) */
		float normX;	/**< The normalized first location coordinate used for drawing operations */
		float normY;	/**< The normalized second location coordinate used for drawing operations */
		float normZ;	/**< The normalized third location coordinate used for drawing operations */
		std::string xDat;
		std::string yDat;
		std::string zDat;
};


/**
 * @brief Defines an ADCIRC element (See fort.14 in ADCIRC manual)
 *
 * Typically, Elements will be stored in a std::vector in order from lowest element number to highest.
 * ADCIRC currently requires that element numbers are the ordered integers from 1 to n, where n is the
 * total number of Elements in the dataset, so accessing a specific Element by element number is as simple
 * as accessing element [elementNumber-1].
 *
 */
struct Element
{
		unsigned int elementNumber;	/**< The element number as defined in fort.14 file */
		Node*	n1;	/**< Pointer to the first Node that makes up the Element */
		Node*	n2;	/**< Pointer to the second Node that makes up the Element */
		Node*	n3;	/**< Pointer to the third Node that makes up the Element */
};


/**
 * @brief A 2-D point in space
 */
struct Point
{
		float x;
		float y;
		Point() : x(0.0), y(0.0) {}
		Point(float a, float b) : x(a), y(b) {}
};


/**
 * @brief Types of actions that the user can perform
 *
 * Types of actions that the user can perform.
 *
 */
enum ActionType {DisplayAction, SelectionAction};


/**
 * @brief Types of tools that the user can use
 *
 * Types of tools that the user can use.
 *
 */
enum ToolType {ClickToolType, CircleToolType, RectangleToolType, PolygonToolType};


/**
 * @brief Types of objects that the user can select
 *
 * Types of objects that the user can select.
 *
 */
enum SelectionType {NodeSelection, ElementSelection};


#endif // ADCDATA_H
