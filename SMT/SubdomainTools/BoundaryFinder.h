#ifndef BOUNDARYFINDER_H
#define BOUNDARYFINDER_H

#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "adcData.h"

struct Edge
{
		unsigned int n1;
		unsigned int n2;
		Edge() : n1(0), n2(0) {}
		Edge(unsigned int first, unsigned int second)
		{
			if (first < second)
			{
				n1 = first;
				n2 = second;
			} else {
				n1 = second;
				n2 = first;
			}
		}
		bool operator== (const Edge &e1) const{
			return (n1 == e1.n1 && n2 == e1.n2);
		}
		bool operator< (const Edge &e1) const{
			if (n1 == e1.n1)
				return n2 < e1.n2;
			return n1 < e1.n1;
		}
};


class BoundaryFinder
{
	public:
		/* Constructor */
		BoundaryFinder();
		~BoundaryFinder();

		/* The Callable Search Function */
		void	PerformBoundarySearch(std::vector<Element*> elements, Boundaries *boundaryData);

};

#endif // BOUNDARYFINDER_H
