#include "ColorButton.h"

ColorButton::ColorButton(QWidget *parent) :
	QToolButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(click()));
	row = 0;
	column = 0;
}


ColorButton::~ColorButton()
{

}


void ColorButton::SetBackgroundColor(QColor color)
{
	currentColor = color;

	QString s( QString("ColorButton {")
		   + QString("border: 1px solid gray;")
		   + QString("background: #")
		   + QString(currentColor.red() < 16? "0" : "") + QString::number(currentColor.red(),16)
		   + QString(currentColor.green() < 16? "0" : "") + QString::number(currentColor.green(),16)
		   + QString(currentColor.blue() < 16? "0" : "") + QString::number(currentColor.blue(),16) + QString(";}")
		   + QString("ColorButton:checked {border: 1px solid black}")
		   + QString("ColorButton:hover {border: 1px solid black};"));
	setStyleSheet(s);
	update();
}


void ColorButton::SetRow(int newRow)
{
	row = newRow;
}


void ColorButton::SetColumn(int newCol)
{
	column = newCol;
}


int ColorButton::GetRow()
{
	return row;
}


int ColorButton::GetColumn()
{
	return column;
}


void ColorButton::click()
{
	emit clicked(currentColor.toHsv());
	emit clicked(currentColor.toHsv(), row, column);
}
