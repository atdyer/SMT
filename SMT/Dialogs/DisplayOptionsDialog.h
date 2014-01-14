#ifndef DISPLAYOPTIONSDIALOG_H
#define DISPLAYOPTIONSDIALOG_H

#include <QDialog>

#include "Project/Domains/Domain.h"

namespace Ui {
	class DisplayOptionsDialog;
}

class DisplayOptionsDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit DisplayOptionsDialog(QWidget *parent = 0);
		~DisplayOptionsDialog();

		void	SetActiveDomain(Domain *newDomain);

		
	private:
		Ui::DisplayOptionsDialog *ui;

		Domain*		currentDomain;

		void	DisconnectCurrentDomain();
		void	DisplayCurrentDomainProperties();
		void	ConnectCurrentDomain();

	signals:

		void	solidOutlineChanged(QColor);
		void	solidFillChanged(QColor);
		void	gradientOutlineChanged(QGradientStops);
		void	gradientFillChanged(QGradientStops);
		void	updateChildrenGeometry();

	public slots:

		void	showSolidOutlineWindow();
		void	showSolidFillWindow();
		void	showGradientOutlineWindow();
		void	showGradientFillWindow();
		void	show();

	private slots:

		void	solidColorChanged(QColor color);
		void	gradientChanged(QGradientStops gradientStops);
};

#endif // DISPLAYOPTIONSDIALOG_H
