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
	wsServer->waterActive = ui->waterdropCheckBox->isChecked();
	wsServer->stonesActive = ui->stonesCheckBox->isChecked();
	wsServer->sticksActive = ui->stonesCheckBox->isChecked();
	wsServer->windActive = ui->windCheckBox->isChecked();
	wsServer->fluteActive = ui->fluteCheckBox->isChecked();

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
	wsServer->waterActive = checked;
	wsServer->send2all("set water "+ state); // syntax: disabled <html-id> <true|false>
}

void WaitingWindow::on_stonesCheckBox_toggled(bool checked)
{
	wsServer->stonesActive = checked;
	QString state = (checked) ? "enable" : "disable";
	wsServer->send2all("set stones "+ state);

}

void WaitingWindow::on_stickCheckBox_toggled(bool checked)
{
	wsServer->sticksActive = checked;
	QString state = (checked) ? "enable" : "disable";
	wsServer->send2all("set sticks "+ state);
}

void WaitingWindow::on_windCheckBox_toggled(bool checked)
{
	wsServer->windActive = checked;
	QString state = (checked) ? "enable" : "disable";
	wsServer->send2all("set wind "+ state);
}

void WaitingWindow::on_fluteCheckBox_toggled(bool checked)
{
	wsServer->fluteActive = checked;
	QString state = (checked) ? "enable" : "disable";
	wsServer->send2all("set flute "+ state);
}
