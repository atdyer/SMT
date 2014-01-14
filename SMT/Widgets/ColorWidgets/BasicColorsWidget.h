#ifndef BASICCOLORSWIDGET_H
#define BASICCOLORSWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QButtonGroup>
#include "Widgets/ColorWidgets/ColorButton.h"

class BasicColorsWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit BasicColorsWidget(QWidget *parent = 0);

	private:

		QGridLayout*	gridLayout;
		QButtonGroup*	buttonGroup;

		bool	includeTitleRow;
		int	buttonsWide;
		int	buttonsHigh;

		void	CreateLayout();
		void	CreateButtons();
		
	signals:

		void	colorClicked(QColor);
		
	public slots:
		
};

#endif // BASICCOLORSWIDGET_H
