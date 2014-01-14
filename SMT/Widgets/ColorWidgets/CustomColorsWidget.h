#ifndef CUSTOMCOLORSWIDGET_H
#define CUSTOMCOLORSWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include "Widgets/ColorWidgets/ColorButton.h"

class CustomColorsWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit CustomColorsWidget(QWidget *parent = 0);
		
	private:

		QGridLayout*			gridLayout;
		QButtonGroup*			buttonGroup;
		std::vector<ColorButton*>	buttons;

		QColor	selectedColor;
		int	selectedRow;
		int	selectedCol;

		bool	includeTitleRow;
		int	buttonsWide;
		int	buttonsHigh;

		void	CreateLayout();
		void	CreateButtons();

	signals:

		void	colorClicked(QColor);

		
	public slots:

		void	setCurrentColor(QColor color, int row, int column);
		void	addColor(QColor color);
		void	removeColor();
		
};

#endif // CUSTOMCOLORSWIDGET_H
