#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Bridge;
class WebView;
class QProgressBar;
class DevToolsWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool forDevTools = false);
    ~MainWindow();

    void setUrl(const QString &url);

private slots:
    void on_webView_loadStarted();
    void on_webView_loadFinished(bool ok);
    void on_pbtnDevTools_clicked();
    void on_pbtnCallJsFunc_clicked();

    // 处理 JS 调用的槽函数
    void on_bridge_minisized();
    void on_bridge_closeWindow();
    void on_bridge_moveWindow(int dx, int dy);

private:
    Bridge *bridge;
    WebView *view;
    QProgressBar *progressBar;
    DevToolsWindow *devTools;

};
#endif // MAINWINDOW_H
