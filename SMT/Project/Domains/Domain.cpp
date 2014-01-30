#include "Domain.h"

Domain::Domain(ProjectFile *projectFile, QObject *parent) :
	QObject(parent),
	camera(0),
	fort14(0),
	progressBar(0),
	projectFile(projectFile),
	selectionLayer(0),
	currentMode(DisplayAction),
	oldx(0.0),
	oldy(0.0),
	newx(0.0),
	newy(0.0),
	dx(0.0),
	dy(0.0),
	pushedButton(Qt::NoButton),
	clicking(false),
	mouseMoved(false)
{
	camera = new GLCamera();
}


Domain::~Domain()
{
	delete camera;
}


void Domain::Draw()
{
	if (fort14)
		fort14->Draw();

	if (selectionLayer)
		selectionLayer->Draw();
}


Fort14* Domain::GetFort14()
{
	return fort14;
}


void Domain::MouseClick(QMouseEvent *event)
{
	clicking = true;
	pushedButton = event->button();

	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;

	if (currentMode == SelectionAction && selectionLayer)
		selectionLayer->MouseClick(event);

	emit updateGL();
}


void Domain::MouseMove(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	CalculateMouseCoordinates();

	if (currentMode == DisplayAction)
	{
		if (clicking)
		{
			Pan(dx, dy);
			emit updateGL();
		}
	}
	else if (currentMode == SelectionAction && selectionLayer)
	{
		selectionLayer->MouseMove(event);
		emit updateGL();
	}

	oldx = newx;
	oldy = newy;
}


void Domain::MouseRelease(QMouseEvent *event)
{
	clicking = false;
	mouseMoved = false;

	oldx = event->x();
	oldy = event->y();

	if (currentMode == SelectionAction && selectionLayer)
	{
		selectionLayer->MouseRelease(event);
	}

	emit updateGL();
}


void Domain::MouseWheel(QWheelEvent *event)
{
	if (!clicking)
		Zoom(event->delta());

	CalculateMouseCoordinates();

	emit updateGL();
}


void Domain::KeyPress(QKeyEvent *event)
{
//	if (event->key() == Qt::Key_O)
//		ToggleTerrainQuadtree();
//	else if (event->key() == Qt::Key_E)
//		UseTool(ClickToolType, ElementSelection);

	if (currentMode == SelectionAction && selectionLayer)
		selectionLayer->KeyPress(event);
}


void Domain::Redo()
{
	if (selectionLayer)
		selectionLayer->Redo();
	emit updateGL();
}


bool Domain::RedoAvailable()
{
	if (selectionLayer)
		return selectionLayer->GetRedoAvailable();
	return false;
}


void Domain::SetCamera(GLCamera *newCam)
{
	camera = newCam;
	if (fort14)
		fort14->SetCamera(camera);

}


void Domain::SetProgressBar(QProgressBar* newBar)
{
	progressBar = newBar;
}


void Domain::SetWindowSize(float w, float h)
{
	if (camera)
		camera->SetWindowSize(-1.0*w/h, 1.0*w/h, -1.0, 1.0, -1000.0, 1000.0);

	if (selectionLayer)
		selectionLayer->WindowSizeChanged(w, h);
}


void Domain::Undo()
{
	if (selectionLayer)
		selectionLayer->Undo();
	emit updateGL();
}


bool Domain::UndoAvailable()
{
	if (selectionLayer)
		return selectionLayer->GetUndoAvailable();
	return false;
}


void Domain::UseTool(ToolType tool, SelectionType selection, SelectionMode mode)
{
	currentMode = SelectionAction;
	if (selectionLayer)
		selectionLayer->UseTool(tool, selection, mode);
	emit setCursor(Qt::CrossCursor);
}


QColor Domain::GetTerrainSolidOutline()
{
	if (fort14)
		return fort14->GetSolidOutlineColor();
	return QColor();
}


QColor Domain::GetTerrainSolidFill()
{
	if (fort14)
		return fort14->GetSolidFillColor();
	return QColor();
}


QGradientStops Domain::GetTerrainGradientOutline()
{
	if (fort14)
		return fort14->GetGradientOutlineColors();
	return QGradientStops();
}


QGradientStops Domain::GetTerrainGradientFill()
{
	if (fort14)
		return fort14->GetGradientFillColors();
	return QGradientStops();
}


ShaderType Domain::GetTerrainOutlineType()
{
	if (fort14)
		return fort14->GetOutlineShaderType();
	return NoShaderType;
}


ShaderType Domain::GetTerrainFillType()
{
	if (fort14)
		return fort14->GetFillShaderType();
	return NoShaderType;
}


float Domain::GetTerrainMinZ()
{
	if (fort14)
		return fort14->GetMinZ();
	return 0.0;
}


float Domain::GetTerrainMaxZ()
{
	if (fort14)
		return fort14->GetMaxZ();
	return 0.0;
}


void Domain::Zoom(float zoomAmount)
{
	if (camera)
		camera->Zoom(zoomAmount);
	emit updateGL();
}


void Domain::Pan(float dx, float dy)
{
	if (camera)
		camera->Pan(dx, dy);
	emit updateGL();
}


void Domain::CalculateMouseCoordinates()
{
	if (camera)
	{
		float glX, glY, domX, domY;
		camera->GetUnprojectedPoint(newx, newy, &glX, &glY);
		if (fort14)
		{
			domX = fort14->GetUnprojectedX(glX);
			domY = fort14->GetUnprojectedY(glY);
			emit mouseX(domX);
			emit mouseY(domY);
		} else {
			emit mouseX(glX);
			emit mouseY(glY);
		}
	} else {
		emit mouseX(newx);
		emit mouseY(newy);
	}
}


void Domain::SetTerrainSolidOutline(QColor newColor)
{
	if (fort14)
		fort14->SetSolidOutlineColor(newColor);

	emit updateGL();
}


void Domain::SetTerrainSolidFill(QColor newColor)
{
	if (fort14)
		fort14->SetSolidFillColor(newColor);

	emit updateGL();
}


void Domain::SetTerrainGradientOutline(QGradientStops newStops)
{
	if (fort14)
		fort14->SetGradientOutlineColors(newStops);

	emit updateGL();
}


void Domain::SetTerrainGradientFill(QGradientStops newStops)
{
	if (fort14)
		fort14->SetGradientFillColors(newStops);

	emit updateGL();
}


void Domain::EnterDisplayMode()
{
	currentMode = DisplayAction;
	emit setCursor(Qt::ArrowCursor);
}

