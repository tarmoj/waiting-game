#ifndef WSSERVER_H
#define WSSERVER_H

#include <QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QStringList>

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

#define WATER 0
#define STICKS 1
#define STONES 2
#define WIND 3
#define FLUTE 4

class WsServer : public QObject
{
    Q_OBJECT
public:
    explicit WsServer(quint16 port, QObject *parent = NULL);
    ~WsServer();
	bool waterActive, stonesActive, sticksActive, windActive, fluteActive, onlyOneEventAllowed;
	void sendMessage(QWebSocket *socket, QString message);
	void setPaused(bool paused);
	void send2all(QString message);
	void sendStates(QWebSocket *socket);
	void emptyIPs();

Q_SIGNALS:
    void closed();
    void newConnection(int connectionsCount);
    void newEvent(QString eventString);
	void eventCountChanged(int type, int number);


private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    //void processBinaryMessage(QByteArray message);
    void socketDisconnected();



private:
    QWebSocketServer *m_pWebSocketServer;
	QList<QWebSocket *> m_clients;
	QStringList waterIPs, stoneIPs, stickIPs, windIPs, fluteIPs;
	int eventCounter[5];
	//void getFilenames();
	//QHash<QString, int>  clientsHash;
	//bool pauseIsOn;
	//QList <QList <QStringList> >  soundFiles ; // directory structure: low/mid/high -> short/mid/long -> filenames
};



#endif // WSSERVER_H
