#include "SliderItemDelegate.h"

SliderItemDelegate::SliderItemDelegate(QObject *parent) :
	QStyledItemDelegate(parent)
{
	minValue = 0.0;
	maxValue = 1.0;
}


void SliderItemDelegate::SetValueRange(float min, float max)
{
	if (min <= max)
	{
		minValue = min;
		maxValue = max;
	} else {
		minValue = max;
		maxValue = min;
	}
}


void SliderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	painter->save();

	int width = option.rect.width();
	int height = option.rect.height();

	QString text = index.data(Qt::DisplayRole).toString();
	QColor color = index.data(Qt::BackgroundRole).value<QColor>();
	QRect colorRect (option.rect.x()+5, option.rect.y()+5, 50, height-10);

	if (option.state & QStyle::State_Selected)
	{
//		QColor selectedColor (color);
//		selectedColor.setAlpha(40);
//		QBrush highlightBrush (selectedColor);
//		painter->fillRect(option.rect, highlightBrush);
		painter->drawRect(option.rect.adjusted(0, 1, -1, -1));
	}

	painter->fillRect(colorRect, color);
	painter->drawRect(colorRect);
	painter->drawText(QRect(option.rect.x()+60, option.rect.y()+5, width-60, height-10), Qt::AlignVCenter, text);

	painter->restore();
}


QSize SliderItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	return QSize(100, 40);
}


QWidget *SliderItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *editor = new QLineEdit(parent);
	return editor;
}


void SliderItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QString currValue = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setText(currValue);
}


void SliderItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	lineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	lineEdit->setGeometry(QRect(option.rect.x()+60, option.rect.y()+5, option.rect.width()-60, option.rect.height()-10));
}


void SliderItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
	QString valueString = lineEdit->text();

	bool validInput = false;
	float newValue = valueString.toFloat(&validInput);

	if (validInput)
	{
		if (newValue <= maxValue && newValue >= minValue)
		{
			model->setData(index, valueString, Qt::DisplayRole);
			model->setData(index, valueString, Qt::EditRole);
			emit valueChanged(index.row(), newValue);
		}
	}

}
