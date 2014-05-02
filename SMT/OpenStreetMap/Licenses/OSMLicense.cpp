#include "OSMLicense.h"

OSMLicense::OSMLicense(int width, int height, QObject *parent) :
	QObject(parent)
{
	surface = new RectangleRenderSurface(QImage(":/licenseimages/images/licenses/osmlicense.png"),
					     width,
					     height);
}


OSMLicense::~OSMLicense()
{
	if (surface)
		delete surface;
}


void OSMLicense::Render()
{
	if (surface)
		surface->Render();
}


void OSMLicense::SetViewportSize(int width, int height)
{
	if (surface)
		surface->SetViewportSize(width, height);
}
