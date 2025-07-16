#include "SocketServer.h"
#include <QDebug>
#include "SandTimerWindow.h"  // 你的窗口类

SocketServer::SocketServer(QObject *parent)
    : QObject(parent), server(new QLocalServer(this)) {
    
    manager = new SandTimerManager(this);
    // 如果已有残留 socket 文件，先移除，确保监听不会失败
    QLocalServer::removeServer("sandtimer_socket");

    if (!server->listen("sandtimer_socket")) {
        qDebug() << "Failed to start server.";
        return;
    }
    // 当有客户端连接时，QLocalServer 会发出 newConnection() 信号，信号会触发自定义的槽函数 onNewConnection()
    connect(server, &QLocalServer::newConnection, this, &SocketServer::onNewConnection);
}

SocketServer::~SocketServer() {}

void SocketServer::onClientDataReceived(QLocalSocket* clientConnection){
    QByteArray data = clientConnection->readAll();
    QString jsonStr = QString::fromUtf8(data).trimmed();
    qDebug() << "Received JSON:" << jsonStr;

    // JSON 解析
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        clientConnection->disconnectFromServer();
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

    clientConnection->disconnectFromServer();
}
void SocketServer::onNewConnection() {
    QLocalSocket *clientConnection = server->nextPendingConnection();
    // 当这个 socket 接收到数据时，会发出 readyRead() 信号，最后一个参数是用于处理数据的lambda表达式
    connect(clientConnection, &QLocalSocket::readyRead, this, [this, clientConnection]() {
        // lambda 表达式
        onClientDataReceived(clientConnection); 
    });
}
