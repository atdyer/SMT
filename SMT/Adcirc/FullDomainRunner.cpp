#include "FullDomainRunner.h"

FullDomainRunner::FullDomainRunner() :
	adcircExecutableLocation(),
	adcircExecutableName(),
	arguments(),
	bash(0),
	fullDomain(0),
	fullDomainPath(),
	runEnvironment(0)
{
}


FullDomainRunner::~FullDomainRunner()
{
	if (bash)
		delete bash;
}


void FullDomainRunner::SetAdcircExecutable(QString newLoc)
{
	adcircExecutableLocation = newLoc;
}


void FullDomainRunner::SetFullDomain(FullDomain *newFull)
{
	if (newFull)
	{
		fullDomain = newFull;
		fullDomainPath = fullDomain->GetPath();
	}
}


bool FullDomainRunner::PrepareForFullDomainRun()
{
	FullDomainRunOptionsDialog dlg;
	dlg.SetAdcircExecutable(adcircExecutableLocation);
	if (dlg.exec())
	{
		adcircExecutableLocation = dlg.GetAdcircExecutableLocation();
		runEnvironment = dlg.GetRunEnvironment();
		std::cout << "ADCIRC: " << adcircExecutableLocation.toStdString() << std::endl;
		return CheckForRequiredFiles();
	}
	return false;
}


bool FullDomainRunner::PerformFullDomainRun()
{
	arguments.clear();
	arguments << "--working-directory="+fullDomainPath;	// Set the working directory of the new terminal
	arguments << "-e";					// Execute command flag
	arguments << "./"+adcircExecutableName;			// Adcirc executable link in project path

	if (bash)
		delete bash;
	bash = new QProcess();

	connect(bash, SIGNAL(finished(int)), this, SLOT(killProcess()));

	bash->start("gnome-terminal", arguments);
	return bash->waitForStarted();
}


bool FullDomainRunner::CheckForRequiredFiles()
{
	QFile adcExe(adcircExecutableLocation);
	adcircExecutableName = QFileInfo(adcExe).fileName();

	/* Check for fort.14, fort.15, fort.015, ln to adcirc */
	bool fort14 = CheckForFile("fort.14");
	bool fort15 = CheckForFile("fort.15");
	bool fort015 = CheckForFile("fort.015");
	bool adcirc = CheckForFile(adcircExecutableName);

	if (adcExe.exists() && !adcirc)
	{
#ifdef Q_OS_WIN32
		adcirc = adcExe.link(fullDomainPath + QDir::separator() + exeName + ".lnk");
#else
		adcirc = adcExe.link(fullDomainPath + QDir::separator() + adcircExecutableName);
#endif
	}

	if (!adcirc || !fort14 || !fort15 || !fort015)
	{
		QString message ("Error - The following files are missing:\n");
		message.append(adcirc ? "" : QString(fullDomainPath) + QDir::separator() + adcircExecutableName + "\n");
		message.append(fort14 ? "" : QString(fullDomainPath) + QDir::separator() + "fort.14\n");
		message.append(fort15 ? "" : QString(fullDomainPath) + QDir::separator() + "fort.15\n");
		message.append(fort015 ? "" : QString(fullDomainPath) + QDir::separator() + "fort.015");
		QMessageBox dlg;
		dlg.setWindowTitle("Run Full Domain");
		dlg.setText(message);
		dlg.setIcon(QMessageBox::Critical);
		dlg.setStandardButtons(QMessageBox::Ok);
		dlg.exec();
		return false;
	}

	return true;
}


bool FullDomainRunner::CheckForFile(QString fileName)
{
	return QFile(fullDomainPath + QDir::separator() + fileName).exists();
}


void FullDomainRunner::killProcess()
{
	if (bash)
		bash->kill();
}

