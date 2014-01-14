#include "ColorGradientFrame.h"

ColorGradientFrame::ColorGradientFrame(QWidget *parent) : QFrame(parent)
{
	currentColor.setHsv(0, 0, 200);

	minHue = 0;
	maxHue = 359;
	minSat = 0;
	maxSat = 255;
	minVal = 0;
	maxVal = 255;

	BuildPixMap();
	setFrameShape(QFrame::StyledPanel);
}


QColor ColorGradientFrame::GetSelectedColor()
{
	return currentColor;
}


void ColorGradientFrame::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	drawFrame(&painter);
	QRect rectangle = contentsRect();
	QStyleOptionFrame optionFrame;
	optionFrame.initFrom(this);
	if (optionFrame.state & QStyle::State_Enabled)
	{
		painter.drawPixmap(rectangle.topLeft(), pixMap);
		DrawCrosshair(&painter, crosshairPoint);
	} else {
		QIcon icon(pixMap);
		icon.paint(&painter, rectangle, 0, QIcon::Disabled);
	}
}


void ColorGradientFrame::resizeEvent(QResizeEvent *event)
{
	QFrame::resizeEvent(event);
	BuildPixMap();
	crosshairPoint = QPoint(HueToX(currentColor.hue()), SatToY(currentColor.saturation()));
	update();
}


void ColorGradientFrame::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QPoint point = event->pos();
		int hue = HueFromX(point.x() - contentsRect().x());
		int sat = SatFromY(point.y() - contentsRect().y());
		if (hue < minHue || hue > maxHue || sat < minSat || sat > maxSat)
		{
			return;
		}
		currentColor.setHsv(hue, sat, currentColor.value());
		emit colorPicked(currentColor);
		crosshairPoint = point;
		update();
	} else {
		QFrame::mousePressEvent(event);
	}
}


void ColorGradientFrame::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint point = event->pos();
		QPoint newPoint = point;

		if (point.x() > contentsRect().right())
			newPoint.setX(contentsRect().right());
		else if (point.x() < contentsRect().left())
			newPoint.setX(contentsRect().left());
		if (point.y() < contentsRect().top())
			newPoint.setY(contentsRect().top());
		else if (point.y() > contentsRect().bottom())
			newPoint.setY(contentsRect().bottom());

		int hue = HueFromX(newPoint.x() - contentsRect().x());
		int sat = SatFromY(newPoint.y() - contentsRect().y());
		if (hue < minHue || hue > maxHue || sat < minSat || sat > maxSat)
		{
			return;
		}
		currentColor.setHsv(hue, sat, currentColor.value());
		emit colorPicked(currentColor);
		crosshairPoint = newPoint;
		update();
	} else {
		QFrame::mouseMoveEvent(event);
	}
}


void ColorGradientFrame::BuildPixMap()
{
	int height = contentsRect().height();
	int width = contentsRect().width();
	QImage image (width, height, QImage::Format_RGB32);
	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
		{
			QColor c;
			c.setHsv(HueFromX(x), SatFromY(y), currentColor.value());
			image.setPixel(x, y, c.rgb());
		}
	}
	pixMap = QPixmap::fromImage(image);
}


void ColorGradientFrame::DrawCrosshair(QPainter *p, const QPoint &pt)
{
	p->save();
	p->setPen(Qt::black);
	p->drawLine(pt-QPoint(0, -3), pt-QPoint(0, -1));
	p->drawLine(pt-QPoint(0, 1), pt-QPoint(0, 3));
	p->drawLine(pt-QPoint(-3, 0), pt-QPoint(-1, 0));
	p->drawLine(pt-QPoint(1, 0), pt-QPoint(3, 0));
	p->restore();
}


int ColorGradientFrame::HueFromX(int x)
{
	return maxHue - x * (maxHue - minHue) / contentsRect().width();
}


int ColorGradientFrame::HueToX(int hue)
{
	float xs = contentsRect().width()*1.0/(maxHue-minHue);
	float dist = hue*xs;
	return (int)(contentsRect().width()-dist);
}


int ColorGradientFrame::SatFromY(int y)
{
	return maxSat - y * (maxSat - minSat) / contentsRect().height();
}


int ColorGradientFrame::SatToY(int sat)
{
	float ys = contentsRect().height()*1.0/(maxSat-minSat);
	float dist = sat*ys;
	return (int)(contentsRect().height() - dist);
}


void ColorGradientFrame::setColor(const QColor &c)
{
	currentColor.setHsv(c.hue(), c.saturation(), c.value());
	emit colorPicked(currentColor);
	crosshairPoint = QPoint(HueToX(currentColor.hue()), SatToY(currentColor.saturation()));
	update();
}


void ColorGradientFrame::setHue(int h)
{
	currentColor.setHsv(h, currentColor.saturation(), currentColor.value());
	emit colorPicked(currentColor);
	crosshairPoint = QPoint(HueToX(currentColor.hue()), SatToY(currentColor.saturation()));
	update();
}


void ColorGradientFrame::setSaturation(int s)
{
	currentColor.setHsv(currentColor.hue(), s, currentColor.value());
	emit colorPicked(currentColor);
	crosshairPoint = QPoint(HueToX(currentColor.hue()), SatToY(currentColor.saturation()));
	update();
}


void ColorGradientFrame::setValue(int v)
{
	currentColor.setHsv(currentColor.hue(), currentColor.saturation(), v);
	emit colorPicked(currentColor);
	crosshairPoint = QPoint(HueToX(currentColor.hue()), SatToY(currentColor.saturation()));
	BuildPixMap();
	update();
}


void ColorGradientFrame::valueSliderChanged(int v)
{
	currentColor.setHsv(currentColor.hue(), currentColor.saturation(), v);
	BuildPixMap();
	update();
}
