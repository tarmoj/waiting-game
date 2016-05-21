#ifndef WAITINGWINDOW_H
#define WAITINGWINDOW_H
#include "csengine.h"
#include "wsserver.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QLabel>

namespace Ui {
class WaitingWindow;
}

class WaitingWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit WaitingWindow(QWidget *parent = 0);
	~WaitingWindow();


	void closeEvent(QCloseEvent *event);
public slots:
	void on_volumeSlider_valueChanged(int value);
	void setClientsCount(int clientsCount);
	void setCounterLabel(int labelIndex, int number);

private slots:
	void on_waterdropCheckBox_toggled(bool checked);

	void on_stonesCheckBox_toggled(bool checked);

	void on_stickCheckBox_toggled(bool checked);

	void on_windCheckBox_toggled(bool checked);

	void on_fluteCheckBox_toggled(bool checked);

	void on_checkBox_toggled(bool checked);

private:
	Ui::WaitingWindow *ui;
	CsEngine *cs;
	WsServer *wsServer;
	QList <QLabel *> labels;

};

#endif // WAITINGWINDOW_H
