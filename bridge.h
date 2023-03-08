#ifndef BRIDGE_H
#define BRIDGE_H

#include <QObject>

class Bridge : public QObject
{
    Q_OBJECT
public:
    explicit Bridge(QObject *parent = nullptr);

signals:
    // QString CallByCPP(const QString &input1, const QString &input2);
    void sig_showMinimized();
    void sig_closeWindow();
    void sig_moveWindow(int dx, int dy);
    void sig_startMoveWindow();
    void sig_stopMoveWindow();

public slots:
    // 供网页JS函数调用
    QString say(const QString &message);
    void showMessageBox(const QString &message);
    // 窗口最小化
    void showMinimized(void);
    // 关闭窗口
    void closeWindow(void);
    // 移动窗口
    void moveWindow(int dx, int dy);
    void startMoveWindow();
    void stopMoveWindow();

};

#endif // BRIDGE_H
