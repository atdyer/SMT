#ifndef BOUNDARYCONDITIONSEXTRACTOR_H
#define BOUNDARYCONDITIONSEXTRACTOR_H

#include <vector>

#include <QString>

#include "Project/Files/ProjectFile.h"
#include "Project/Domains/FullDomain.h"
#include "Project/Domains/SubDomain.h"

class BoundaryConditionsExtractor
{
	public:
		BoundaryConditionsExtractor();

		bool	ExtractBoundaryConditions(ProjectFile* file,
						  FullDomain* full,
						  std::vector<SubDomain*> subs);

	private:

		FullDomain*		fullDomain;
		ProjectFile*	projectFile;
		std::vector<SubDomain*>	subDomains;

		bool	AllVersionsCompatible();
		bool	CheckFullDomainFinished();
};

#endif // BOUNDARYCONDITIONSEXTRACTOR_H
