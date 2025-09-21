#include <opencv2/opencv.hpp>
#include "xlibutil.h"
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <X11/Xatom.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <opencv2/dnn_superres.hpp>
using namespace cv::dnn_superres;

extern "C" {
    #include <xdo.h>
}

Xlibutil::Xlibutil()
{

}

void Xlibutil::xwindowClose(Window window)
{

    Display *display = QX11Info::display();

    Atom atom = XInternAtom(display, "_NET_CLOSE_WINDOW", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = atom;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = atom;
    xev.xclient.data.l[1] = CurrentTime;

    XSendEvent(display, DefaultRootWindow(display), false, SubstructureNotifyMask, &xev);
    XFlush(display);
}

QRect Xlibutil::primaryDisplayDimensions()
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    return screenGeometry;
}

void Xlibutil::xreservedSpace(Window window, int h)
{
    Display *display = XOpenDisplay(NULL);
    QRect geometry = this->primaryDisplayDimensions();
    Screen* scr = DefaultScreenOfDisplay(display); // Get the default screen
    int xorgscreenHeight = scr->height;
    int xorgscreenWidth = scr->width;

    // set in xorg reserved space in desktop
    long prop[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, geometry.x(), geometry.width()};

    int extraMargin = xorgscreenHeight - geometry.height();
    if (geometry.y() != 0){
        prop[3] = h;
    } else {
        prop[3] = h + extraMargin;
    }

    XChangeProperty(display, window,
                    XInternAtom(display, "_NET_WM_STRUT_PARTIAL", false),
                    XA_CARDINAL, 32, PropModeReplace, (unsigned char *)&prop, 12);
    XFlush(display);
}

void Xlibutil::xchange(Window window, const char * atom)
{
    Display *display = QX11Info::display();

    Atom tmp = XInternAtom(display, atom, false);

    XChangeProperty(display, window,
                    XInternAtom(display, "_NET_WM_WINDOW_TYPE", false),
                    XA_ATOM, 32, PropModeReplace, (unsigned char *)&tmp, 1);
    XFlush(display);
}

char* Xlibutil::xwindowType(Window window)
{
    Display *display = QX11Info::display();
    Atom nameAtom = XInternAtom(display,"_NET_WM_WINDOW_TYPE",false);
    Atom type;
    int format;
    unsigned long nitems, after;
    unsigned char *data;
    int status;

    status = XGetWindowProperty(display, window, nameAtom, 0L, LONG_MAX,
                                         false, XA_ATOM, &type, &format,
                                         &nitems, &after, &data);

    if (status != Success || nitems == 0) return "_NET_WM_WINDOW_TYPE_NORMAL";

    Atom prop = ((Atom *)data)[0];
    data = (unsigned char *)XGetAtomName(display, prop);
    XFlush(display);
    return (char *)data;
}

unsigned char* Xlibutil::xwindowState(Window window)
{
    Display *display = QX11Info::display();
    Atom nameAtom = XInternAtom(display,"_NET_WM_STATE", true);
    Atom type;
    int format;
    unsigned long nitems, after;
    unsigned char *data = 0;
    int status;

    status = XGetWindowProperty(display, window, nameAtom, 0L, LONG_MAX,
                                         false, XA_ATOM, &type, &format,
                                         &nitems, &after, &data);

    if (status != Success || nitems == 0) return 0x0;

    Atom prop = ((Atom *)data)[0];
    data = (unsigned char *)XGetAtomName(display, prop);
    XFlush(display);
    return data;
}

char* Xlibutil::xwindowClass(Window window)
{
    Display *display = QX11Info::display();
    int status;
    unsigned long nitems;
    unsigned char* data;
    data = this->windowProperty(display, window, "WM_CLASS", &nitems, &status);

    if (status != Success || nitems == 0) return "unknow";
    XFlush(display);
    return (char*)data;
}

char* Xlibutil::xwindowName(Window window)
{
     Display *display = QX11Info::display();
     Atom nameAtom = XInternAtom(display,"_NET_WM_NAME",false);
     Atom utf8Atom = XInternAtom(display,"UTF8_STRING",false);
     Atom type;
     int format;
     unsigned long nitems, after;
     unsigned char *data;
     int status;

    status = XGetWindowProperty(display, window, nameAtom, 0, 65536,
                                         false, utf8Atom, &type, &format,
                                         &nitems, &after, &data);

    if (status != Success || nitems == 0) return "unknow";
    XFlush(display);
    return (char *)data;
}

int Xlibutil::xwindowPid(Window window)
{
    Display *display = QX11Info::display();
    unsigned char *data;
    unsigned long nitems;
    int window_pid = 0;
    int status;

    data = this->windowProperty(display, window, "_NET_WM_PID", &nitems, &status);
    window_pid = (int) *((unsigned long *)data);

    if (status != Success || nitems == 0) return 0;
    XFlush(display);
    return window_pid;
}

