#ifndef VALUESLIDER_H
#define VALUESLIDER_H

#include <QWidget>
#include <QFrame>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QMouseEvent>

class ValueSlider : public QFrame
{
		Q_OBJECT
	public:
		ValueSlider(QWidget *parent = 0);

		QColor	GetCurrentColor();

	protected:

		void	paintEvent(QPaintEvent *);
		void	resizeEvent(QResizeEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);

	private:

		QPoint	trianglePoint;
		QPixmap	pixMap;
		QColor	currentColor;

		int	minHue, maxHue;
		int	minSat, maxSat;
		int	minVal, maxVal;
		int	triangleWidth;
		int	triangleHeight;

		void	BuildPixMap();
		void	DrawTriangle(QPainter *p, const QPoint &pt);
		int	ValFromY(int y);
		int	ValToY(int val);

	signals:

		void	colorPicked(QColor);
		void	valuePicked(int);

	public slots:

		void	setColor(const QColor &c);
};

#endif // VALUESLIDER_H
