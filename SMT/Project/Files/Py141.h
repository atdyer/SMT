#ifndef PY141_H
#define PY141_H

#include <QObject>

#include <vector>
#include <map>
#include <istream>
#include <fstream>
#include <sstream>

#include "Project/Files/ProjectFile.h"

class Py141 : public QObject
{
		Q_OBJECT
	public:
		explicit Py141(QObject *parent=0);
		Py141(QString domainName, ProjectFile *projectFile, QObject *parent=0);

		void					SaveFile();
		void					SetNewToOld(std::map<unsigned int, unsigned int> newMap);
		void					SetOldToNew(std::map<unsigned int, unsigned int> newMap);

		unsigned int				ConvertNewToOld(unsigned int newNum);
		std::vector<unsigned int>		ConvertNewToOld(std::vector<unsigned int> newVector);
		unsigned int				ConvertOldToNew(unsigned int oldNum);
		std::vector<unsigned int>		ConvertOldToNew(std::vector<unsigned int> oldVector);
		QString					GetFilePath();
		std::map<unsigned int, unsigned int>	GetNewToOld();
		std::map<unsigned int, unsigned int>	GetOldToNew();

	private:

		QString			domainName;
		ProjectFile*	projectFile;
		QString			targetFile;

		std::map<unsigned int, unsigned int>	newToOldElements;
		std::map<unsigned int, unsigned int>	oldToNewElements;

		void	ReadFile();
};

#endif // PY141_H
