#include "CreateSubdomainDialog.h"
#include "ui_CreateSubdomainDialog.h"

CreateSubdomainDialog::CreateSubdomainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateSubdomainDialog)
{
	ui->setupUi(this);

	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui->existsMessage->setVisible(false);

	ui->locationMessage->setStyleSheet("color: rgb(200, 0, 0);");
	ui->existsMessage->setStyleSheet("color: rgb(200, 0, 0);");

	connect(ui->subdomainName, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)));
}

CreateSubdomainDialog::~CreateSubdomainDialog()
{
	delete ui;
}


bool CreateSubdomainDialog::CreateTargetDir()
{
	bool dirSuccess = QDir().mkdir(targetDir);
	std::cout << dirSuccess << std::endl;

	if (dirSuccess)
	{
		return true;
	} else {
		QMessageBox::warning(this, tr("Create New Subdomain"),
				     tr("Unable to create directory for new subomain:\n") + targetDir,
				     QMessageBox::Ok);

		return false;
	}
}


int CreateSubdomainDialog::GetRecordFrequency()
{
	bool check;
	int recordFrequency = ui->recordFrequency->text().toInt(&check);
	if (check)
		return recordFrequency;
	return 0;
}


QString CreateSubdomainDialog::GetSubdomainName()
{
	return ui->subdomainName->text();
}


QString CreateSubdomainDialog::GetSubdomainDirectory()
{
	return targetDir;
}


void CreateSubdomainDialog::SetFullDomainDirectory(QString dir)
{
	fullDir = QDir::cleanPath(dir) + QDir::separator();
	ui->locationMessage->setText(fullDir);
}


void CreateSubdomainDialog::nameChanged(QString newName)
{

	targetDir = fullDir + newName;

	if (targetDir.toStdString() == fullDir.toStdString())
	{
		ui->existsMessage->setText("Enter a Subdomain Name");
		ui->existsMessage->setVisible(true);

	} else {

		ui->existsMessage->setText("Subdomain already exists. Please choose another name.");
		ui->existsMessage->setVisible(false);

		if (QDir(targetDir).exists())
		{
			ui->existsMessage->setVisible(true);
			ui->locationMessage->setStyleSheet("color: rgb(200, 0, 0);");
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		} else {
			ui->existsMessage->setVisible(false);
			ui->locationMessage->setStyleSheet("color: rgb(0, 150, 0);");
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

		}
	}

	ui->locationMessage->setText(targetDir);

}
