#ifndef GRADIENTSLIDERWIDGET_H
#define GRADIENTSLIDERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QBoxLayout>

#include <iostream>

#include "Widgets/ColorWidgets/GradientSliderFrame.h"
#include "Widgets/ColorWidgets/TriangleSliderButton.h"

class GradientSliderWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit GradientSliderWidget(QWidget *parent = 0);

		unsigned int	AddSlider();
		void		RemoveSlider(unsigned int sliderID);

		void	SetMinValue(float newMin);
		void	SetMaxValue(float newMax);

		void	SetGradientStops(QGradientStops newStops);

		void	SetSliderColor(unsigned int sliderID, QColor newColor);
		void	SetSliderValue(unsigned int sliderID, float newValue);

		float	GetMinValue();
		float	GetMaxValue();

		QColor	GetSliderColor(unsigned int sliderID);
		float	GetSliderValue(unsigned int sliderID);

		QGradientStops	GetGradient();

	protected:

		void	mouseMoveEvent(QMouseEvent *event);
		void	paintEvent(QPaintEvent *event);

	private:

		QBoxLayout*	layout;

		QLabel*	minLabel;
		QLabel*	maxLabel;

		GradientSliderFrame*				gradientFrame;
		QMap<unsigned int, TriangleSliderButton*>	sliders;

		int		sliderWidth;
		int		sliderHeight;
		int		sliderTop;
		int		sliderBottom;
		int		sliderX;
		unsigned int	pressedSlider;

		float	minValue;
		float	maxValue;

		void	CreateLayout();
		void	UpdateGradientStops();
		void	CheckSliderCount();
		void	CreateDefaultSliders();

		int	MapValueToY(float val);
		float	MapYToValue(int y);
		
	signals:

		void	sliderAdded(unsigned int, float, QColor);
		void	sliderRemoved(unsigned int);
		void	currentSliderChanged(unsigned int, float, QColor);
		void	sliderColorChanged(unsigned int, QColor);
		void	sliderValueChanged(unsigned int, float);
		void	removeSliderAvailable(bool);
		void	gradientStopsUpdated(const QGradientStops&);
		
	public slots:

		void	sliderPressed(unsigned int sliderID);
		void	sliderReleased(unsigned int sliderID);
		void	removeSlider(unsigned int sliderID);
		void	requestNewColor(unsigned int sliderID);
		void	updateGradientStops(unsigned int sliderID, QColor newColor);
};

#endif // GRADIENTSLIDERWIDGET_H
