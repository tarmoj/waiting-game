#include "wsserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>
#include <QDir>


QT_USE_NAMESPACE



WsServer::WsServer(quint16 port, QObject *parent) :
    QObject(parent),
	m_pWebSocketServer(new QWebSocketServer(QStringLiteral("WaitingServer"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients()
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "WsServer listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &WsServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &WsServer::closed);
    }
	onlyOneEventAllowed = false;
	emptyIPs(); // reset counters and empty arrays
}



WsServer::~WsServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void WsServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WsServer::processTextMessage);
    //connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WsServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WsServer::socketDisconnected);

    m_clients << pSocket;
	sendStates(pSocket);

    emit newConnection(m_clients.count());
}



void WsServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (!pClient) {
        return;
    }
    qDebug()<<message;
	QString senderIP = pClient->peerAddress().toString();
	QStringList messageParts = message.split(",");


	if (messageParts[0]=="play") { // comes in as "play,water|stones|sticks|wind|flute,<pan>"
		QString type = messageParts[1];
		double pan = messageParts[2].toDouble();
		QString scoreLine;
		if (type=="flute") {
			if (onlyOneEventAllowed) {
				if (!fluteIPs.contains(senderIP)) {
					fluteIPs.append(senderIP);
					pClient->sendTextMessage("set flute disable");
					emit eventCountChanged(FLUTE, ++eventCounter[FLUTE]); // increase the counter and send signal to UI
				} else {
					qDebug()<<"Second try from: " << senderIP;
					return;
				}
			}
			scoreLine.sprintf("i \"flute\" 0 15 %f ", pan);
		} else {
			if (onlyOneEventAllowed) {
				if (type=="water" && !waterIPs.contains(senderIP)) {
					waterIPs.append(senderIP);
					emit eventCountChanged(WATER, ++eventCounter[WATER]);
					pClient->sendTextMessage("set water disable");
				} else if (type=="stones" && !stoneIPs.contains(senderIP)) {
					stoneIPs.append(senderIP);
					emit eventCountChanged(STONES, ++eventCounter[STONES]);
					pClient->sendTextMessage("set stones disable");
				} else if (type=="sticks" && !stickIPs.contains(senderIP)) {
					stickIPs.append(senderIP);
					emit eventCountChanged(STICKS, ++eventCounter[STICKS]);
					pClient->sendTextMessage("set sticks disable");
				} else if (type=="wind" && !windIPs.contains(senderIP)) {
					windIPs.append(senderIP);
					emit eventCountChanged(WIND, ++eventCounter[WIND]);
					pClient->sendTextMessage("set wind disable");
				} else {
					qDebug()<<"Seems like second try from: " << senderIP;
					return;
				}
			}
			// get filename as random from the subfolder according to the sound type
			QString path = "../sounds/"+type + "/";
			QDir directory(path);
			QStringList files = directory.entryList(QStringList()<<"*.wav");
			QString filename = path + files[qrand()%files.count()];
			//qDebug()<<"File selected: "<<filename;


			scoreLine.sprintf("i \"play\" 0 5 \"%s\" %f ",filename.toLocal8Bit().data(), pan);
		}
		qDebug()<< scoreLine;
		emit newEvent(scoreLine);
	}


}

//void WsServer::processBinaryMessage(QByteArray message)
//{
//    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
//    if (pClient) {
//        pClient->sendBinaryMessage(message);
//    }
//}

void WsServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        m_clients.removeAll(pClient);
        emit newConnection(m_clients.count());
        pClient->deleteLater();
	}
}


void WsServer::sendMessage(QWebSocket *socket, QString message )
{
    if (socket == 0)
    {
        return;
    }
    socket->sendTextMessage(message);

}

void WsServer::send2all(QString message)
{
	foreach (QWebSocket *socket, m_clients) {
		socket->sendTextMessage(message);
	}
}

void WsServer::sendStates(QWebSocket *socket) // if a socket connects, send it, what sounds are enabled, what not.
{
	socket->sendTextMessage( QString("set water ") + (waterActive ? "enable" : "disable") );
	socket->sendTextMessage( QString("set stones ") + (stonesActive ? "enable" : "disable") );
	socket->sendTextMessage( QString("set sticks ") + (sticksActive ? "enable" : "disable") );
	socket->sendTextMessage( QString("set wind ") + (windActive ? "enable" : "disable") );
	socket->sendTextMessage( QString("set flute ") + (fluteActive ? "enable" : "disable") );
	//TODO: check if one event already played:
	if (onlyOneEventAllowed ) {
		QString ip = socket->peerAddress().toString();
		if (fluteIPs.contains(ip)) {
			socket->sendTextMessage("set flute disable");
		}
		if (waterIPs.contains(ip)) {
			socket->sendTextMessage("set water disable");
		}
		if (stoneIPs.contains(ip)) {
			socket->sendTextMessage("set stones disable");
		}
		if (stickIPs.contains(ip)) {
			socket->sendTextMessage("set sticks disable");
		}
		if (windIPs.contains(ip)) {
			socket->sendTextMessage("set wind disable");
		}
	}
}

void WsServer::emptyIPs()
{
	waterIPs.clear();
	stoneIPs.clear();  stickIPs.clear();
	windIPs.clear(); fluteIPs.clear();
	for (int i=0;i<5;i++) {
		eventCounter[i]=0;
		emit eventCountChanged(i, 0);
	}
}


