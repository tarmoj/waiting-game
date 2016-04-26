#include "waitingwindow.h"
#include "ui_waitingwindow.h"

WaitingWindow::WaitingWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::WaitingWindow)
{
	ui->setupUi(this);
	wsServer = new WsServer(12012);
	cs = new CsEngine("../waiting-game.csd");
	cs->start();
	cs->setChannel("volume",(MYFLT)ui->volumeSlider->value()/100); // send initial value
	connect(wsServer, SIGNAL(newConnection(int)), this, SLOT(setClientsCount(int)));
	connect(wsServer, SIGNAL(newEvent(QString)),cs,SLOT(csEvent(QString))  );
}

WaitingWindow::~WaitingWindow()
{
	delete ui;
}

void WaitingWindow::setClientsCount(int clientsCount)
{
	ui->clientsCountLabel->setText(QString::number(clientsCount));
}


void WaitingWindow::on_volumeSlider_valueChanged(int value)
{
	cs->setChannel("volume",(MYFLT) value/100.0);
}

void WaitingWindow::on_waterdropCheckBox_toggled(bool checked)
{
	QString state = (checked) ? "enable" : "disable";
	wsServer->send2all("set water "+ state); // syntax: disabled <html-id> <true|false>
}

void WaitingWindow::on_stonesCheckBox_toggled(bool checked)
{
	QString state = (checked) ? "enable" : "disable";
	wsServer->send2all("set stones "+ state);
}
