#include <QDesktopWidget>

#include "threads.h"
#include "qquickimage.h"
#include <cstring>
#include <stdio.h>
#include <X11/Xlib.h>
#include <toml++/toml.hpp>
#include <csignal>
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


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int h;
    try {
        auto config = toml::parse_file("config.toml");
        h = config["dimensions"]["height"].value_or(0);
    } catch (const toml::parse_error& err) {
        h = 40;
    }

    QDesktopWidget desktop;
    Context context;
    context.basepath = app.applicationDirPath();

    QQuickImage image;
    image.ctx = &context;

    Display *display;
    // Open a connection to the X server
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        return 1;
    }
    Screen* scr = DefaultScreenOfDisplay(display); // Get the default screen
    int xorgscreenHeight = scr->height;
    int xorgscreenWidth = scr->width; // Get the screen width

    XCloseDisplay(display);

    QRect screenGeometry = context.primaryDisplayDimensions();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    Threads *threads = new Threads();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("Context", &context);
    engine.addImageProvider(QLatin1String("standardicons"), new StandardIconProvider(app.style()));
    engine.addImageProvider("pixmap", &image);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    QObject *main = engine.rootObjects().first();
    QWindow *window = qobject_cast<QWindow *>(main); // (QWindow *)main
    window->setGeometry(screenGeometry.x(), screenHeight + screenGeometry.y() - h,screenWidth, h);

    window->setProperty("mainId", window->winId());
    window->xChanged(Qt::WA_X11DoNotAcceptFocus | Qt::WA_X11NetWmWindowTypeDock);

    app.setAttribute(Qt::AA_X11InitThreads);

    threads->main = window;
    threads->start();

    context.xchange(window->winId(), "_NET_WM_WINDOW_TYPE_DOCK");
    context.xreservedSpace(window->winId(), window->height());

    return app.exec();
}
