#ifndef TRIANGLESLIDERBUTTON_H
#define TRIANGLESLIDERBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QMenu>
#include <QContextMenuEvent>
#include <QColorDialog>

class TriangleSliderButton : public QPushButton
{
		Q_OBJECT
	public:

		static int	sliderWidth;
		static int	sliderHeight;

		explicit TriangleSliderButton(QWidget *parent = 0);

		unsigned int	GetID();
		void		SetRemovable(bool newRemovable);
		void		SetColor(QColor newColor);
		void		SetValue(float newValue);
		QColor		GetColor();
		float		GetValue();

	protected:

		void	paintEvent(QPaintEvent *event);
		void	resizeEvent(QResizeEvent *event);
		void	mousePressEvent(QMouseEvent *);
		void	mouseReleaseEvent(QMouseEvent *);
		void	contextMenuEvent(QContextMenuEvent *event);

	private:

		unsigned int		sliderID;
		static unsigned int	nextID;

		float		currentValue;

		QColor		triangleColor;
		QPolygon	triangle;
		int		triangleWidth;
		int		triangleHeight;
		bool		isRemovable;

		QPoint		lastMousePoint;

		void		BuildTriangle();
		void		SelectNewColor();

	signals:

		void	sliderPressed(unsigned int);
		void	sliderReleased(unsigned int);
		void	removeSlider(unsigned int);
		void	colorChanged(unsigned int, QColor);

	public slots:

		void	requestColorChange();
		void	requestRemoval();
};

#endif // TRIANGLESLIDERBUTTON_H