unsigned char* Xlibutil::windowProperty(Display *display, Window window, const char *arg, unsigned long *nitems, int *status)
{
    Atom type;
    int size, actual_format;
    //unsigned long nitems;
    unsigned char *data;

    /*unsigned long nbytes;*/
    unsigned long bytes_after; /* unused */

    Atom tmp = XInternAtom(display, arg, false);

    // 4096 / 4 , (~0L)

    *status = XGetWindowProperty(display, window,tmp, 0, (~0L),
                                  false, AnyPropertyType, &type,
                                  &actual_format, nitems, &bytes_after,
                                  &data);
    return data;
}

XWindowAttributes Xlibutil::attrWindow(Display *display, Window window)
{
    XWindowAttributes attr;
    XGetWindowAttributes(display, window, &attr);
    return attr;
}

void Xlibutil::resizeWindow(Display *display, Window window, int x, int y, unsigned int w, unsigned int h)
{
    XMoveResizeWindow(display, window, x, y, w, h);
}

void Xlibutil::xactive(Window window)
{
    Display *display = QX11Info::display();
    XEvent xev;
    Atom wm_state  =  XInternAtom(display, "_NET_ACTIVE_WINDOW", false);
    Atom win_min  =  XInternAtom(display, "_NET_ACTIVE_WINDOW", false);

    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = window;
    xev.xclient.message_type = wm_state;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = win_min;
    xev.xclient.data.l[1] = CurrentTime;
    XSendEvent(display, DefaultRootWindow(display), false, SubstructureNotifyMask, &xev);
    XFlush(display);
}

void Xlibutil::xminimize(Window window)
{
    Display *display = QX11Info::display();
    int screen;
    XWindowAttributes attr = this->attrWindow(display, window);
    screen = XScreenNumberOfScreen(attr.screen);
    XIconifyWindow(display, window, screen);
    XFlush(display);
}

Window* Xlibutil::xwindows(Display *display, unsigned long *size)
{
    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", true);
        Atom actual_type;
        int actual_format;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, size, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);
        XFlush(display);
        return lists;
    }

    return NULL;
}

Window Xlibutil::xwindowID(int pid)
{
    Display *display = QX11Info::display();

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", true);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (pid == this->xwindowPid(lists[i]))
            {
                XFree(prop);
                XFlush(display);
                return lists[i];
            }
        }
    }

    XFlush(display);
    return NULL;
}

Window Xlibutil::xwindowIdByClass(QString wmclass)
{
    Display *display = QX11Info::display();

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", true);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
            {
                XFree(prop);
                XFlush(display);
                return lists[i];
            }
        }
    }

    XFlush(display);
    return NULL;
}

bool Xlibutil::xsingleActive(QString wmclass)
{
    Display *display = QX11Info::display();
    Atom atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", true);
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop;

    int status = XGetWindowProperty(display, this->xwindowIdByClass(wmclass), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

    if ((unsigned char*)prop != 0x0)
    {
        XFree(prop);
        XFlush(display);
        return true;
    }

    XFree(prop);
    XFlush(display);
    return false;
}

bool Xlibutil::xisActive(QString wmclass)
{
    Display *display = QX11Info::display();

    Atom atom = XInternAtom(display, "_NET_ACTIVE_WINDOW", true);
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop;

    int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

    Window *lists = (Window *)((unsigned long *)prop);

    for (int i = 0; i < nitems; i++)
    {
        if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
        {
            XFree(prop);
            XFlush(display);
            return 1;
        }
    }

    XFlush(display);
    return 0;
}

void Xlibutil::xminimizeByClass(QString wmclass)
{
    Display *display = QX11Info::display();

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", true);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);
        int minimized = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
            {
                this->xminimize(lists[i]);
            }
        }

        XFree(prop);
        XFlush(display);
    }
}

void Xlibutil::xactiveByClass(QString wmclass)
{
    Display *display = QX11Info::display();

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", true);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
                this->xactive(lists[i]);
        }

        XFlush(display);
        XFree(prop);
    }
}

bool Xlibutil::xwindowExist(QString wmclass)
{
    Display *display = QX11Info::display();

    if (display)
    {
        Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", true);
        Atom actual_type;
        int actual_format;
        unsigned long nitems;
        unsigned long bytes_after;
        unsigned char *prop;

        int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, false, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        Window *lists = (Window *)((unsigned long *)prop);

        for (int i = 0; i < nitems; i++)
        {
            if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
            {
                XFree(prop);
                XFlush(display);
                return true;
            }
        }

        XFlush(display);
        XFree(prop);
    }
    return false;
}

