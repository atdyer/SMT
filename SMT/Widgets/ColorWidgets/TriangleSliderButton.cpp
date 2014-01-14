#include "TriangleSliderButton.h"

unsigned int	TriangleSliderButton::nextID = 1;
int		TriangleSliderButton::sliderWidth = 21;
int		TriangleSliderButton::sliderHeight = 21;

TriangleSliderButton::TriangleSliderButton(QWidget *parent) :
	QPushButton(parent),
	sliderID(nextID),
	currentValue(0.0),
	triangleWidth(sliderWidth),
	triangleHeight(sliderHeight)
{
	++nextID;

	triangleColor = QColor::fromRgb(0, 0, 0);

	isRemovable = false;

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFixedSize(triangleWidth, triangleHeight);
}


unsigned int TriangleSliderButton::GetID()
{
	return sliderID;
}


void TriangleSliderButton::SetRemovable(bool newRemovable)
{
	isRemovable = newRemovable;
}


void TriangleSliderButton::SetColor(QColor newColor)
{
	triangleColor = newColor;
	update();
}


void TriangleSliderButton::SetValue(float newValue)
{
	currentValue = newValue;
}


QColor TriangleSliderButton::GetColor()
{
	return triangleColor;
}


float TriangleSliderButton::GetValue()
{
	return currentValue;
}


void TriangleSliderButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);

	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addPolygon(triangle);
	painter.drawPath(path);
	painter.fillPath(path, triangleColor);
}


void TriangleSliderButton::resizeEvent(QResizeEvent *event)
{
	QPushButton::resizeEvent(event);
	BuildTriangle();
}


void TriangleSliderButton::mousePressEvent(QMouseEvent *event)
{
	lastMousePoint = event->globalPos();
	emit sliderPressed(sliderID);
}


void TriangleSliderButton::mouseReleaseEvent(QMouseEvent *event)
{
	emit sliderReleased(sliderID);
	if (event->button() == Qt::LeftButton && event->globalPos() == lastMousePoint)
	{
		SelectNewColor();
	}
}


void TriangleSliderButton::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu contextMenu;
	QAction *changeColorAction = contextMenu.addAction("Change Color");
	connect(changeColorAction, SIGNAL(triggered()), this, SLOT(requestColorChange()));
	if (isRemovable)
	{
		QAction *removeColorAction = contextMenu.addAction("Remove Slider");
		connect(removeColorAction, SIGNAL(triggered()), this, SLOT(requestRemoval()));
	}
	QPoint location = mapToGlobal(event->pos());
	contextMenu.exec(location);
}


void TriangleSliderButton::BuildTriangle()
{
	int width = contentsRect().width();
	int height = contentsRect().height();

	triangle.clear();

	triangle << QPoint(width-1, 0);
	if (height%2 == 1)
	{
		triangle << QPoint(0, (width-1)/2);
	} else {
		triangle << QPoint(0, width/2);
	}
	triangle << QPoint(width-1, height-1);
	triangle << QPoint(width-1, 0);
}


void TriangleSliderButton::SelectNewColor()
{
	QColor newColor = QColorDialog::getColor(triangleColor, this, "Choose Slider Color", QColorDialog::ShowAlphaChannel);
	if (newColor.isValid())
	{
		SetColor(newColor);
		emit colorChanged(sliderID, newColor);
	}
}


void TriangleSliderButton::requestColorChange()
{
	SelectNewColor();
}


void TriangleSliderButton::requestRemoval()
{
	emit removeSlider(sliderID);
}

