#ifndef PROJECTSETTINGSDIALOG_H
#define PROJECTSETTINGSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>

namespace Ui {
	class ProjectSettingsDialog;
}

class ProjectSettingsDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit ProjectSettingsDialog(QWidget *parent = 0);
		~ProjectSettingsDialog();

		void	SetAdcircExecutableLocation(const QString &currentLoc);

		QString		GetAdcircExecutableLocation();
		
	private:

		Ui::ProjectSettingsDialog *ui;

		bool	ExecutableIsValid(QString execLocation);

	private slots:

		void	ChooseAdcircExecutableLocation();
};

#endif // PROJECTSETTINGSDIALOG_H
