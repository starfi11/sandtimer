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
#include <QPropertyAnimation>

class SandTimerWindow : public QWidget {
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)

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

    // ✅ 动态背景动画相关
    QColor backgroundColor;
    QPropertyAnimation* bgAnimation = nullptr;

    QColor getBackgroundColor() const;
    void setBackgroundColor(const QColor& color);
    
};
