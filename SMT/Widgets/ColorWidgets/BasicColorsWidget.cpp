#include "BasicColorsWidget.h"

BasicColorsWidget::BasicColorsWidget(QWidget *parent) :
	QWidget(parent)
{	
	includeTitleRow = true;
	buttonsWide = 8;
	buttonsHigh = 6;

	CreateLayout();
	CreateButtons();
}


void BasicColorsWidget::CreateLayout()
{
	gridLayout = new QGridLayout(this);
	buttonGroup = new QButtonGroup(this);
}


void BasicColorsWidget::CreateButtons()
{
	int titleRow = 1 ? includeTitleRow : 0;
	for (int row=0; row<buttonsHigh; ++row)
	{
		for (int col=0; col<buttonsWide; ++col)
		{
			ColorButton *newButton = new ColorButton();
			newButton->setCheckable(true);
			connect(newButton, SIGNAL(clicked(QColor)), this, SIGNAL(colorClicked(QColor)));
			int r = 0;
			int g = 85 * (col/2);
			int b = 0;
			if (row < 3)
			{
				if (col % 2 == 0) r = 0; else r = 170;
			} else {
				if (col % 2 == 0) r = 85; else r = 255;
			}
			if (row == 1 || row == 4)
			{
				b = 127;
			}
			else if (row == 2 || row == 5)
			{
				b = 255;
			}

			newButton->SetBackgroundColor(QColor::fromRgb(r, g, b));

			gridLayout->addWidget(newButton, row+titleRow, col);
			buttonGroup->addButton(newButton);
		}
	}
	if (includeTitleRow)
	{
		QLabel *titleLabel = new QLabel("Basic Colors:");
		titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
		gridLayout->addWidget(titleLabel, 0, 0, 1, gridLayout->columnCount());
	}
}
