#include "GradientSliderFrame.h"

GradientSliderFrame::GradientSliderFrame(QWidget *parent) :
	QFrame(parent)
{
	QGradientStop stop1(0.0, QColor::fromRgb(0, 0, 0));
	QGradientStop stop2(1.0, QColor::fromRgb(255, 255, 255));
	gradStops.push_back(stop1);
	gradStops.push_back(stop2);
}


void GradientSliderFrame::SetStops(const QGradientStops &stopPoints)
{
	gradStops = stopPoints;
}


QGradientStops GradientSliderFrame::GetStops()
{
	return gradStops;
}


void GradientSliderFrame::paintEvent(QPaintEvent *)
{
	QImage img(size(), QImage::Format_RGB32);
	img.fill(0);

	QLinearGradient grad(0, 0, 0, height());
	grad.setStops(gradStops);

	QPainter painter(&img);
	painter.fillRect(rect(), grad);

	QPainter p(this);
	p.drawImage(0, 0, img);
	p.setPen(QColor(0, 0, 0));
	p.drawRect(0, 0, width()-1, height()-1);
}
