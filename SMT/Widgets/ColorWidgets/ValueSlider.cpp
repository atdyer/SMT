#include "ValueSlider.h"

ValueSlider::ValueSlider(QWidget *parent) : QFrame(parent)
{
	currentColor.setHsv(0, 0, 200);
	minHue = 0;
	maxHue = 359;
	minSat = 0;
	maxSat = 255;
	minVal = 0;
	maxVal = 255;
	triangleWidth = 10;
	triangleHeight = 7;

	BuildPixMap();
}


QColor ValueSlider::GetCurrentColor()
{
	return currentColor;
}


void ValueSlider::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	drawFrame(&painter);
	QRect rectangle = contentsRect();
	QStyleOptionFrame optionFrame;
	optionFrame.initFrom(this);
	if (optionFrame.state & QStyle::State_Enabled)
	{
		painter.drawPixmap(rectangle.topLeft(), pixMap);
		DrawTriangle(&painter, trianglePoint);
	} else {
		QIcon icon(pixMap);
		icon.paint(&painter, rectangle, 0, QIcon::Disabled);
	}
}


void ValueSlider::resizeEvent(QResizeEvent *event)
{
	QFrame::resizeEvent(event);
	BuildPixMap();
	trianglePoint = QPoint(contentsRect().width()-triangleWidth, ValToY(currentColor.value()));
	update();
}


void ValueSlider::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QPoint point = event->pos();
		int val = ValFromY(point.y() - contentsRect().y());
		if (val < minVal || val > maxVal)
		{
			return;
		}
		currentColor.setHsv(currentColor.hue(), currentColor.saturation(), val);
		emit colorPicked(currentColor);
		emit valuePicked(currentColor.value());
		trianglePoint = QPoint(contentsRect().width()-triangleWidth, point.y());
		update();
	} else {
		QFrame::mousePressEvent(event);
	}
}


void ValueSlider::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint point = event->pos();
		if (!contentsRect().contains(QPoint(contentsRect().x(), point.y())))
		{
			return;
		}
		int val = ValFromY(point.y() - contentsRect().y());
		if (val < minVal || val > maxVal)
		{
			return;
		}
		currentColor.setHsv(currentColor.hue(), currentColor.saturation(), val);
		emit colorPicked(currentColor);
		emit valuePicked(currentColor.value());
		trianglePoint = QPoint(contentsRect().width()-triangleWidth, event->pos().y());
		update();
	} else {
		QFrame::mouseMoveEvent(event);
	}
}


void ValueSlider::BuildPixMap()
{
	int height = contentsRect().height();
	int width = contentsRect().width()-triangleWidth;
	QImage image (width, height, QImage::Format_RGB32);
	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
		{
			QColor c;
			c.setHsv(currentColor.hue(), currentColor.saturation(), ValFromY(y));
			image.setPixel(x, y, c.rgb());
		}
	}
	pixMap = QPixmap::fromImage(image);
}


void ValueSlider::DrawTriangle(QPainter *p, const QPoint &pt)
{
	p->save();
	p->setPen(Qt::black);
	p->setRenderHint(QPainter::Antialiasing);
	QPolygon triangle;
	triangle << QPoint(pt.x()+2, pt.y());
	triangle << QPoint(pt.x()+triangleWidth+1, pt.y()+triangleHeight);
	triangle << QPoint(pt.x()+triangleWidth+1, pt.y()-triangleHeight);
	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addPolygon(triangle);
	p->drawPath(path);
	p->fillPath(path, Qt::black);
	p->restore();
}


int ValueSlider::ValFromY(int y)
{
	return maxVal - y * (maxVal - minVal) / contentsRect().height();
}


int ValueSlider::ValToY(int val)
{
	float ys = contentsRect().height()*1.0/(maxVal - minVal);
	float dist = val*ys;
	return (int)(contentsRect().height() - dist);
}


void ValueSlider::setColor(const QColor &c)
{
	currentColor.setHsv(c.hue(), c.saturation(), c.value());
	trianglePoint = QPoint(contentsRect().width()-triangleWidth, ValToY(currentColor.value()));
	emit colorPicked(currentColor);
	BuildPixMap();
	update();
}
