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


/**
 * @brief Creates a new subdomain from an existing full domain
 *
 * This class is used to create new subdomains from an existing full domain.
 * The following steps are followed when creating a subdomain:
 *
 * - Check the full domain for a fort.015 file. If it has one already,
 *   the full domain already has subdomains and the same subdomain version
 *   (and record frequency) must be used. If the user is trying to
 *   use a different subdomain version, alert them that the
 *   appropriate one will be automatically used.
 * - The currently selected elements are retrieved from the full domain,
 *   along with the boundaries of the selection. Some sanity checks are
 *   performed on the selection just to be sure that a valid subdomain
 *   is being created.
 * - Create new node and element numbers for the subdomain and write the
 *   py.140 and py.141 files.
 * - Get the appropriate boundary nodes (outer for version 1, inner and outer
 *   for version 2) and write the bnlist.14 file.
 * - Create the new fort.14 file
 * - Create the new subdomain fort.015 file
 * - Check for a full domain fort.13 file. If one exists, create the subdomain
 *   fort.13 file
 * - Update the project file with all newly created file locations.
 *
 *
 * Because of minor nuances that keep arising between the version 1 and version 2
 * methods, a separate function has been created for each. They are extremely
 * similar but making modifications to each will be more simple moving forward
 * if they are separated.
 *
 * Note: Will probably rethink fort.015 process. It seems it would probably be
 *	 more useful to set the subdomain version and record frequency at the
 *	 start of a project and then never ask the user again for those values.
 *
 */
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

//		BNList14*				bnList;
//		Fort015*				fort015Full;
//		Fort015*				fort015Sub;
		FullDomain*				fullDomain;
//		std::vector<unsigned int>		innerBoundaryNodes;
//		std::vector<unsigned int>		outerBoundaryNodes;
//		Py140*					py140;
//		Py141*					py141;
		ProjectFile*				projectFile;
//		std::vector<Element*>			selectedElements;
//		std::vector<Node*>			selectedNodes;
		QString					subdomainName;

		// Methods for creating version 1 subdomains
		bool	CreateSubdomainVersion1(QString targetDir, int recordFrequency);
		bool	CreateBNListVersion1(std::vector<unsigned int> boundaryNodes, Py140 *py140);

		// Methods for creating version 2 subdomains
		bool	CreateSubdomainVersion2(QString targetDir, int recordFrequency);
		bool	CreateBNListVersion2(std::vector<unsigned int> innerBoundaryNodes, std::vector<unsigned int> outerBoundaryNodes, Py140 *py140);

		// Methods that apply to creating both versions
		Fort015*		GetFullDomainFort015(int version, int recordFrequency);
		std::vector<Node*>	GetSelectedNodes(std::vector<Element*> selectedElements);
		Fort13*			CreateFort13(Py140 *py140);
		bool			CreateFort14(std::vector<Node *> selectedNodes, std::vector<Element*> selectedElements,
						     Py140 *py140, Py141 *py141);
		Py140*			CreatePy140(std::vector<Node*> selectedNodes);
		Py141*			CreatePy141(std::vector<Element*> selectedElements);

		// Older methods
		bool	CheckForExistingSubdomainFiles(QString targetDir);
//		void	FindBoundaries(int version);
//		void	FindUniqueNodes();
//		void	MapOldToNewElements();
//		void	MapOldToNewNodes();
//		bool	WriteFort14();

		bool	WarnSubdomainFilesExist(QString targetDir);
		bool	WarnFort015Mismatch(int requestedVersion, int actualVersion,
					    int requestedFrequency, int actualFrequency);
};

#endif // SUBDOMAINCREATOR_NEW_H
