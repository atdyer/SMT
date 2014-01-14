#include "CreateSubdomainDialog.h"
#include "ui_CreateSubdomainDialog.h"

CreateSubdomainDialog::CreateSubdomainDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateSubdomainDialog)
{
	ui->setupUi(this);

	connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(BrowseForFolder()));
}

CreateSubdomainDialog::~CreateSubdomainDialog()
{
	delete ui;
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


int CreateSubdomainDialog::GetSubdomainVersion()
{
	if (ui->subdomainVersion->currentIndex() == 0)
		return 1;
	else
		return 2;
}


QString CreateSubdomainDialog::GetTargetDirectory()
{
	return ui->targetDir->text();
}


void CreateSubdomainDialog::SetDefaultDirectory(QString dir)
{
	defaultDir = dir;
}


void CreateSubdomainDialog::BrowseForFolder()
{
	QStringList selections;
	QFileDialog dialog(0, "Choose New Subdomain Directory", QDir::homePath());
	dialog.setModal(true);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly, true);
	dialog.setDirectory(defaultDir);
	if (dialog.exec())
	{
		selections = dialog.selectedFiles();
		if (!selections.isEmpty())
		{
			ui->targetDir->setText(selections.first());
		}
	} else {
		return;
	}
}
