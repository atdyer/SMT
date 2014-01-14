#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>

class ColorButton : public QToolButton
{
		Q_OBJECT
	public:
		explicit ColorButton(QWidget *parent = 0);
		~ColorButton();

		void	SetBackgroundColor(QColor color);
		void	SetRow(int newRow);
		void	SetColumn(int newCol);
		int	GetRow();
		int	GetColumn();

	private:

		QColor	currentColor;
		int	row;
		int	column;

	public slots:

		void	click();

	signals:

		void	clicked(QColor);
		void	clicked(QColor, int, int);
};

#endif // COLORBUTTON_H
