#ifndef QUADTREEDATA_H
#define QUADTREEDATA_H

#include "adcData.h"
#include <vector>

/**
 * @brief Defines a leaf used for grouping Nodes and Elements in the Quadtree data structure
 */
struct leaf
{
		float			bounds[4];	/**< Defines the x-y boundaries of the rectangular leaf */
		std::vector<Node*>	nodes;		/**< A list of pointers to the Nodes in the leaf */
		std::vector<Element*>	elements;	/**< A list of pointers to the Elements in the leaf */

		bool contains(Point p)
		{
			return (p.x >= bounds[0] &&
				p.x <= bounds[1] &&
				p.y >= bounds[2] &&
				p.y <= bounds[3]);
		}
};


/**
 * @brief Defines a branch used for grouping Nodes in the Quadtree data structure
 */
typedef struct branch {
		float		bounds[4];	/**< Defines the x-y boundaries of the rectangular branch */
		leaf		*leaves[4];	/**< A placeholder for the four possible leaves in the branch */
		struct branch	*branches[4];	/**< A placeholder for the four possible branches in the branch */

		bool contains(Point p)
		{
			return (p.x >= bounds[0] &&
				p.x <= bounds[1] &&
				p.y >= bounds[2] &&
				p.y <= bounds[3]);
		}
} branch;


#endif // QUADTREEDATA_H
