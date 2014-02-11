#ifndef FORT13_H
#define FORT13_H

#include <QObject>
#include <QString>

#include <vector>
#include <istream>
#include <fstream>
#include <sstream>

#include "Project/Files/ProjectFile.h"
#include "Project/Files/Py140.h"

struct NodalAttribute {
		QString attributeName;
		QString	units;
		QString	valuesPerNode;
		QString	defaultValues;
		std::vector<QString>		nonDefaultLines;

		NodalAttribute() :
			attributeName(),
			units(),
			valuesPerNode(),
			defaultValues(),
			nonDefaultLines()
		{
			nonDefaultLines.clear();
		}
};

class Fort13 : public QObject
{
		Q_OBJECT
	public:
		explicit Fort13(QObject *parent = 0);
		Fort13(ProjectFile *projectFile, QObject *parent=0);
		Fort13(QString domainName, ProjectFile *projectFile, QObject *parent=0);

		QString				GetHeaderLine();
		unsigned int			GetNumNodes();
		unsigned int			GetNumAttributes();
		std::vector<NodalAttribute>	GetNodalAttributes();

		void	SaveFile();
		void	SetHeaderLine(QString newLine);
		void	SetNumNodes(unsigned int newNumNodes);
		void	SetNodalAttributes(std::vector<NodalAttribute> newAttributes, Py140* py140);

	private:

		QString				targetFile;
		ProjectFile*			projectFile;
		QString				domainName;
		bool				isFullDomain;

		QString				headerLine;
		unsigned int			numNodes;
		unsigned int			numAttributes;
		std::vector<NodalAttribute>	attributes;

		void	ReadFile();
		void	WriteFile();
		
};

#endif // FORT13_H
