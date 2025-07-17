#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "SocketServer.h"
#include <QDebug>
#include <QJsonDocument> 
#include <QJsonObject>
#include <QJsonParseError>
#include "SandTimerWindow.h"
#include "SandTimerManager.h"

class SocketServer : public QObject {
    Q_OBJECT

public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();

private:
    QTcpServer *server;
    SandTimerManager *manager;

    void onNewConnection();
    void onClientDataReceived(QTcpSocket *clientConnection);
};
