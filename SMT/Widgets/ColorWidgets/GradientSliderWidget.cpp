#include "GradientSliderWidget.h"

GradientSliderWidget::GradientSliderWidget(QWidget *parent) :
	QWidget(parent),
	minLabel(0),
	maxLabel(0),
	gradientFrame(0)
{
	sliderWidth = TriangleSliderButton::sliderWidth;
	sliderHeight = TriangleSliderButton::sliderHeight;

	sliderTop = 0;
	sliderBottom = 0;
	sliderX = 0;
	pressedSlider = 0;

	minValue = 0.0;
	maxValue = 1.0;

	CreateLayout();
}


unsigned int GradientSliderWidget::AddSlider()
{
	TriangleSliderButton *newSlider = new TriangleSliderButton(this);
	unsigned int newID = newSlider->GetID();
	if (!sliders.contains(newID))
	{
		newSlider->SetValue((minValue + maxValue) / 2.0);
		newSlider->SetColor(QColor::fromRgb(0, 0, 0));
		connect(newSlider, SIGNAL(sliderPressed(uint)), this, SLOT(sliderPressed(uint)));
		connect(newSlider, SIGNAL(sliderReleased(uint)), this, SLOT(sliderReleased(uint)));
		connect(newSlider, SIGNAL(colorChanged(uint,QColor)), this, SLOT(updateGradientStops(uint,QColor)));
		connect(newSlider, SIGNAL(removeSlider(uint)), this, SLOT(removeSlider(uint)));
		sliders[newID] = newSlider;

		newSlider->show();
		CheckSliderCount();

		emit sliderAdded(newID, newSlider->GetValue(), newSlider->GetColor());

		return newID;
	} else {
		delete newSlider;
	}

	return 0;
}


void GradientSliderWidget::RemoveSlider(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		TriangleSliderButton *currentSlider = sliders.take(sliderID);
		delete currentSlider;
		emit sliderRemoved(sliderID);
	}

	CheckSliderCount();
	UpdateGradientStops();
}


void GradientSliderWidget::SetMinValue(float newMin)
{
	minValue = newMin;
	if (minLabel)
		minLabel->setText(QString("Minimum: ").append(QString::number(minValue, 'f', 4)));
	UpdateGradientStops();
//	update();
}


void GradientSliderWidget::SetMaxValue(float newMax)
{
	maxValue = newMax;
	if (maxLabel)
		maxLabel->setText(QString("Maximum: ").append(QString::number(maxValue, 'f', 4)));
	UpdateGradientStops();
}


void GradientSliderWidget::SetGradientStops(QGradientStops newStops)
{	
	QList<uint> keys = sliders.keys();
	for (int i=0; i<keys.size(); ++i)
	{
		RemoveSlider(keys[i]);
	}

	if (newStops.size() >= 2)
	{
		for (int i=0; i<newStops.size(); ++i)
		{
			unsigned int newSlider = AddSlider();
			SetSliderValue(newSlider, minValue + (1.0-newStops[i].first) * (maxValue-minValue));
			SetSliderColor(newSlider, newStops[i].second);
		}
	} else {
		CreateDefaultSliders();
	}

	UpdateGradientStops();
}


void GradientSliderWidget::SetSliderColor(unsigned int sliderID, QColor newColor)
{
	if (sliders.contains(sliderID))
	{
		sliders.value(sliderID)->SetColor(newColor);
		emit sliderColorChanged(sliderID, newColor);
		UpdateGradientStops();
	}
}


void GradientSliderWidget::SetSliderValue(unsigned int sliderID, float newValue)
{
	if (sliders.contains(sliderID))
	{
		sliders.value(sliderID)->SetValue(newValue);
		emit sliderValueChanged(sliderID, newValue);
		UpdateGradientStops();

	}
}


float GradientSliderWidget::GetMinValue()
{
	return minValue;
}


float GradientSliderWidget::GetMaxValue()
{
	return maxValue;
}


QColor GradientSliderWidget::GetSliderColor(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		return sliders.value(sliderID)->GetColor();
	}
	return QColor::fromRgb(0, 0, 0);
}


float GradientSliderWidget::GetSliderValue(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		return sliders.value(sliderID)->GetValue();
	}
	return 0.0;
}


QGradientStops GradientSliderWidget::GetGradient()
{
	if (gradientFrame)
	{
		return gradientFrame->GetStops();
	}
}


void GradientSliderWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (pressedSlider != 0 && sliders.contains(pressedSlider))
	{
		int yLoc = event->y();
		TriangleSliderButton *currentSlider = sliders.value(pressedSlider, 0);
		if (currentSlider)
		{
			float newValue = MapYToValue(yLoc);
			SetSliderValue(pressedSlider, newValue);
			emit sliderValueChanged(pressedSlider, newValue);
			UpdateGradientStops();
		}
	}
}


