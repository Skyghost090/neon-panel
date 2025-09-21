#include "threads.h"
#include <stdio.h>
#include <stdlib.h>

unsigned long size;
Display *display = XOpenDisplay(0);

void Threads::createWindowicon(Context& context, int& i)
{
    QString type = context.xwindowType(this->client_list[i]);

    if (type == "_NET_WM_WINDOW_TYPE_NORMAL" || type == "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE")
    {
        this->noWindow = false;
        this->renderThread[i] = new Threads;
        this->main->connect(renderThread[i], SIGNAL(onDesktopWindow(QString, QString, int)), this->main, SIGNAL(desktopWindow(QString, QString, int)));
        this->renderThread[i]->onDesktopWindow(context.xwindowName(this->client_list[i]), QString(context.xwindowClass(this->client_list[i])).toLower(), (int)this->client_list[i]);
    }
}

void Threads::renderWindowlist(Context& context)
{
    this->client_list = context.xwindows(display, &size);

    if (this->client_list != NULL)
    {
        this->main->setProperty("windowVerify", true);
        this->noWindow = true;

        for (int i = 0; i < size; i++)
        {
            this->createWindowicon(context, i);
        }

        if (this->noWindow)
        {
            this->clearwindowThread = new Threads;
            this->main->connect(clearwindowThread, SIGNAL(onClearWindows()), this->main, SIGNAL(clearWindows()));
            this->clearwindowThread->onClearWindows();
        }

        XFree(this->client_list);
    }
}

void Threads::run()
{
    QObject *obj;
    if(!display)
        exit(2);

    Context context;
    XSelectInput(display, DefaultRootWindow(display), SubstructureNotifyMask);
    XEvent dispayEvent;

    while (true)
    {
        XNextEvent(display, &dispayEvent);
        QThread::msleep(17);

        if (dispayEvent.type == CreateNotify || dispayEvent.type == DestroyNotify)
            this->renderWindowlist(context);

        XFlush(display);
    }
}
