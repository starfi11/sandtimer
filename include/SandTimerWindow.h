#pragma once

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QMenu>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <QPainter>
#include <QPainterPath>

class SandTimerWindow : public QWidget {
    Q_OBJECT

public:
    explicit SandTimerWindow(const QString& labelName, QWidget* parent = nullptr);
    void startCountdown(const QString& labelName, int seconds);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void contextMenuEvent(QContextMenuEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private slots:
    void updateCountdown();

private:
    QLabel* nameLabel;
    QLabel* timeLabel;
    QTimer* timer;
    int remainingSeconds;
    QPoint dragPosition;
    QMenu* contextMenu;
    QMediaPlayer* player;
    QAudioOutput* audioOutput; 
    
};
