#include "main_window.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>

#define APPLICATION_NAME "qtBrowser"
//#define DEFAULT_URL "https://www.baidu.com"
//#define DEFAULT_URL "http://192.168.1.104:8080/"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(APPLICATION_NAME);
    QApplication::setApplicationVersion("1.0.0.1");

    // QString url = DEFAULT_URL;
    QString url = QDir::currentPath();
    url.append("/index.html");
    bool devTools = false;

    QCommandLineParser parser;
    parser.setApplicationDescription(APPLICATION_NAME" Command Helper");
    parser.addHelpOption();
    parser.addVersionOption();
    const QCommandLineOption urlOption("url", "The url of the front-end page of DeepLink");
    const QCommandLineOption devOption("dev", "developer mode");
    parser.addOption(urlOption);
    parser.addOption(devOption);
    // Process the actual command line arguments given by the user
    parser.process(app);
    if (parser.isSet(urlOption)) url = parser.value(urlOption);
    if (parser.isSet(devOption)) devTools = true;

    int width = 1280, height = 875;
    if (devTools) height += 36;

    MainWindow w(devTools);
    w.setUrl(url);
    w.resize(width, height);
    w.show();
    return app.exec();
}
