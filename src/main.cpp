#include <QApplication>
#include "SandTimerWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SandTimerWindow window;
    window.startCountdown(10); // 150秒倒计时
    return app.exec();
}
