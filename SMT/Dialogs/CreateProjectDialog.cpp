#include "CreateProjectDialog.h"
#include "ui_CreateProjectDialog.h"

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateProjectDialog)
{
	ui->setupUi(this);

	connect(ui->projectName, SIGNAL(textChanged(QString)), this, SLOT(OnProjectNameChange(QString)));
	connect(ui->projectDirectory, SIGNAL(textChanged(QString)), this, SLOT(OnProjectDirectoryChange(QString)));
	connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(OnBrowseButtonClick()));
	connect(ui->useSameDir, SIGNAL(toggled(bool)), this, SLOT(SetChooseButtonsActive(bool)));

	connect(ui->chooseFort10, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort11, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort13, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort14, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort15, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort19, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort20, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort22, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort23, SIGNAL(clicked()), this, SLOT(ChooseFile()));
	connect(ui->chooseFort24, SIGNAL(clicked()), this, SLOT(ChooseFile()));

	connect(ui->fort14Loc, SIGNAL(editingFinished()), this, SLOT(Fort14Changed()));

	ui->projectDirectory->setText(QDir::toNativeSeparators((QDir::homePath() + QDir::separator())));

	projectDirectory = ui->projectDirectory->text();
	projectName = ui->projectName->text();

	CheckForMinimumFields();

	ui->projectName->selectAll();
}

CreateProjectDialog::~CreateProjectDialog()
{
	delete ui;
}


QString CreateProjectDialog::GetProjectDirectory()
{
	return projectDirectory;
}


QString CreateProjectDialog::GetProjectName()
{
	return projectName;
}


