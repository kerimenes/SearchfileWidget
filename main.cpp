#include "serialno.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SerialNo w;
	w.show();

	return a.exec();
}
