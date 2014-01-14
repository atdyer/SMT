#ifndef CREATESUBDOMAINDIALOG_H
#define CREATESUBDOMAINDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
	class CreateSubdomainDialog;
}

class CreateSubdomainDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit CreateSubdomainDialog(QWidget *parent = 0);
		~CreateSubdomainDialog();

		int	GetRecordFrequency();
		QString	GetSubdomainName();
		int	GetSubdomainVersion();
		QString	GetTargetDirectory();

		void	SetDefaultDirectory(QString dir);
		
	private:
		Ui::CreateSubdomainDialog *ui;

		QString	defaultDir;

	private slots:

		void	BrowseForFolder();
};

#endif // CREATESUBDOMAINDIALOG_H
