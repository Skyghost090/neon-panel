#include "threads.h"
#include <stdio.h>
#include <X11/extensions/Xdamage.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <algorithm>
#include <iostream>

unsigned long size;
int windowSize;
Display *display = XOpenDisplay(0);

void Threads::createWindowicon(Context& context, int& i)
{
    QString type = context.xwindowType(this->client_list[i]);
    printf("type: %s\n", type.toUtf8().constData());
    if (type == "_NET_WM_WINDOW_TYPE_NORMAL" || type == "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE")
    {
        windowSize++;
        this->noWindow = false;
        this->renderThread[i] = new Threads;
        this->main->connect(renderThread[i], SIGNAL(onDesktopWindow(QString, QString, int, int)), this->main, SIGNAL(desktopWindow(QString, QString, int, int)));
        this->renderThread[i]->onDesktopWindow(context.xwindowName(this->client_list[i]), QString(context.xwindowClass(this->client_list[i])).toLower(), (int)this->client_list[i], (int)i);
        printf("%s\n", QString(context.xwindowClass(this->client_list[i])).toLower().toUtf8().constData());
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

void Threads::renderTrayIcons(Context& context, XEvent event)
{
    printf("trayIcon\n");
    QList<QString> trayList;
    QList<Window> _trayList;
    XDamageNotifyEvent *dEvent;
    Window id = event.xclient.data.l[2];
    QString wclass = QString(context.xwindowClass(id));
    bool add = true;

    if (context.xwindowIcon(id).isNull())
    {
        add = false;
    }

    // qDebug() << add << wclass << context.xwindowClass(dEvent->drawable);
    if (wclass != "unknow")
    {

        for (int i = 0; i < trayList.length(); i++)
        {
            if (trayList.at(i) == wclass)
            {
                add = false;
                break;
            }
        }

        for (int i = 0; i < _trayList.length(); i++)
        {
            if (_trayList.at(i) == id)
            {
                add = false;
                break;
            }
        }

        QString type = QString(context.xwindowType(id));

        if (type == "_NET_WM_WINDOW_TYPE_NORMAL" || type == "_KDE_NET_WM_WINDOW_TYPE_OVERRIDE")
        {
            if (add)
            {
                int status;
                unsigned long nitems;

                QString args = QString::number(id);// + ";" + wclass;
                QMetaObject::invokeMethod(this->main, "addTrayIcon",
                                          Q_ARG(QVariant,  args)
                );

                trayList.append(wclass);
            }
        }
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
    this->client_list = context.xwindows(display, &size);

    while (true)
    {
        XNextEvent(display, &dispayEvent);
        QThread::msleep(600);
        context.updatePanelWidth(this->main);

        if (dispayEvent.type == CreateNotify || dispayEvent.type == DestroyNotify)
            this->renderWindowlist(context);

        if (dispayEvent.type == ClientMessage)
            this->renderTrayIcons(context, dispayEvent);

        XFlush(display);
    }
}
