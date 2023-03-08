#include "devtools_window.h"

#include <QApplication>
#include <QScreen>
#include <QVBoxLayout>

#include "webview.h"

DevToolsWindow::DevToolsWindow(QWidget *parent)
    : QMainWindow{parent}
    , view(nullptr)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::ClickFocus);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    view = new WebView(this);

    layout->addWidget(view);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    connect(view, &WebView::titleChanged, this, &DevToolsWindow::handleWebViewTitleChanged);

    // view->resize()
    view->show();
}

QSize DevToolsWindow::sizeHint() const
{
    QRect desktopRect = QApplication::primaryScreen()->geometry();
    QSize size = desktopRect.size() * qreal(0.9);
    return size;
}

void DevToolsWindow::handleWebViewTitleChanged(const QString &title)
{
    setWindowTitle(title);
}

WebView *DevToolsWindow::webView() const
{
    return view;
}
