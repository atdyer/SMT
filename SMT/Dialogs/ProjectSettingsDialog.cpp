#include "ProjectSettingsDialog.h"
#include "ui_ProjectSettingsDialog.h"

ProjectSettingsDialog::ProjectSettingsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProjectSettingsDialog)
{
	ui->setupUi(this);

	connect(ui->chooseAdcircExecutableButton, SIGNAL(clicked()), this, SLOT(ChooseAdcircExecutableLocation()));
}

ProjectSettingsDialog::~ProjectSettingsDialog()
{
	delete ui;
}


void ProjectSettingsDialog::SetAdcircExecutableLocation(const QString &currentLoc)
{
	ui->adcircExecutableLine->setText(currentLoc);
}


QString ProjectSettingsDialog::GetAdcircExecutableLocation()
{
	return ui->adcircExecutableLine->text();
}


bool ProjectSettingsDialog::ExecutableIsValid(QString execLocation)
{
	if (QFile(execLocation).exists())
		return true;
	return false;
}


void ProjectSettingsDialog::ChooseAdcircExecutableLocation()
{
	QFileDialog dlg (0, "Choose ADCIRC executable", QDir::homePath());
	dlg.setModal(true);
	dlg.setFileMode(QFileDialog::ExistingFile);

	if (dlg.exec())
	{
		QString newLoc = dlg.selectedFiles().first();
		if (ExecutableIsValid(newLoc))
		{
			ui->adcircExecutableLine->setText(newLoc);
		}
	}
}
