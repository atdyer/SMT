#ifndef FULLDOMAINRUNOPTIONSDIALOG_H
#define FULLDOMAINRUNOPTIONSDIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
	class FullDomainRunOptionsDialog;
}

class FullDomainRunOptionsDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit FullDomainRunOptionsDialog(QWidget *parent = 0);
		~FullDomainRunOptionsDialog();

		void	SetAdcircExecutable(QString newPath);

		QString	GetAdcircExecutableLocation();
//		int	GetSubdomainApproach();
//		int	GetRecordFrequency();
		int	GetRunEnvironment();

		
	private:
		Ui::FullDomainRunOptionsDialog *ui;

		bool	ExecutableIsValid(QString execLocation);

	private slots:

		void	ChooseAdcircExecutableLocation();
};

#endif // FULLDOMAINRUNOPTIONSDIALOG_H
