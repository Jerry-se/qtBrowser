#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr);

    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;
    bool event(QEvent *) override;

public slots:
    void startMoveWindow();
    void stopMoveWindow();

private:
    QPointF pressPoint;
    QObject *childObj;
    bool moving;

};

#endif // WEBVIEW_H
