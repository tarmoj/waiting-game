#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H
#include "csengine.h"
#include "wsserver.h"

#include <QMainWindow>

namespace Ui {
class WaitingWindow;
}

class WaitingWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit WaitingWindow(QWidget *parent = 0);
	~WaitingWindow();

public slots:
	void on_volumeSlider_valueChanged(int value);
	void setClientsCount(int clientsCount);

private:
	Ui::WaitingWindow *ui;
	CsEngine *cs;
	WsServer *wsServer;

};

#endif // WAITINGWINDOW_H