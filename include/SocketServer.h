#pragma once
#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include "SandTimerManager.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>


class SocketServer : public QObject {
    Q_OBJECT

public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();

private slots:
    void onNewConnection();
    void onClientDataReceived(QLocalSocket* clientConnection);

private:
    QLocalServer *server;
    SandTimerManager* manager;  // 管理计时器的类实例
};
