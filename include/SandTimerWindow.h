#pragma once

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include <QMediaPlayer>
#include <QAudioOutput>


class SandTimerWindow : public QWidget {
    Q_OBJECT

public:
    explicit SandTimerWindow(QWidget* parent = nullptr);
    void startCountdown(int seconds);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void updateCountdown();

private:
    QLabel* timeLabel;
    QTimer* timer;
    int remainingSeconds;
    QPoint dragPosition;
    QMenu* contextMenu;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    
};
