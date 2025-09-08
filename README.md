**SandTimer** 是一个基于 **Qt** 和 **TCP Socket** 的轻量级计时器管理工具（自用）。
 它通过监听本地端口接收控制指令，支持远程或本地命令控制计时器的创建、取消和重置操作。
 每个计时器以悬浮窗口的形式展示，支持自由拖动，默认显示在屏幕右侧。
 当计时归零时，应用会播放提示音，并继续以负时间形式显示超时持续时长。

This project uses Qt6 (Core, Gui, Widgets, Multimedia), which is licensed under the LGPL v3.
See third_party_notices/Qt-LGPL.txt for details.
