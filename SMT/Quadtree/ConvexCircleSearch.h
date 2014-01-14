#ifndef CONVEXCIRCLESEARCH_H
#define CONVEXCIRCLESEARCH_H

#include "CircleSearchNew.h"

class ConvexCircleSearch : public CircleSearchNew
{
	public:
		ConvexCircleSearch();

	protected:

		int	SearchElements(branch *currBranch);
		int	SearchElements(leaf *currLeaf);
};

#endif // CONVEXCIRCLESEARCH_H
