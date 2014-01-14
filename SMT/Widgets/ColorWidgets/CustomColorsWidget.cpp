#include "CustomColorsWidget.h"

CustomColorsWidget::CustomColorsWidget(QWidget *parent) :
	QWidget(parent)
{
	includeTitleRow = true;
	buttonsWide = 8;
	buttonsHigh = 4;
	selectedRow = 0;
	selectedCol = 0;

	CreateLayout();
	CreateButtons();
}


void CustomColorsWidget::CreateLayout()
{
	gridLayout = new QGridLayout(this);
	buttonGroup = new QButtonGroup(this);

}


void CustomColorsWidget::CreateButtons()
{
	int titleRow = 1 ? includeTitleRow : 0;
	for (int row=0; row<buttonsHigh; ++row)
	{
		for (int col=0; col<buttonsWide; ++col)
		{
			ColorButton *newButton = new ColorButton();
			newButton->setEnabled(false);
			newButton->SetRow(row);
			newButton->SetColumn(col);
			newButton->setCheckable(true);
			connect(newButton, SIGNAL(clicked(QColor)), this, SIGNAL(colorClicked(QColor)));
			connect(newButton, SIGNAL(clicked(QColor,int,int)), this, SLOT(setCurrentColor(QColor,int,int)));
			newButton->SetBackgroundColor(QColor::fromRgb(255, 255, 255));
			gridLayout->addWidget(newButton, row+titleRow, col);
			buttons.push_back(newButton);
			buttonGroup->addButton(newButton);
		}
	}

	if (includeTitleRow)
	{
		QLabel *titleLabel = new QLabel("Custom Colors:");
		titleLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
		gridLayout->addWidget(titleLabel, 0, 0, 1, gridLayout->columnCount());
	}

	QPushButton *removeColorButton = new QPushButton();
	removeColorButton->setText("Remove From Custom Colors");
	removeColorButton->setFixedHeight(27);
	connect(removeColorButton, SIGNAL(clicked()), this, SLOT(removeColor()));
	gridLayout->addWidget(removeColorButton, gridLayout->rowCount(), 0, 1, gridLayout->columnCount());
}


void CustomColorsWidget::setCurrentColor(QColor color, int row, int column)
{
	selectedColor = color;
	selectedRow = row;
	selectedCol = column;
}


void CustomColorsWidget::addColor(QColor color)
{
	ColorButton *currentButton = 0;
	for (std::vector<ColorButton*>::iterator it=buttons.begin(); it != buttons.end(); ++it)
	{
		currentButton = *it;
		if (currentButton && !currentButton->isEnabled())
		{
			currentButton->SetBackgroundColor(color);
			currentButton->setEnabled(true);
			break;
		}
	}
}


void CustomColorsWidget::removeColor()
{
	ColorButton *currentButton = 0;
	for (std::vector<ColorButton*>::iterator it=buttons.begin(); it != buttons.end(); ++it)
	{
		currentButton = *it;
		if (currentButton &&
		    currentButton->GetRow() == selectedRow &&
		    currentButton->GetColumn() == selectedCol)
		{
			currentButton->SetBackgroundColor(QColor::fromRgb(255, 255, 255));
			currentButton->setEnabled(false);
			break;
		}
	}
	currentButton = *buttons.begin();
	if (currentButton)
		currentButton->setChecked(true);
}
