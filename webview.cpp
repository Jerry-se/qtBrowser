#include "webview.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QTimer>

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
    , childObj(nullptr)
    , moving(false)
{
    // setMouseTracking(true);
    connect(this, &QWebEngineView::renderProcessTerminated,
            [this](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode) {
        QString status;
        switch (termStatus) {
        case QWebEnginePage::NormalTerminationStatus:
            status = tr("Render process normal exit");
            break;
        case QWebEnginePage::AbnormalTerminationStatus:
            status = tr("Render process abnormal exit");
            break;
        case QWebEnginePage::CrashedTerminationStatus:
            status = tr("Render process crashed");
            break;
        case QWebEnginePage::KilledTerminationStatus:
            status = tr("Render process killed");
            break;
        }
        QMessageBox::StandardButton btn = QMessageBox::question(window(), status,
                                                   tr("Render process exited with code: %1\n"
                                                      "Do you want to reload the page ?").arg(statusCode));
        if (btn == QMessageBox::Yes)
            QTimer::singleShot(0, [this] { reload(); });
    });
}

bool WebView::eventFilter(QObject *watched, QEvent *event)
{
//    if (watched == view) {
    if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << "mouse press";
        QMouseEvent *ev = dynamic_cast<QMouseEvent*>(event);
        pressPoint = ev->globalPosition();
        qDebug() << pressPoint;
    } else if (event->type() == QEvent::MouseMove) {
        QMouseEvent *ev = dynamic_cast<QMouseEvent*>(event);
        QPointF cur = ev->globalPosition();
        qDebug() << cur;
        if (moving) {
            QPointF dp = cur - pressPoint;
            QPoint ptWin = window()->pos();
            window()->move(ptWin.x() + dp.x(), ptWin.y() + dp.y());
        }
        pressPoint = cur;
    }
//    }
    return false;
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    // TODO: do nothing
}

bool WebView::event(QEvent *evt)
{
    if(evt->type()== QEvent::ChildPolished ){
        QChildEvent* ce=static_cast<QChildEvent*>(evt);
        childObj = ce->child();
        if(childObj){
            childObj->installEventFilter(this);
        }
    }
    return QWebEngineView::event(evt);
}

void WebView::startMoveWindow()
{
    moving = true;
}

void WebView::stopMoveWindow()
{
    moving = false;
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
//    BrowserWindow *mainWindow = qobject_cast<BrowserWindow*>(window());
//    if (!mainWindow)
//        return nullptr;

//    switch (type) {
//    case QWebEnginePage::WebBrowserTab: {
//        return mainWindow->tabWidget()->createTab();
//    }
//    case QWebEnginePage::WebBrowserBackgroundTab: {
//        return mainWindow->tabWidget()->createBackgroundTab();
//    }
//    case QWebEnginePage::WebBrowserWindow: {
//        return mainWindow->browser()->createWindow()->currentTab();
//    }
//    case QWebEnginePage::WebDialog: {
//        WebPopupWindow *popup = new WebPopupWindow(page()->profile());
//        connect(popup->view(), &WebView::devToolsRequested, this, &WebView::devToolsRequested);
//        return popup->view();
//    }
//    }
    return nullptr;
}
