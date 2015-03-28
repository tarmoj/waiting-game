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

    emit newConnection(m_clients.count());
}



void WsServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (!pClient) {
        return;
    }
    qDebug()<<message;

	QStringList messageParts = message.split(",");

//	if (messageParts[0]=="pause")
//		setPause();

//	if (messageParts[0]=="continue")
//		setContinue();

	if (messageParts[0]=="play") { // comes in as "play,water|stones|sticks|wind,<pan>"
		QString type = messageParts[1];
		double pan = messageParts[2].toDouble();

		// get filename as random from the subfolder according to the sound type
		QString path = "../sounds/"+type + "/";
		QDir directory(path);
		QStringList files = directory.entryList(QStringList()<<"*.wav");
		QString filename = path + files[qrand()%files.count()];
		//qDebug()<<"File selected: "<<filename;

		QString scoreLine;
		scoreLine.sprintf("i \"play\" 0 5 \"%s\" %f ",filename.toLocal8Bit().data(), pan);
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

//void WsServer::getFilenames()
//{	QStringList voices = QStringList()<< "low" << "medium" << "high";
//	QStringList lengths = QStringList()<< "short" << "medium" << "long";
//	for (int voice=LOW;voice<=HIGH;voice++) {

//		for (int length=SHORT; length<=LONG; length++) {
//			QString path = "../sounds/"+voices[voice]+"/"+lengths[length];
//			QDir directory(path);
//			QStringList files = directory.entryList(QStringList()<<"*.wav");

//			foreach (QString fileName, files) {
//					soundFiles[voice][length].append(path+"/"+fileName);
//			}
//			//qDebug()<<"directory: "<<path<<"files: "<< soundFiles[voice][length];
//		}
//	}
//}


void WsServer::sendMessage(QWebSocket *socket, QString message )
{
    if (socket == 0)
    {
        return;
    }
    socket->sendTextMessage(message);

}

