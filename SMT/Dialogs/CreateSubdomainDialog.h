#ifndef CREATESUBDOMAINDIALOG_H
#define CREATESUBDOMAINDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QDir>
#include <QCloseEvent>
#include <QMessageBox>
#include <iostream>

namespace Ui {
	class CreateSubdomainDialog;
}

class CreateSubdomainDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit CreateSubdomainDialog(QWidget *parent = 0);
		~CreateSubdomainDialog();

		bool	CreateTargetDir();

		int	GetRecordFrequency();
		QString	GetSubdomainName();
		QString GetSubdomainDirectory();

		void	SetFullDomainDirectory(QString dir);
		
	private:
		Ui::CreateSubdomainDialog *ui;

		QString	fullDir;
		QString targetDir;

	private slots:

		void	nameChanged(QString newName);
};

#endif // CREATESUBDOMAINDIALOG_H
