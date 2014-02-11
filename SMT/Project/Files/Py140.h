#ifndef PY140_H
#define PY140_H

#include <QObject>

#include <vector>
#include <map>
#include <istream>
#include <fstream>
#include <sstream>

#include "Project/Files/ProjectFile.h"

class Py140 : public QObject
{
		Q_OBJECT
	public:
		explicit Py140(QObject *parent=0);
		Py140(QString domainName, ProjectFile *projectFile, QObject *parent=0);

		void					SaveFile();
		void					SetNewToOld(std::map<unsigned int, unsigned int> newMap);
		void					SetOldToNew(std::map<unsigned int, unsigned int> newMap);

		unsigned int				ConvertNewToOld(unsigned int newNum);
		std::vector<unsigned int>		ConvertNewToOld(std::vector<unsigned int> newVector);
		unsigned int				ConvertOldToNew(unsigned int oldNum);
		std::vector<unsigned int>		ConvertOldToNew(std::vector<unsigned int> oldVector);
		QString					GetFilePath();
		std::map<unsigned int, unsigned int>	GetNewToOld();
		unsigned int				GetNumNodes();
		std::map<unsigned int, unsigned int>	GetOldToNew();
		bool					HasOldNode(unsigned int nodeNum);

	private:

		QString		domainName;
		ProjectFile*	projectFile;
		QString		targetFile;

		std::map<unsigned int, unsigned int>	newToOldNodes;
		std::map<unsigned int, unsigned int>	oldToNewNodes;

		void	ReadFile();
};

#endif // PY140_H
