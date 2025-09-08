#include "SandTimerWindow.h"
#include <QFont>
#include <QPalette>
#include <QCoreApplication>
#include <QGuiApplication>
#include <QScreen>

QColor SandTimerWindow::getBackgroundColor() const {
    return backgroundColor;
}

void SandTimerWindow::setBackgroundColor(const QColor& color) {
    backgroundColor = color;
    update();
}

SandTimerWindow::SandTimerWindow(const QString& labelName, QWidget* parent)
    : QWidget(parent), remainingSeconds(0)
{
    backgroundColor = QColor(255, 255, 255, 230);  // 初始为白色背景
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(150, 90);

    // nameLabel: 事件名称
    nameLabel = new QLabel(this);
    QFont nameFont;
    nameFont.setPointSize(12);
    nameLabel->setFont(nameFont);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("color: gray;");

    timeLabel = new QLabel("00:00", this);
    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    timeLabel->setFont(font);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("color: black; font: bold 24px;");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(nameLabel);     // 先放事件名
    layout->addWidget(timeLabel);     // 再放倒计时
    layout->setContentsMargins(10, 8, 10, 10);
    layout->setSpacing(1);
    setLayout(layout);

    audioOutput = new QAudioOutput(this);
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);
    // 设置音量（0.0~1.0）
    audioOutput->setVolume(0.8);
    // 设置音频文件路径
    player->setSource(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/assets/alarm.wav"));


    // 计时器
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SandTimerWindow::updateCountdown);
}
// void SandTimerWindow::contextMenuEvent(QContextMenuEvent* event)
// {
//     if (contextMenu)
//         contextMenu->exec(event->globalPos());
// }
void SandTimerWindow::startCountdown(const QString& labelName, int seconds) {
    // 清理旧的动画与计时器
    if (timer && timer->isActive())
        timer->stop();

    if (bgAnimation) {
        bgAnimation->stop();
        bgAnimation->deleteLater();
        bgAnimation = nullptr;
    }

    backgroundColor = QColor(255, 255, 255, 230);
    update();

    remainingSeconds = seconds;

    // 显示时间
    // 设置 nameLabel 的显示文本为：原名 + 总时间
    int minutes = seconds / 60;
    int secs = seconds % 60;
    QString timeStr = QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(secs, 2, 10, QChar('0'));
    nameLabel->setText(labelName  + " " + timeStr);

    updateCountdown();
    timer->start(1000);

    QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();

    //  右侧中间位置
    int x = screenGeometry.right() - width() - 10;  // 靠右边，留 10px 边距
    int y = screenGeometry.height() / 2 - height() / 2;

    move(x, y);  // 设置窗口位置
    show();
}

void SandTimerWindow::updateCountdown() {
    if (remainingSeconds == 0) {
        player->play();

        // 安全地重新创建动画
        if (bgAnimation) {
            bgAnimation->stop();
            bgAnimation->deleteLater();
            bgAnimation = nullptr;
        }

        bgAnimation = new QPropertyAnimation(this, "backgroundColor");
        bgAnimation->setDuration(1500);
        bgAnimation->setLoopCount(4);
        bgAnimation->setStartValue(QColor(255, 255, 255, 230));
        bgAnimation->setKeyValueAt(0.5, QColor(144, 238, 144, 255));
        bgAnimation->setEndValue(QColor(255, 255, 255, 230));

        // 不使用 DeleteWhenStopped，结束时手动回收并置空
        connect(bgAnimation, &QPropertyAnimation::finished, this, [this]() {
            if (bgAnimation) {
                bgAnimation->deleteLater();
                bgAnimation = nullptr;
            }
        });

        bgAnimation->start();
    }

    // 常规倒计时显示逻辑
    int absSeconds = std::abs(remainingSeconds);
    int minutes = absSeconds / 60;
    int seconds = absSeconds % 60;
    QString timeStr = QString("%1:%2").arg(minutes, 2, 10, QChar('0'))
                                      .arg(seconds, 2, 10, QChar('0'));
    if (remainingSeconds < 0)
        timeStr = "-" + timeStr;

    timeLabel->setText(timeStr);
    remainingSeconds--;
}

void SandTimerWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
}

void SandTimerWindow::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton)
        move(event->globalPosition().toPoint() - dragPosition);
}

void SandTimerWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF rect = this->rect();
    QPainterPath path;
    path.addRoundedRect(rect, 15, 15);  // 圆角 15px

    // 使用动画背景色
    painter.fillPath(path, backgroundColor);

    // 边框
    QPen pen(QColor(100, 100, 100, 160)); // 灰色边框
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawPath(path);
}
