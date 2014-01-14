#ifndef SHADEROPTIONSSTACKEDWIDGET_H
#define SHADEROPTIONSSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QColorDialog>
#include <QMap>
#include <QListWidget>
#include "Widgets/ColorWidgets/ColorGradientFrame.h"
#include "Widgets/ColorWidgets/ValueSlider.h"
#include "Widgets/ColorWidgets/SliderItemDelegate.h"

namespace Ui {
	class ShaderOptionsStackedWidget;
}

class ShaderOptionsStackedWidget : public QStackedWidget
{
		Q_OBJECT
		
	public:
		explicit ShaderOptionsStackedWidget(QWidget *parent = 0);
		~ShaderOptionsStackedWidget();

		void	SetSolidColor(QColor newColor);
		void	SetGradient(QGradientStops newGradient);
		void	SetGradientRange(float lowVal, float highVal);
		
	private:

		Ui::ShaderOptionsStackedWidget *ui;

		QMap<unsigned int, QListWidgetItem*>	sliderListItems;

		SliderItemDelegate *delegate;

		void	SetupGradientPicker();
		void	SetupSliderListContextMenu();


	protected slots:

		void	shaderTypeSelected(int type);

		void	colorChanged(const QColor &c);
		void	addCurrentColorToCustomColors();

		/* Slider slots */
		void	addSlider();
		void	removeSlider();
		void	removeSlider(unsigned int sliderID);
		void	sliderAdded(unsigned int sliderID, float sliderValue, QColor sliderColor);
		void	sliderValueSet(int row, float newValue);
		void	currentSliderChanged(unsigned int sliderID, float sliderValue, QColor sliderColor);
		void	gradientSliderColorChanged(unsigned int sliderID, QColor newColor);
		void	gradientSliderValueChanged(unsigned int sliderID, float newValue);
		void	editCurrentSliderValue();
		void	editCurrentSliderColor();

	signals:

		void	solidColorChanged(QColor);
		void	gradientChanged(QGradientStops);

};

#endif // SHADEROPTIONSSTACKEDWIDGET_H
