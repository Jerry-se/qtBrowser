#include "main_window.h"

#include <QElapsedTimer>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWebChannel>

#include "bridge.h"
#include "webview.h"
#include "devtools_window.h"

MainWindow::MainWindow(bool forDevTools)
    : QMainWindow(nullptr)
    , bridge(nullptr)
    , view(nullptr)
    , progressBar(nullptr)
    , devTools(nullptr)
{
    if (!forDevTools)
        this->setWindowFlags(Qt::FramelessWindowHint);

    // setAttribute(Qt::WA_DeleteOnClose, true);
    setFocusPolicy(Qt::ClickFocus);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    if (forDevTools) {
        QHBoxLayout *devLayout = new QHBoxLayout;
        devLayout->setSpacing(0);
        devLayout->setContentsMargins(5, 5, 5, 5);

        QLineEdit *urlEdit = new QLineEdit(this);
        urlEdit->setObjectName("leUrl");
        urlEdit->setFixedWidth(600);
        // urlEdit->setMaximumWidth(600);
        connect(urlEdit, &QLineEdit::returnPressed, [this, urlEdit]() {
            if (view) view->setUrl(QUrl::fromUserInput(urlEdit->text()));
        });
        devLayout->addWidget(urlEdit);

        QPushButton *pbtnReload = new QPushButton(this);
        pbtnReload->setText("Reload");
        pbtnReload->setFixedWidth(72);
        connect(pbtnReload, &QPushButton::clicked, [this]() {
            if (view) view->reload();
        });
        devLayout->addSpacing(10);
        devLayout->addWidget(pbtnReload);

        QPushButton *pbtnDevTools = new QPushButton(this);
        pbtnDevTools->setText("Dev Tools");
        pbtnDevTools->setFixedWidth(72);
        connect(pbtnDevTools, &QPushButton::clicked, this, &MainWindow::on_pbtnDevTools_clicked);
        devLayout->addSpacing(10);
        devLayout->addWidget(pbtnDevTools, 0, Qt::AlignLeft);

        QLabel *lblInput = new QLabel("Input:", this);
        lblInput->setMaximumHeight(16);
        devLayout->addWidget(lblInput, 0, Qt::AlignRight);
        QLineEdit *leInput = new QLineEdit(this);
        leInput->setObjectName("leInput");
        leInput->setFixedWidth(100);
        devLayout->addSpacing(10);
        devLayout->addWidget(leInput);
        QLabel *lblOutput = new QLabel("", this);
        lblOutput->setObjectName("lblOutput");
        lblOutput->setFixedWidth(100);
        lblOutput->setMaximumHeight(16);
        devLayout->addSpacing(10);
        devLayout->addWidget(lblOutput);
        QPushButton *pbtnCallJsFunc = new QPushButton("Call JS Func", this);
        pbtnCallJsFunc->setFixedWidth(100);
        connect(pbtnCallJsFunc, &QPushButton::clicked, this, &MainWindow::on_pbtnCallJsFunc_clicked);
        devLayout->addSpacing(10);
        devLayout->addWidget(pbtnCallJsFunc);
        QLabel *lblTime = new QLabel(this);
        lblTime->setObjectName("lblTime");
        lblTime->setMaximumHeight(16);
        devLayout->addSpacing(10);
        devLayout->addWidget(lblTime);

        layout->addLayout(devLayout);

        progressBar = new QProgressBar(this);
        progressBar->setMaximum(100);
        progressBar->setMinimum(0);
        progressBar->setMaximumHeight(2);
        progressBar->setMinimumHeight(2);
        progressBar->setTextVisible(false);
        layout->addWidget(progressBar);
    }

    bridge = new Bridge();
    connect(bridge, &Bridge::sig_showMinimized, this, &MainWindow::on_bridge_minisized);
    connect(bridge, &Bridge::sig_closeWindow, this, &MainWindow::on_bridge_closeWindow);
    connect(bridge, &Bridge::sig_moveWindow, this, &MainWindow::on_bridge_moveWindow);

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("doc", bridge);
    view = new WebView(this);
    view->page()->setWebChannel(channel);
    connect(view, &QWebEngineView::loadStarted, this, &MainWindow::on_webView_loadStarted);
    if (forDevTools)
        connect(view, &QWebEngineView::loadProgress, progressBar, &QProgressBar::setValue);
    connect(view, &QWebEngineView::loadFinished, this, &MainWindow::on_webView_loadFinished);

    connect(bridge, &Bridge::sig_startMoveWindow, view, &WebView::startMoveWindow);
    connect(bridge, &Bridge::sig_stopMoveWindow, view, &WebView::stopMoveWindow);

    layout->addWidget(view);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    view->show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setUrl(const QString &url)
{
    view->setUrl(QUrl::fromUserInput(url));
    QLineEdit *urlEdit = findChild<QLineEdit*>("leUrl");
    if (urlEdit) urlEdit->setText(url);
}

void MainWindow::on_webView_loadStarted()
{
    qDebug() << "webview load started";
    QPushButton *btn = findChild<QPushButton*>("pbtnReload");
    if (btn) btn->setEnabled(false);
}

void MainWindow::on_webView_loadFinished(bool ok)
{
    qDebug() << "webview load finished: " << ok;
    QPushButton *btn = findChild<QPushButton*>("pbtnReload");
    if (btn) btn->setEnabled(true);
}

void MainWindow::on_pbtnDevTools_clicked()
{
    if (!devTools) {
        devTools = new DevToolsWindow(nullptr);
        QObject::connect(devTools, &QObject::destroyed, [this]() { devTools = nullptr; });
        view->page()->setDevToolsPage(devTools->webView()->page());
        view->page()->triggerAction(QWebEnginePage::InspectElement);
    }
    devTools->show();
}

void MainWindow::on_pbtnCallJsFunc_clicked()
{
    QLineEdit *leInput = findChild<QLineEdit*>("leInput");
    QString strInput = leInput->text();
    QLabel *lblOutput = findChild<QLabel*>("lblOutput");
    if (strInput.isEmpty()) {
        lblOutput->setText("input is empty");
    } else {
        // QTime t1 = QTime::currentTime();
        QElapsedTimer t1;
        t1.start();
        view->page()->runJavaScript("document.title", [](const QVariant &v) { qDebug() << v.toString(); });
        // QTime t2 = QTime::currentTime();
        float interval1 = t1.nsecsElapsed() * 1e-6;
        qDebug() << interval1 << "ms";
        t1.restart();
        view->page()->runJavaScript(QString("CallByCPP(\"%1\")").arg(strInput),
                                    [lblOutput](const QVariant &v) { lblOutput->setText(v.toString()); });
        // qint64 interval1 = t1.msecsTo(t2);
        // qDebug() << interval1 << " " << interval2;
        float interval2 = t1.nsecsElapsed() * 1e-6;
        qDebug() << interval2 << "ms";
        QLabel *lblTime = findChild<QLabel*>("lblTime");
        if (lblTime)
            lblTime->setText(QString("%1ms, %2ms").arg(interval1).arg(interval2));
    }
}

void MainWindow::on_bridge_minisized()
{
    this->showMinimized();
}

void MainWindow::on_bridge_closeWindow()
{
    this->close();
}

void MainWindow::on_bridge_moveWindow(int dx, int dy)
{
    QPoint curPos = window()->pos();
    window()->move(curPos.x() + dx, curPos.y() + dy);
}

