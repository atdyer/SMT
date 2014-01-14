#include "ShaderOptionsStackedWidget.h"
#include "ui_ShaderOptionsStackedWidget.h"

ShaderOptionsStackedWidget::ShaderOptionsStackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::ShaderOptionsStackedWidget)
{
	ui->setupUi(this);

	delegate = 0;

	ui->colorFrame->setAutoFillBackground(true);

	connect(ui->colorPicker, SIGNAL(colorPicked(QColor)), ui->valueSlider, SLOT(setColor(QColor)));
	connect(ui->valueSlider, SIGNAL(colorPicked(QColor)), this, SLOT(colorChanged(QColor)));
	connect(ui->valueSlider, SIGNAL(valuePicked(int)), ui->colorPicker, SLOT(valueSliderChanged(int)));
	connect(ui->hueSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setHue(int)));
	connect(ui->satSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setSaturation(int)));
	connect(ui->valSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setValue(int)));

	connect(ui->basicColorsWidget, SIGNAL(colorClicked(QColor)), ui->colorPicker, SLOT(setColor(QColor)));

	connect(ui->addToCustomColorsButton, SIGNAL(clicked()), this, SLOT(addCurrentColorToCustomColors()));

	connect(this, SIGNAL(currentChanged(int)), this, SLOT(shaderTypeSelected(int)));

	/* Set up gradient picker */
	SetupGradientPicker();
	SetupSliderListContextMenu();
}

ShaderOptionsStackedWidget::~ShaderOptionsStackedWidget()
{
	delete ui;
}


void ShaderOptionsStackedWidget::SetSolidColor(QColor newColor)
{
	ui->hueSpin->setValue(newColor.hue());
	ui->satSpin->setValue(newColor.saturation());
	ui->valSpin->setValue(newColor.value());
	ui->alphaSpin->setValue(newColor.alpha());
	update();
}


void ShaderOptionsStackedWidget::SetGradient(QGradientStops newGradient)
{
	ui->gradientSliderWidget->SetGradientStops(newGradient);
}


void ShaderOptionsStackedWidget::SetGradientRange(float lowVal, float highVal)
{
	ui->gradientSliderWidget->SetMinValue(lowVal);
	ui->gradientSliderWidget->SetMaxValue(highVal);
	if (delegate)
		delegate->SetValueRange(lowVal, highVal);
}


void ShaderOptionsStackedWidget::SetupGradientPicker()
{
	delegate = new SliderItemDelegate();
	delegate->SetValueRange(ui->gradientSliderWidget->GetMinValue(), ui->gradientSliderWidget->GetMaxValue());
	connect(delegate, SIGNAL(valueChanged(int,float)), this, SLOT(sliderValueSet(int,float)));

	ui->sliderList->setItemDelegate(delegate);


	connect(ui->gradientSliderWidget, SIGNAL(currentSliderChanged(uint,float,QColor)), this, SLOT(currentSliderChanged(uint,float,QColor)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderColorChanged(uint,QColor)), this, SLOT(gradientSliderColorChanged(uint,QColor)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderValueChanged(uint,float)), this, SLOT(gradientSliderValueChanged(uint,float)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderAdded(uint,float,QColor)), this, SLOT(sliderAdded(uint,float,QColor)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderRemoved(uint)), this, SLOT(removeSlider(uint)));
	connect(ui->gradientSliderWidget, SIGNAL(removeSliderAvailable(bool)), ui->removeSliderButton, SLOT(setEnabled(bool)));
	connect(ui->gradientSliderWidget, SIGNAL(gradientStopsUpdated(QGradientStops)), this, SIGNAL(gradientChanged(QGradientStops)));
	connect(ui->addSliderButton, SIGNAL(clicked()), this, SLOT(addSlider()));
	connect(ui->removeSliderButton, SIGNAL(clicked()), this, SLOT(removeSlider()));

	/* Add default sliders */
	unsigned int initialBottomSlider = ui->gradientSliderWidget->AddSlider();
	unsigned int initialTopSlider = ui->gradientSliderWidget->AddSlider();

	if (initialBottomSlider)
	{
		ui->gradientSliderWidget->SetSliderColor(initialBottomSlider, QColor::fromRgb(0, 255, 0));
		ui->gradientSliderWidget->SetSliderValue(initialBottomSlider, ui->gradientSliderWidget->GetMinValue());
	}

	if (initialTopSlider)
	{
		ui->gradientSliderWidget->SetSliderColor(initialTopSlider, QColor::fromRgb(0, 0, 255));
		ui->gradientSliderWidget->SetSliderValue(initialTopSlider, ui->gradientSliderWidget->GetMaxValue());
	}
}


void ShaderOptionsStackedWidget::SetupSliderListContextMenu()
{
	QAction *valueAction = new QAction("Edit Value", 0);
	QAction *colorAction = new QAction("Edit Color", 0);
	connect(valueAction, SIGNAL(triggered()), this, SLOT(editCurrentSliderValue()));
	connect(colorAction, SIGNAL(triggered()), this, SLOT(editCurrentSliderColor()));
	ui->sliderList->addAction(valueAction);
	ui->sliderList->addAction(colorAction);
	ui->sliderList->setContextMenuPolicy(Qt::ActionsContextMenu);
}


