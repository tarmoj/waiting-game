#include "waitingwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WaitingWindow w;
	w.show();

	return a.exec();
}
