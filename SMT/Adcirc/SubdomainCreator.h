#ifndef SUBDOMAINCREATOR_NEW_H
#define SUBDOMAINCREATOR_NEW_H

#include <QString>
#include <QObject>

#include "Project/Domains/FullDomain.h"
#include "Project/Domains/SubDomain.h"
#include "Project/Files/ProjectFile.h"
#include "Project/Files/BNList14.h"
#include "Project/Files/Py140.h"
#include "Project/Files/Py141.h"
#include "SubdomainTools/BoundarySearch.h"

class SubdomainCreator
{
	public:
		SubdomainCreator();
		~SubdomainCreator();

		bool	CreateSubdomain(QString newName,
					ProjectFile *projFile,
					QString targetDir,
					FullDomain *fDomain,
					int version,
					int recordFrequency);

	private:

		BNList14*				bnList;
		Fort015*				fort015Full;
		Fort015*				fort015Sub;
		FullDomain*				fullDomain;
		std::vector<unsigned int>		innerBoundaryNodes;
		std::vector<unsigned int>		outerBoundaryNodes;
		Py140*				py140;
		Py141*				py141;
		ProjectFile*			projectFile;
		std::vector<Element*>			selectedElements;
		std::vector<Node*>			selectedNodes;
		QString					subdomainName;

		bool	CheckForExistingSubdomainFiles(QString targetDir);
		void	FindBoundaries(int version);
		void	FindUniqueNodes();
		void	MapOldToNewElements();
		void	MapOldToNewNodes();
		bool	WriteFort14();

		bool	WarnSubdomainFilesExist(QString targetDir);
};

#endif // SUBDOMAINCREATOR_NEW_H
