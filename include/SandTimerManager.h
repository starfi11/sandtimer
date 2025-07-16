#pragma once

#include <QObject>
#include <QString>
#include <QMap>
#include "SandTimerWindow.h"

// 管理单个计时器的信息
struct TimerInfo {
    SandTimerWindow* window;
    int initialSeconds;
};

class SandTimerManager : public QObject {
    Q_OBJECT

public:
    explicit SandTimerManager(QObject* parent = nullptr);

    // 启动新的计时器（已有则重置）
    void startTimer(const QString& label, int seconds);

    // 重置已有计时器
    void resetTimer(const QString& label);

    // 取消并关闭已有计时器
    void cancelTimer(const QString& label);

private slots:
    void handleTimerClosed();  // 计时器窗口被关闭后自动清理记录

private:
    QMap<QString, TimerInfo> timerMap;
};
