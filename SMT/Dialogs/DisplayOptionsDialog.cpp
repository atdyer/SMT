#include "DisplayOptionsDialog.h"
#include "ui_DisplayOptionsDialog.h"

DisplayOptionsDialog::DisplayOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DisplayOptionsDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	currentDomain = 0;

	ui->outlineShaderOptions->setCurrentIndex(ui->outlineShaderType->currentIndex());
	ui->fillShaderOptions->setCurrentIndex(ui->fillShaderType->currentIndex());

	connect(ui->outlineShaderType, SIGNAL(currentIndexChanged(int)), ui->outlineShaderOptions, SLOT(setCurrentIndex(int)));
	connect(ui->outlineShaderOptions, SIGNAL(solidColorChanged(QColor)), this, SLOT(solidColorChanged(QColor)));
	connect(ui->outlineShaderOptions, SIGNAL(gradientChanged(QGradientStops)), this, SLOT(gradientChanged(QGradientStops)));
	connect(ui->fillShaderType, SIGNAL(currentIndexChanged(int)), ui->fillShaderOptions, SLOT(setCurrentIndex(int)));
	connect(ui->fillShaderOptions, SIGNAL(solidColorChanged(QColor)), this, SLOT(solidColorChanged(QColor)));
	connect(ui->fillShaderOptions, SIGNAL(gradientChanged(QGradientStops)), this, SLOT(gradientChanged(QGradientStops)));

}

DisplayOptionsDialog::~DisplayOptionsDialog()
{
	delete ui;
}


void DisplayOptionsDialog::SetActiveDomain(Domain *newDomain)
{
	DisconnectCurrentDomain();
	currentDomain = newDomain;
	DisplayCurrentDomainProperties();
	ConnectCurrentDomain();
}


void DisplayOptionsDialog::DisconnectCurrentDomain()
{
	if (currentDomain)
	{
		disconnect(this, SIGNAL(solidOutlineChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidOutline(QColor)));
		disconnect(this, SIGNAL(solidFillChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidFill(QColor)));
		disconnect(this, SIGNAL(gradientOutlineChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientOutline(QGradientStops)));
		disconnect(this, SIGNAL(gradientFillChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientFill(QGradientStops)));
	}
}


void DisplayOptionsDialog::DisplayCurrentDomainProperties()
{
	if (currentDomain)
	{
		Fort14 *fort14 = currentDomain->GetFort14();
		if (fort14)
		{
			ui->outlineShaderOptions->SetSolidColor(fort14->GetSolidOutlineColor());
			ui->outlineShaderOptions->SetGradientRange(fort14->GetMinZ(), fort14->GetMaxZ());
			ui->outlineShaderOptions->SetGradient(fort14->GetGradientOutlineColors());
			ui->outlineShaderOptions->update();

			ui->fillShaderOptions->SetSolidColor(fort14->GetSolidFillColor());
            //ui->fillShaderOptions->SetGradientRange(fort14->GetMinZ(), fort14->GetMaxZ());
            ui->fillShaderOptions->SetGradientRange(fort14->GetMinDisplayVal(), fort14->GetMaxDisplayVal());    //aa15
			ui->fillShaderOptions->SetGradient(fort14->GetGradientFillColors());
			ui->fillShaderOptions->update();

			ShaderType currentFill = fort14->GetFillShaderType();
			ShaderType currentOutline = fort14->GetOutlineShaderType();

			if (currentFill == SolidShaderType)
			{
				showSolidFillWindow();
			}
			else if (currentFill == GradientShaderType)
			{
				showGradientFillWindow();
				if (currentDomain)
					fort14->SetGradientFillColors(fort14->GetGradientFillColors());
			}

			if (currentOutline == SolidShaderType)
			{
				showSolidOutlineWindow();
			}
			else if (currentOutline == GradientShaderType)
			{
				showGradientOutlineWindow();
			}
		}
	}
}


void DisplayOptionsDialog::ConnectCurrentDomain()
{
	if (currentDomain)
	{
		connect(this, SIGNAL(solidOutlineChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidOutline(QColor)));
		connect(this, SIGNAL(solidFillChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidFill(QColor)));
		connect(this, SIGNAL(gradientOutlineChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientOutline(QGradientStops)));
		connect(this, SIGNAL(gradientFillChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientFill(QGradientStops)));
	}
}


void DisplayOptionsDialog::showSolidOutlineWindow()
{
	ui->outlineShaderType->setCurrentIndex(0);
}


void DisplayOptionsDialog::showSolidFillWindow()
{
	ui->fillShaderType->setCurrentIndex(0);
}


void DisplayOptionsDialog::showGradientOutlineWindow()
{
	ui->outlineShaderType->setCurrentIndex(1);
}


void DisplayOptionsDialog::showGradientFillWindow()
{
	ui->fillShaderType->setCurrentIndex(1);
}


void DisplayOptionsDialog::show()
{
	QDialog::show();
	emit updateChildrenGeometry();
}


void DisplayOptionsDialog::solidColorChanged(QColor color)
{
	int currentTab = ui->tabWidget->currentIndex();
	if (currentTab == 1)
	{
		emit solidOutlineChanged(color);
	}
	else if (currentTab == 0)
	{
		emit solidFillChanged(color);
	}
}


void DisplayOptionsDialog::gradientChanged(QGradientStops gradientStops)
{
	int currentTab = ui->tabWidget->currentIndex();
	if (currentTab == 1)
	{
		emit gradientOutlineChanged(gradientStops);
	}
	else if (currentTab == 0)
	{
		emit gradientFillChanged(gradientStops);
	}
}
