#ifndef BOUNDARYSEARCH_H
#define BOUNDARYSEARCH_H

#include <set>
#include <map>
#include <vector>

#include "adcData.h"

#ifndef BOUNDARIES
#define BOUNDARIES
struct Boundaries
{
		std::set<unsigned int>	innerBoundaryNodes;
		std::set<unsigned int>	outerBoundaryNodes;
};
#endif

class BoundarySearch
{

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

	public:
		BoundarySearch();

		Boundaries	FindBoundaries(std::vector<Element> *elements);
		Boundaries	FindBoundaries(std::vector<Element*> elements);
};

#endif // BOUNDARYSEARCH_H
