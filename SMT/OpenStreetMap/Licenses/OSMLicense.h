#ifndef OSMLICENSE_H
#define OSMLICENSE_H

#include <QObject>

#include "OpenStreetMap/Surfaces/RectangleRenderSurface.h"

class OSMLicense : public QObject
{
		Q_OBJECT
	public:
		explicit OSMLicense(int width, int height, QObject *parent = 0);
		~OSMLicense();

		void	Render();
		void	SetViewportSize(int width, int height);

	private:

		RectangleRenderSurface* surface;


};

#endif // OSMLICENSE_H
