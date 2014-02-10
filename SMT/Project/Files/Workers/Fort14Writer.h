#ifndef FORT14WRITER_H
#define FORT14WRITER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdio.h>

#include <QObject>
#include <QDir>

#include "adcData.h"


/**
 * @brief Used to save changes to nodal data in an existing fort.14 file
 */
class Fort14Writer : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14Writer(QString fileLoc,
				      std::vector<Node> *nodeList,
				      std::vector<Element> *elementList,
				      QObject *parent = 0);
		
	signals:

		void	StartedWriting();
		void	Progress(int);
		void	FinishedWriting(bool);
		
	public slots:

		void	SaveFile();

	private:

		int			currentProgress;
		std::vector<Element>*	elements;
		std::vector<Node>*	nodes;
		QString			targetFile;

		bool	WriteFile();
		
};

#endif // FORT14WRITER_H
