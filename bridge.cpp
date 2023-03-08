#include "bridge.h"

#include <QMessageBox>

Bridge::Bridge(QObject *parent)
    : QObject{parent}
{

}

QString Bridge::say(const QString &message)
{
    // return QVariant(message);
    return "C++ received: " + message;
}

void Bridge::showMessageBox(const QString &message)
{
    QMessageBox::information(nullptr, "JS Info", message);
}

void Bridge::showMinimized(void)
{
    emit sig_showMinimized();
}

void Bridge::closeWindow(void)
{
    emit sig_closeWindow();
}

void Bridge::moveWindow(int dx, int dy)
{
    emit sig_moveWindow(dx, dy);
}

void Bridge::startMoveWindow()
{
    emit sig_startMoveWindow();
}

void Bridge::stopMoveWindow()
{
    emit sig_stopMoveWindow();
}
