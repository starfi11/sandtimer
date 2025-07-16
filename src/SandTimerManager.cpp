#include "SandTimerManager.h"

SandTimerManager::SandTimerManager(QObject* parent)
    : QObject(parent) {}

void SandTimerManager::startTimer(const QString& label, int seconds) {
    // 如果存在旧窗口，先关闭
    if (timerMap.contains(label)) {
        timerMap[label].window->close();
        timerMap.remove(label);
    }

    // 创建新窗口
    SandTimerWindow* window = new SandTimerWindow();
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->startCountdown(seconds);
    window->show();

    connect(window, &QObject::destroyed, this, &SandTimerManager::handleTimerClosed);

    // 添加到管理映射中
    timerMap[label] = TimerInfo{ window, seconds };
}

void SandTimerManager::resetTimer(const QString& label) {
    if (!timerMap.contains(label))
        return;

    int seconds = timerMap[label].initialSeconds;
    SandTimerWindow* window = timerMap[label].window;
    window->startCountdown(seconds);
}

void SandTimerManager::cancelTimer(const QString& label) {
    if (!timerMap.contains(label))
        return;

    timerMap[label].window->close();  // 触发 destroyed 信号
    timerMap.remove(label);
}

void SandTimerManager::handleTimerClosed() {
    SandTimerWindow* closedWindow = qobject_cast<SandTimerWindow*>(sender());
    if (!closedWindow)
        return;

    // 找出并移除对应 label
    QString foundLabel;
    for (auto it = timerMap.begin(); it != timerMap.end(); ++it) {
        if (it->window == closedWindow) {
            foundLabel = it.key();
            break;
        }
    }

    if (!foundLabel.isEmpty()) {
        timerMap.remove(foundLabel);
    }
}
