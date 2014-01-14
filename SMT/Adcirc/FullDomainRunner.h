#ifndef FULLDOMAINRUNNER_NEW_H
#define FULLDOMAINRUNNER_NEW_H

#include <QString>
#include <QMessageBox>
#include <QProcess>
#include <iostream>

#include "Dialogs/FullDomainRunOptionsDialog.h"

#include "Project/Domains/FullDomain.h"

class FullDomainRunner : public QObject
{
		Q_OBJECT
	public:
		FullDomainRunner();
		~FullDomainRunner();

		void	SetAdcircExecutable(QString newLoc);
		void	SetFullDomain(FullDomain *newFull);

		bool	PrepareForFullDomainRun();
		bool	PerformFullDomainRun();

	private:

		QString		adcircExecutableLocation;
		QString		adcircExecutableName;
		QStringList	arguments;
		QProcess*	bash;
		FullDomain*	fullDomain;
		QString		fullDomainPath;
		int		runEnvironment;

		bool	CheckForRequiredFiles();
		bool	CheckForFile(QString fileName);

	private slots:

		void	killProcess();

};

#endif // FULLDOMAINRUNNER_NEW_H
