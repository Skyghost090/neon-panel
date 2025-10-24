#ifndef THREADS_H
#define THREADS_H

#include <QThread>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QWindow>
#include <QObject>
#include <QDebug>
#include <QX11Info>
#include <QQuickImageProvider>
#include <QList>
#include <QVariant>
#include "context.h"

class Threads : public QThread
{

    Q_OBJECT

public:
    void run();
    QWindow *main;
    void renderWindowlist(Context& context);
    void modifiedWindowList(Context& context);

private:
    QList<QVariant> windowList;
    Window *client_list;
    void createWindowicon(Context& context, int& i);
    void CreateWindowicon(Context& context, int& i);
    void renderTrayIcons(Context& context, XEvent event);
    bool noWindow;
    Threads *windowlistThread[99], *renderThread[99], *clearwindowThread, *guiThread;

signals:
    void onCreate();
    void onDestroy();
    void onDesktopWindow(QString nome, QString wmclass, int winId, int size);
    void onClearWindows();
};

#endif // THREADS_H