void ShaderOptionsStackedWidget::shaderTypeSelected(int type)
{
	if (type == 0)
	{
		emit solidColorChanged(QColor(ui->colorFrame->palette().color(backgroundRole())));
	}
	else if (type == 1)
	{
		emit gradientChanged(ui->gradientSliderWidget->GetGradient());
	}
}


void ShaderOptionsStackedWidget::colorChanged(const QColor &c)
{
	QPalette newPalette (backgroundRole(), c);
	ui->colorFrame->setPalette(newPalette);

	bool hue = ui->hueSpin->blockSignals(true);
	bool sat = ui->satSpin->blockSignals(true);
	bool val = ui->valSpin->blockSignals(true);

	ui->hueSpin->setValue(c.hue());
	ui->satSpin->setValue(c.saturation());
	ui->valSpin->setValue(c.value());

	ui->hueSpin->blockSignals(hue);
	ui->satSpin->blockSignals(sat);
	ui->valSpin->blockSignals(val);

	QColor finalColor(c);
	finalColor.setAlpha(ui->alphaSpin->text().toFloat());

	emit solidColorChanged(finalColor);
}


void ShaderOptionsStackedWidget::addCurrentColorToCustomColors()
{
	ui->customColorsWidget->addColor(ui->colorFrame->palette().color(backgroundRole()));
}


void ShaderOptionsStackedWidget::addSlider()
{
	unsigned int sliderID = ui->gradientSliderWidget->AddSlider();
	if (sliderID)
	{
		float initialValue = (ui->gradientSliderWidget->GetMinValue() + ui->gradientSliderWidget->GetMaxValue()) / 2.0;
		QColor initialColor = QColor::fromRgb(255, 255, 255);
		ui->gradientSliderWidget->SetSliderValue(sliderID, initialValue);
		ui->gradientSliderWidget->SetSliderColor(sliderID, initialColor);
	}
}


void ShaderOptionsStackedWidget::removeSlider()
{
	QListWidgetItem *currentSlider = ui->sliderList->currentItem();
	unsigned int sliderID = sliderListItems.key(currentSlider, 0);
	if (sliderID)
	{
		ui->gradientSliderWidget->RemoveSlider(sliderID);
	}
}


void ShaderOptionsStackedWidget::removeSlider(unsigned int sliderID)
{
	if (sliderListItems.contains(sliderID))
	{
		QListWidgetItem *currentSlider = sliderListItems.take(sliderID);
		delete currentSlider;
	}
}


void ShaderOptionsStackedWidget::sliderAdded(unsigned int sliderID, float sliderValue, QColor sliderColor)
{
	if (!sliderListItems.contains(sliderID))
	{
		QListWidgetItem *newItem = new QListWidgetItem(ui->sliderList, 0);

		sliderListItems[sliderID] = newItem;

		newItem->setData(Qt::DisplayRole, sliderValue);
		newItem->setData(Qt::EditRole, sliderValue);
		newItem->setData(Qt::BackgroundRole, sliderColor);
		newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
	}
}


void ShaderOptionsStackedWidget::sliderValueSet(int row, float newValue)
{
	unsigned int sliderID = sliderListItems.key(ui->sliderList->item(row), 0);
	ui->gradientSliderWidget->SetSliderValue(sliderID, newValue);
}


void ShaderOptionsStackedWidget::currentSliderChanged(unsigned int sliderID, float sliderValue, QColor sliderColor)
{
	if (sliderListItems.contains(sliderID))
	{
		ui->sliderList->setCurrentItem(sliderListItems[sliderID]);
	}
}


void ShaderOptionsStackedWidget::gradientSliderColorChanged(unsigned int sliderID, QColor newColor)
{
	if (sliderListItems.contains(sliderID))
	{
		QListWidgetItem *listItem = sliderListItems[sliderID];
		listItem->setData(Qt::BackgroundRole, newColor);
	}
}


void ShaderOptionsStackedWidget::gradientSliderValueChanged(unsigned int sliderID, float newValue)
{
	if (sliderListItems.contains(sliderID))
	{
		QListWidgetItem *listItem = sliderListItems[sliderID];
		listItem->setData(Qt::DisplayRole, newValue);
		ui->sliderList->sortItems(Qt::DescendingOrder);
	}
}


void ShaderOptionsStackedWidget::editCurrentSliderValue()
{
	QListWidgetItem *currentSlider = ui->sliderList->currentItem();
	ui->sliderList->editItem(currentSlider);
}


void ShaderOptionsStackedWidget::editCurrentSliderColor()
{
	QListWidgetItem *currentSlider = ui->sliderList->currentItem();
	unsigned int sliderID = sliderListItems.key(currentSlider, 0);
	if (sliderID)
	{
		ui->gradientSliderWidget->requestNewColor(sliderID);
	}
}
