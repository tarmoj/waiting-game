#include "waitingwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WaitingWindow w;
	w.show();
	w.setWindowIcon(QIcon::fromTheme("applications-other")); // otherwis close icon, Why?

	return a.exec();
}
