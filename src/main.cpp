#include <QApplication>
#include "SandTimerWindow.h"
#include "SocketServer.h"  // 本地 socket 服务类

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SocketServer server;  // 内部监听 Named Pipe 或 UNIX socket

    return app.exec();
}
