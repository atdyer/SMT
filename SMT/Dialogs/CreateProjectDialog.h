#ifndef CREATEPROJECTDIALOG_H
#define CREATEPROJECTDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>

namespace Ui {
	class CreateProjectDialog;
}

class CreateProjectDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit CreateProjectDialog(QWidget *parent = 0);
		~CreateProjectDialog();

		QString	GetProjectDirectory();
		QString	GetProjectName();

		QString GetFort10Location();
		QString GetFort11Location();
		QString GetFort13Location();
		QString GetFort14Location();
		QString GetFort15Location();
		QString GetFort19Location();
		QString GetFort20Location();
		QString GetFort22Location();
		QString GetFort23Location();
		QString GetFort24Location();

		bool	GetUseSymbolicLinkFort10();
		bool	GetUseSymbolicLinkFort11();
		bool	GetUseSymbolicLinkFort13();
		bool	GetUseSymbolicLinkFort14();
		bool	GetUseSymbolicLinkFort15();
		bool	GetUseSymbolicLinkFort19();
		bool	GetUseSymbolicLinkFort20();
		bool	GetUseSymbolicLinkFort22();
		bool	GetUseSymbolicLinkFort23();
		bool	GetUseSymbolicLinkFort24();

		
	private:
		Ui::CreateProjectDialog *ui;

		QString projectDirectory;
		QString projectName;

		bool	ValidParentDirectory();
		bool	ProjectDirExists();
		bool	ProjectFileExists();
		bool	Fort14FileExists();
		bool	Fort15FileExists();

		void	FillFields();
		void	CheckForMinimumFields();

	private slots:

		void	OnProjectNameChange(QString projName);
		void	OnProjectDirectoryChange(QString projDir);
		void	OnBrowseButtonClick();

		void	SetChooseButtonsActive(bool active);
		void	Fort14Changed();
		void	ChooseFile();


};

#endif // CREATEPROJECTDIALOG_H
