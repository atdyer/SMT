#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	QObject::connect(&w, SIGNAL(quit()), qApp, SLOT(quit()));

	w.show();
	
	return a.exec();
}
