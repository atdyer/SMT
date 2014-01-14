#include "FullDomainRunOptionsDialog.h"
#include "ui_FullDomainRunOptionsDialog.h"

FullDomainRunOptionsDialog::FullDomainRunOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FullDomainRunOptionsDialog)
{
	ui->setupUi(this);

	ui->runEnvironmentGroup->setId(ui->runEnvironmentManager, 0);
	ui->runEnvironmentGroup->setId(ui->runEnvironmentShell, 1);
	ui->runEnvironmentGroup->setId(ui->runEnvironmentHere, 2);

	connect(ui->chooseAdcircExecutableButton, SIGNAL(clicked()), this, SLOT(ChooseAdcircExecutableLocation()));
}

FullDomainRunOptionsDialog::~FullDomainRunOptionsDialog()
{
	delete ui;
}


void FullDomainRunOptionsDialog::SetAdcircExecutable(QString newPath)
{
	ui->adcircExecutable->setText(newPath);
}


QString FullDomainRunOptionsDialog::GetAdcircExecutableLocation()
{
	return ui->adcircExecutable->text();
}


//int FullDomainRunOptionsDialog::GetSubdomainApproach()
//{
//	if (ui->subdomainMethodSpin->currentText() == "New Approach")
//		return 2;
//	else
//		return 1;
//}


//int FullDomainRunOptionsDialog::GetRecordFrequency()
//{
//	return ui->recordFrequency->value();
//}


int FullDomainRunOptionsDialog::GetRunEnvironment()
{
	return ui->runEnvironmentGroup->checkedId();
}


bool FullDomainRunOptionsDialog::ExecutableIsValid(QString execLocation)
{
	if (QFile(execLocation).exists())
		return true;
	return false;
}


void FullDomainRunOptionsDialog::ChooseAdcircExecutableLocation()
{
	QFileDialog dlg (0, "Choose ADCIRC executable", QDir::homePath());
	dlg.setModal(true);
	dlg.setFileMode(QFileDialog::ExistingFile);

	if (dlg.exec())
	{
		QString newLoc = dlg.selectedFiles().first();
		if (ExecutableIsValid(newLoc))
		{
			ui->adcircExecutable->setText(newLoc);
		}
	}
}
