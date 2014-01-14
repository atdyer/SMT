#ifndef FORT015_NEW_H
#define FORT015_NEW_H

#include <set>
#include <fstream>
#include <ostream>
#include <sstream>

#include <QObject>

#include "Project/Files/ProjectFile.h"

class Fort015 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort015(QObject *parent=0);
		Fort015(ProjectFile *projectFile, QObject *parent=0);
		Fort015(QString domainName, ProjectFile *projectFile, QObject *parent=0);
//		Fort015(QString domainName, ProjectFile *projectFile, QString targetDir, QObject *parent=0);

//		void	AddInnerBoundaryNodes(std::vector<unsigned int> newNodes);
//		void	AddOuterBoundaryNodes(std::vector<unsigned int> newNodes);
//		void	SetApproach(int approach);
//		void	SetRecordFrequency(int frequency);
//		void	WriteFile();

		void			AddBoundaryNodes(std::vector<unsigned int> newNodes);
		void			AddInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void			AddOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		bool			FileExists();
		std::set<unsigned int>	GetBoundaryNodes();
		std::set<unsigned int>	GetInnerBoundaryNodes();
		std::set<unsigned int>	GetOuterBoundaryNodes();
		int			GetRecordFrequency();
		int			GetSubdomainApproach();
		void			SetSubdomainApproach(int approach);
		void			SetRecordFrequency(int frequency);
		void			WriteFile();

	private:

//		QString			domainName;
//		std::set<unsigned int>	innerBoundaryNodes;
//		bool			isFullDomainFile;
//		std::set<unsigned int>	outerBoundaryNodes;
//		ProjectFile*	projectFile;
//		int			recordFrequency;
//		int			subdomainApproach;

		QString			domainName;
		std::set<unsigned int>	boundaryNodes;
		int			enforceBN;
		std::set<unsigned int>	innerBoundaryNodes;
		bool			isFullDomainFile;
		std::set<unsigned int>	outerBoundaryNodes;
		ProjectFile*	projectFile;
		int			recordFrequency;
		int			subdomainApproach;
		QString			targetFile;

//		QString	GetFilePath();
		void	ReadFile();
};

#endif // FORT015_NEW_H
