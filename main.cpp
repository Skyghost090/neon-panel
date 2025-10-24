#include <QDesktopWidget>

#include "threads.h"
#include "qquickimage.h"
#include <cstring>
#include <stdio.h>
#include <X11/Xlib.h>
#include <toml++/toml.hpp>
#include <csignal>
#include <iostream>
#include <stdlib.h>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickImageProvider>
#include <QStyle>

class StandardIconProvider : public QQuickImageProvider
{
public:
    StandardIconProvider(QStyle *style)
    : QQuickImageProvider(Pixmap)
    , m_style(style)
    {}

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        Q_UNUSED(size)
        static const auto metaobject = QMetaEnum::fromType<QStyle::StandardPixmap>();
        const int value = metaobject.keyToValue(id.toLatin1());
        QIcon icon = m_style->standardIcon(static_cast<QStyle::StandardPixmap>(value));
        return icon.pixmap(requestedSize);
    }

    QStyle *m_style;
};

auto config = toml::parse_file("config.toml");

int getConfigHeight()
{
    int h;
    try {
        h = config["dimensions"]["height"].value_or(0);
    } catch (const toml::parse_error& err) {
        h = 40;
    }
    return h;
}

bool getOnTopConfig(QWindow *window, QRect screenGeometry)
{
    std::optional<std::string> position = config["dimensions"]["panelPosition"].value<std::string>();
    int height = getConfigHeight();
    if(position == "top"){
        window->setGeometry(screenGeometry.x(), 0,screenGeometry.width(), height);
        return true;
    } else if(position == "bottom") {
        window->setGeometry(screenGeometry.x(), screenGeometry.height() + screenGeometry.y() - height,screenGeometry.width(), height);
        return false;
    }
}

void ifDisplayIsOpened()
{
    Display *display;
    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    XCloseDisplay(display);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QDesktopWidget desktop;
    Context context;
    context.basepath = app.applicationDirPath();
    QQuickImage image;
    image.ctx = &context;

    ifDisplayIsOpened();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Context", &context);
    engine.addImageProvider(QLatin1String("standardicons"), new StandardIconProvider(app.style()));
    engine.addImageProvider("pixmap", &image);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *main = engine.rootObjects().first();
    QWindow *window = qobject_cast<QWindow *>(main); // (QWindow *)main
    QRect screenGeometry = context.primaryDisplayDimensions();
    window->setProperty("mainId", window->winId());
    window->xChanged(Qt::WA_X11DoNotAcceptFocus | Qt::WA_X11NetWmWindowTypeDock);
    app.setAttribute(Qt::AA_X11InitThreads);

    Threads *threads = new Threads();
    threads->main = window;
    threads->start();

    context.xchange(window->winId(), "_NET_WM_WINDOW_TYPE_DOCK");
    context.xreservedSpace(window->winId(), window->height(), getOnTopConfig(window, screenGeometry));
    return app.exec();
}