QString CreateProjectDialog::GetFort10Location()
{
	QFile file (ui->fort10Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort11Location()
{
	QFile file (ui->fort11Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort13Location()
{
	QFile file (ui->fort13Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort14Location()
{
	QFile file (ui->fort14Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort15Location()
{
	QFile file (ui->fort15Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort19Location()
{
	QFile file (ui->fort19Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort20Location()
{
	QFile file (ui->fort20Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort22Location()
{
	QFile file (ui->fort22Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort23Location()
{
	QFile file (ui->fort23Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}


QString CreateProjectDialog::GetFort24Location()
{
	QFile file (ui->fort24Loc->text());
	if (file.exists())
		return QFileInfo(file).absoluteFilePath();
	return QString();
}



bool CreateProjectDialog::GetUseSymbolicLinkFort10()
{
	return ui->link10->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort11()
{
	return ui->link11->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort13()
{
	return ui->link13->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort14()
{
	return ui->link14->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort15()
{
	return ui->link15->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort19()
{
	return ui->link19->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort20()
{
	return ui->link20->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort22()
{
	return ui->link22->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort23()
{
	return ui->link23->isChecked();
}


bool CreateProjectDialog::GetUseSymbolicLinkFort24()
{
	return ui->link24->isChecked();
}


bool CreateProjectDialog::ValidParentDirectory()
{
	return QDir(projectDirectory).exists();
}


bool CreateProjectDialog::ProjectDirExists()
{
	return QDir(projectDirectory+projectName).exists();
}


bool CreateProjectDialog::ProjectFileExists()
{
	QStringList filters ("*.spf");
	QDir dir (projectDirectory+projectName);
	dir.setNameFilters(filters);

	if (dir.entryList().size() == 0)
		return false;
	return true;
}


bool CreateProjectDialog::Fort14FileExists()
{
	return QFile(ui->fort14Loc->text()).exists();
}


bool CreateProjectDialog::Fort15FileExists()
{
	return QFile(ui->fort15Loc->text()).exists();
}


void CreateProjectDialog::FillFields()
{
	QFile fort14File (ui->fort14Loc->text());

	if (fort14File.exists())
	{
		QDir directory = QFileInfo(fort14File).absoluteDir();
		if (directory.exists("fort.10"))
			ui->fort10Loc->setText(directory.absolutePath()+QDir::separator()+"fort.10");
		if (directory.exists("fort.11"))
			ui->fort11Loc->setText(directory.absolutePath()+QDir::separator()+"fort.11");
		if (directory.exists("fort.13"))
			ui->fort13Loc->setText(directory.absolutePath()+QDir::separator()+"fort.13");
		if (directory.exists("fort.15"))
			ui->fort15Loc->setText(directory.absolutePath()+QDir::separator()+"fort.15");
		if (directory.exists("fort.19"))
			ui->fort19Loc->setText(directory.absolutePath()+QDir::separator()+"fort.19");
		if (directory.exists("fort.20"))
			ui->fort20Loc->setText(directory.absolutePath()+QDir::separator()+"fort.20");
		if (directory.exists("fort.22"))
			ui->fort22Loc->setText(directory.absolutePath()+QDir::separator()+"fort.22");
		if (directory.exists("fort.23"))
			ui->fort23Loc->setText(directory.absolutePath()+QDir::separator()+"fort.23");
		if (directory.exists("fort.24"))
			ui->fort24Loc->setText(directory.absolutePath()+QDir::separator()+"fort.24");

	}
}


void CreateProjectDialog::CheckForMinimumFields()
{
	if (ui->projectName->text().isEmpty())
	{
		ui->warnLabel->setText("Choose a Project Name");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}

	if (ValidParentDirectory())
	{
		if (ProjectDirExists() && ProjectFileExists())
		{
			ui->warnLabel->setText("Project Already Exists");
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
			return;
		}
	} else {
		ui->warnLabel->setText("Choose a valid project directory");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}

	if (!Fort14FileExists())
	{
		ui->warnLabel->setText("Choose a valid full domain fort.14 file");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}

	if (!Fort15FileExists())
	{
		ui->warnLabel->setText("Choose a valid full domain fort.15 file");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}

	ui->warnLabel->clear();
	ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}


void CreateProjectDialog::OnProjectNameChange(QString projName)
{
	projectName = projName;
	CheckForMinimumFields();
}


void CreateProjectDialog::OnProjectDirectoryChange(QString projDir)
{
	projectDirectory = projDir;
	CheckForMinimumFields();
}


void CreateProjectDialog::OnBrowseButtonClick()
{
	QStringList selections;
	QFileDialog dialog(this, "Choose the Directory Where the New Project Will Be Created");
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setDirectory(projectDirectory);
	dialog.setModal(true);
	if (dialog.exec())
	{
		selections = dialog.selectedFiles();
		if (!selections.isEmpty())
		{
			QDir dir(selections.first());
			if (dir.exists())
			{
				ui->projectDirectory->setText(dir.path()+QDir::separator());
			}
		}
	}
}


void CreateProjectDialog::SetChooseButtonsActive(bool active)
{
	ui->chooseFort10->setEnabled(!active);
	ui->chooseFort11->setEnabled(!active);
	ui->chooseFort13->setEnabled(!active);
	ui->chooseFort15->setEnabled(!active);
	ui->chooseFort19->setEnabled(!active);
	ui->chooseFort20->setEnabled(!active);
	ui->chooseFort22->setEnabled(!active);
	ui->chooseFort23->setEnabled(!active);
	ui->chooseFort24->setEnabled(!active);
	ui->fort10Loc->setEnabled(!active);
	ui->fort11Loc->setEnabled(!active);
	ui->fort13Loc->setEnabled(!active);
	ui->fort15Loc->setEnabled(!active);
	ui->fort19Loc->setEnabled(!active);
	ui->fort20Loc->setEnabled(!active);
	ui->fort22Loc->setEnabled(!active);
	ui->fort23Loc->setEnabled(!active);
	ui->fort24Loc->setEnabled(!active);

	if (active && !ui->fort14Loc->text().isEmpty())
	{
		FillFields();
	}
}


void CreateProjectDialog::Fort14Changed()
{
	if (ui->useSameDir->isChecked())
		FillFields();
}


void CreateProjectDialog::ChooseFile()
{
	QPushButton *senderButton = static_cast<QPushButton*>(sender());
	QLineEdit *targetLine = 0;
	QString fileType;
	if (senderButton == ui->chooseFort10)
	{
		targetLine = ui->fort10Loc;
		fileType = ".10";
	}
	else if (senderButton == ui->chooseFort11)
	{
		targetLine = ui->fort11Loc;
		fileType = ".11";
	}
	else if (senderButton == ui->chooseFort13)
	{
		targetLine = ui->fort13Loc;
		fileType = ".13";
	}
	else if (senderButton == ui->chooseFort14)
	{
		targetLine = ui->fort14Loc;
		fileType = ".14";
	}
	else if (senderButton == ui->chooseFort15)
	{
		targetLine = ui->fort15Loc;
		fileType = ".15";
	}
	else if (senderButton == ui->chooseFort19)
	{
		targetLine = ui->fort19Loc;
		fileType = ".19";
	}
	else if (senderButton == ui->chooseFort20)
	{
		targetLine = ui->fort20Loc;
		fileType = ".20";
	}
	else if (senderButton == ui->chooseFort22)
	{
		targetLine = ui->fort22Loc;
		fileType = ".22";
	}
	else if (senderButton == ui->chooseFort23)
	{
		targetLine = ui->fort23Loc;
		fileType = ".23";
	}
	else if (senderButton == ui->chooseFort24)
	{
		targetLine = ui->fort24Loc;
		fileType = ".24";
	}

	if (targetLine)
	{
		QFileDialog dlg (this, "Choose fort" + fileType + " file");
		dlg.setFileMode(QFileDialog::ExistingFile);
		QFile currentFile (targetLine->text());
		if (currentFile.exists())
		{
			dlg.setDirectory(QFileInfo(currentFile).absoluteDir());
		} else {
			dlg.setDirectory(projectDirectory);
		}
		dlg.setModal(true);
		if (dlg.exec())
		{
			targetLine->setText(dlg.selectedFiles().first());
			if (targetLine == ui->fort14Loc)
			{
				Fort14Changed();
			}
		}
	}

	CheckForMinimumFields();
}