QString Xlibutil::xwindowLauncher(Window window)
{
    Display *display = QX11Info::display();
    unsigned char *data;
    unsigned long nitems;
    QString launcher;
    int status;

    data = this->windowProperty(display, window, "_BAMF_DESKTOP_FILE", &nitems, &status);
    launcher = (char *)data;
    XFlush(display);

    return launcher;
}

void Xlibutil::xaddDesktopFile(int pid, QString arg)
{
    Display *display = QX11Info::display();
    //unsigned char* deskfile = (unsigned char*)"/usr/share/applications/xfce4-terminal.desktop";
    unsigned char* deskfile = (unsigned char*)arg.toUtf8().constData();
    int status;

    if(display)
    {
        XFlush(display);
        Atom atom = XInternAtom(display, "_BAMF_DESKTOP_FILE", false);
        status = XChangeProperty(display, this->xwindowID(pid), atom, XA_STRING, 8, PropModeReplace, deskfile, arg.length());
        XFlush(display);
    }
}

cv::Mat applyAlphaChannel(const cv::Mat &upscaledBGR, const cv::Mat &originalBGRA) {
    // Extrai o canal alfa do original
    std::vector<cv::Mat> origChannels;
    cv::split(originalBGRA, origChannels);
    cv::Mat alpha = origChannels[3];

    // Redimensiona o alfa para o tamanho da imagem upscaleada
    cv::Mat alphaUpscaled;
    cv::resize(alpha, alphaUpscaled, upscaledBGR.size(), 0, 0, cv::INTER_LINEAR);

    // Converte BGR -> BGRA
    cv::Mat upscaledBGRA;
    cv::cvtColor(upscaledBGR, upscaledBGRA, cv::COLOR_BGR2BGRA);

    // Substitui o canal alfa
    std::vector<cv::Mat> channels;
    cv::split(upscaledBGRA, channels);
    channels[3] = alphaUpscaled;
    cv::merge(channels, upscaledBGRA);

    return upscaledBGRA;
}

cv::Mat QImageToMat(const QImage &img) {
    QImage converted = img.convertToFormat(QImage::Format_RGBA8888);
    return cv::Mat(converted.height(), converted.width(), CV_8UC4, (void*)converted.bits()).clone();
}

QImage MatToQImage(const cv::Mat &mat) {
    cv::Mat tmp;
    if(mat.channels() == 4)
        tmp = mat.clone();
    else if(mat.channels() == 3)
        cv::cvtColor(mat, tmp, cv::COLOR_BGR2BGRA);
    else if(mat.channels() == 1)
        cv::cvtColor(mat, tmp, cv::COLOR_GRAY2BGRA);
    return QImage(tmp.data, tmp.cols, tmp.rows, tmp.step, QImage::Format_RGBA8888).copy();
}

QImage upscaleWithOpenCV(const QImage &input) {
    cv::Mat mat = QImageToMat(input);

    DnnSuperResImpl superResolution;
    superResolution.readModel("ESPCN_x4.pb"); // caminho do modelo OpenCV ESRGAN
    superResolution.setModel("espcn", 4);     // nome do modelo, fator de upscale 4

    cv::Mat out;
    cv::Mat bgrImage;
    cv::cvtColor(mat, bgrImage, cv::COLOR_BGRA2BGR);
    superResolution.upsample(bgrImage, out);        // aplica super-resolution
    cv::Mat outBGRA = applyAlphaChannel(out, mat);

    return MatToQImage(outBGRA);
}

QPixmap renderWmIcon(ulong* data, QPixmap map)
{
    QImage image (data[0], data[1], QImage::Format_ARGB32);

    for (int i=0; i<image.byteCount() / 4; ++i)
    {
        ((uint*)image.bits())[i] = data[i + 2];
    }

    QImage upscaledImg = upscaleWithOpenCV(image);
    QImage upscaled2xImg = upscaleWithOpenCV(upscaledImg);
    return QPixmap::fromImage(upscaled2xImg);
}

QPixmap Xlibutil::xwindowIcon(Window window)
{
    Display *display = QX11Info::display();
    Atom type;
    int format, revert_to;
    unsigned long bytes_after, nitems;
    ulong* data;
    Window windowFocus;

    XGetInputFocus(display, &windowFocus, &revert_to);
    Atom prop = XInternAtom(display, "_NET_WM_ICON", false);
    XGetWindowProperty(display, window, prop, 0, LONG_MAX, false, AnyPropertyType,
                       &type, &format, &nitems, &bytes_after, (uchar**)&data);

    QPixmap map;

    if (data != 0x0){
        map = renderWmIcon(data, map);
    } else {
        QImage image;
        image.load("./xfce-unknown.svg");
        map = QPixmap::fromImage(image);
    }

    XFree(data);
    XFlush(display);
    return map;
}
