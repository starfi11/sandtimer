#include "SandTimerWindow.h"
#include <QFont>
#include <QPalette>

SandTimerWindow::SandTimerWindow(QWidget* parent)
    : QWidget(parent), remainingSeconds(0)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(150, 80);

    timeLabel = new QLabel("00:00", this);
    QFont font;
    font.setPointSize(24);
    font.setBold(true);
    timeLabel->setFont(font);
    timeLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(timeLabel);
    layout->setContentsMargins(10, 10, 10, 10);
    setLayout(layout);

    audioOutput = new QAudioOutput(this);
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);

    // 设置音量（0.0~1.0）
    audioOutput->setVolume(0.8);

    // 设置音频文件路径
    player->setSource(QUrl::fromLocalFile("D:/chen_education/project/Aproject/sandtimer/assets/alarm.wav"));


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SandTimerWindow::updateCountdown);
    contextMenu = new QMenu(this);
    QAction* closeAction = new QAction("关闭计时器", this);
    connect(closeAction, &QAction::triggered, this, &QWidget::close);
    contextMenu->addAction(closeAction);
}
void SandTimerWindow::contextMenuEvent(QContextMenuEvent* event)
{
    if (contextMenu)
        contextMenu->exec(event->globalPos());
}
void SandTimerWindow::startCountdown(int seconds) {
    remainingSeconds = seconds;
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
    // 播放音频（只在刚好为0秒时触发一次）
    if (remainingSeconds == 0) {
        player->play();
    }

    // 计算绝对时间
    int absSeconds = std::abs(remainingSeconds);
    int minutes = absSeconds / 60;
    int seconds = absSeconds % 60;

    QString timeStr = QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));

    // 如果是负时间，加上负号并变颜色
    if (remainingSeconds < 0) {
        timeStr = "-" + timeStr;
        timeLabel->setStyleSheet("color: red; font: bold 24px;");
    } else {
        timeLabel->setStyleSheet("color: black; font: bold 24px;");
    }

    timeLabel->setText(timeStr);

    // 倒计时继续
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