void GradientSliderWidget::paintEvent(QPaintEvent *event)
{
	if (maxLabel)
		maxLabel->move(width()/2 - maxLabel->width()/2, 0);
	if (gradientFrame && minLabel && maxLabel)
	{
		gradientFrame->resize(width()-sliderWidth, height()-maxLabel->height()-minLabel->height());
		gradientFrame->move(0, maxLabel->height());

		minLabel->move(width()/2 - minLabel->width()/2, maxLabel->height()+gradientFrame->height());
	}

	TriangleSliderButton* currentSlider = 0;
	QMap<unsigned int, TriangleSliderButton*>::const_iterator it = sliders.constBegin();
	while (it != sliders.constEnd())
	{
		currentSlider = it.value();
		if (currentSlider)
		{
			float currentValue = currentSlider->GetValue();
			currentSlider->move(gradientFrame->x() + gradientFrame->width(), MapValueToY(currentValue));
		}
		++it;
	}
}


void GradientSliderWidget::CreateLayout()
{
	layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

	gradientFrame = new GradientSliderFrame(this);

	maxLabel = new QLabel(QString("Maximum: ").append(QString::number(maxValue, 'f', 4)), this);
	minLabel = new QLabel(QString("Minimum: ").append(QString::number(minValue, 'f', 4)), this);
	maxLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	minLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	maxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	minLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	layout->addWidget(maxLabel);
	layout->addWidget(gradientFrame);
	layout->addWidget(minLabel);
}


void GradientSliderWidget::UpdateGradientStops()
{
	if (gradientFrame)
	{
		QGradientStops stops;

		foreach(TriangleSliderButton* slider, sliders)
		{
			float valuePercentage = (slider->GetValue() - minValue) / (maxValue - minValue);
			stops << QGradientStop(1.0-valuePercentage, slider->GetColor());
		}

		gradientFrame->SetStops(stops);
		emit gradientStopsUpdated(stops);
		update();
	}
}


void GradientSliderWidget::CheckSliderCount()
{
	TriangleSliderButton *currentSlider = 0;
	bool removable = sliders.size() <= 2 ? false : true;

	QMap<unsigned int, TriangleSliderButton*>::const_iterator it = sliders.constBegin();
	while (it != sliders.constEnd())
	{
		currentSlider = *it;
		if (currentSlider)
		{
			currentSlider->SetRemovable(removable);
		}
		++it;
	}

	emit removeSliderAvailable(removable);
}


void GradientSliderWidget::CreateDefaultSliders()
{
	unsigned int bottomSlider = AddSlider();
	unsigned int topSlider = AddSlider();
	SetSliderValue(bottomSlider, minValue);
	SetSliderValue(topSlider, maxValue);
	SetSliderColor(bottomSlider, QColor::fromRgb(0, 0, 255));
	SetSliderColor(topSlider, QColor::fromRgb(0, 255, 0));
}


int GradientSliderWidget::MapValueToY(float val)
{
	float valuePercentage = (val - minValue) / (maxValue - minValue);
	float remainderPercentage = 1.0 - valuePercentage;
	return (gradientFrame->y() + (int)(remainderPercentage*(gradientFrame->height()))) - sliderHeight/2;
}


float GradientSliderWidget::MapYToValue(int y)
{
	if (y < gradientFrame->y())
	{
		return maxValue;
	}
	if (y > gradientFrame->y() + gradientFrame->height())
	{
		return minValue;
	}

	float yPercentage = float (gradientFrame->y() + gradientFrame->height() - y) / float(gradientFrame->height());
	return minValue + yPercentage*(maxValue - minValue);
}


void GradientSliderWidget::sliderPressed(unsigned int sliderID)
{
	pressedSlider = sliderID;
	emit currentSliderChanged(sliderID, GetSliderValue(sliderID), GetSliderColor(sliderID));
}


void GradientSliderWidget::sliderReleased(unsigned int sliderID)
{
	if (pressedSlider == sliderID)
	{
		pressedSlider = 0;
	}
}


void GradientSliderWidget::removeSlider(unsigned int sliderID)
{
	RemoveSlider(sliderID);
}


void GradientSliderWidget::requestNewColor(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		sliders[sliderID]->requestColorChange();
	}
}


void GradientSliderWidget::updateGradientStops(unsigned int sliderID, QColor newColor)
{
	UpdateGradientStops();
	emit sliderColorChanged(sliderID, newColor);
}

