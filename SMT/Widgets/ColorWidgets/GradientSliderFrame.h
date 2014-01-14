#ifndef GRADIENTSLIDERFRAME_H
#define GRADIENTSLIDERFRAME_H

#include <QFrame>
#include <QLinearGradient>
#include <QPainter>
#include <QBrush>

#include <iostream>

class GradientSliderFrame : public QFrame
{
		Q_OBJECT
	public:
		explicit GradientSliderFrame(QWidget *parent = 0);

		void		SetStops(const QGradientStops & stopPoints);
		QGradientStops	GetStops();

	protected:

		void	paintEvent(QPaintEvent *);

	private:

		QGradientStops	gradStops;
		
};

#endif // GRADIENTSLIDERFRAME_H
