#ifndef DEVTOOLSWINDOW_H
#define DEVTOOLSWINDOW_H

#include <QMainWindow>

class WebView;

class DevToolsWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit DevToolsWindow(QWidget *parent = nullptr);

    QSize sizeHint() const override;

    WebView *webView() const;

signals:

private slots:
    void handleWebViewTitleChanged(const QString &title);

private:
    WebView *view;

};

#endif // DEVTOOLSWINDOW_H
