#ifndef BNLIST14_H
#define BNLIST14_H

#include <QObject>
#include <QDir>
#include <QFile>

#include <vector>
#include <fstream>
#include <sstream>

#include "Project/Files/ProjectFile.h"

class BNList14 : public QObject
{
		Q_OBJECT
	public:
		explicit BNList14(QObject *parent=0);
		BNList14(QString domainName, ProjectFile *projectFile, QObject *parent=0);

		void	SetInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetSubdomainVersion(int v);
		void	SaveFile();

		QString				GetFilePath();
		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
		unsigned int			GetNumInnerBoundaryNodes();
		unsigned int			GetNumOuterBoundaryNodes();
		int				GetSubdomainVersion();

	private:

		QString				domainName;
		std::vector<unsigned int>	innerNodes;
		std::vector<unsigned int>	outerNodes;
		ProjectFile*		projectFile;
		QString				targetFile;
		int				version;

		void	ReadFile();
};

#endif // BNLIST14_H
