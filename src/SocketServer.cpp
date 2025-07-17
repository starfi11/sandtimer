#include "SocketServer.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include "SandTimerWindow.h"  // 你的窗口类

SocketServer::SocketServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this)) {

    manager = new SandTimerManager(this);

    quint16 port = 61420;  // 可自定义本地通信端口
    if (!server->listen(QHostAddress::LocalHost, port)) {
        qDebug() << "Failed to start TCP server on port" << port;
        return;
    }

    connect(server, &QTcpServer::newConnection, this, &SocketServer::onNewConnection);
}

SocketServer::~SocketServer() {}

void SocketServer::onClientDataReceived(QTcpSocket* clientConnection) {
    QByteArray data = clientConnection->readAll();
    QString jsonStr = QString::fromUtf8(data).trimmed();
    qDebug() << "Received JSON:" << jsonStr;

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        clientConnection->disconnectFromHost();
        return;
    }

    QJsonObject obj = doc.object();
    QString cmd = obj.value("cmd").toString();
    QString label = obj.value("label").toString();

    if (cmd == "start") {
        double seconds = obj.value("time").toDouble();
        if (!label.isEmpty() && seconds > 0) {
            manager->startTimer(label, static_cast<int>(seconds));
            qDebug() << "Started timer:" << label << seconds << "s";
        } else {
            qWarning() << "Invalid start command parameters.";
        }
    }
    else if (cmd == "cancel") {
        if (!label.isEmpty()) {
            manager->cancelTimer(label);
            qDebug() << "Canceled timer:" << label;
        }
    }
    else if (cmd == "reset") {
        if (!label.isEmpty()) {
            manager->resetTimer(label);
            qDebug() << "Reset timer:" << label;
        }
    }
    else {
        qWarning() << "Unsupported command:" << cmd;
    }

    clientConnection->disconnectFromHost();
}

void SocketServer::onNewConnection() {
    QTcpSocket *clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QTcpSocket::readyRead, this, [this, clientConnection]() {
        onClientDataReceived(clientConnection);
    });
}
