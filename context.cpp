#include "context.h"
#include <unistd.h>
#include <iostream>
#include <toml++/toml.hpp>
#include <filesystem>

QString nome, icon, tmp, execApp, wmclass, iconDefault;
QStringList list, nitems;
bool stop = false;

int Context::libraryIntLoad(int arg, QString args, QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    int num;

    if (library.load())
    {
        typedef int (*func)(int i, QString s);
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            num = f(arg, args);
    }

    return num;
}

int Context::libraryIntLoad(QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    int num;

    if (library.load())
    {
        typedef int (*func)();
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            num = f();
    }

    return num;
}

QString Context::libraryQStringLoad(int arg, QString args, QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    QString text;

    if (library.load())
    {
        typedef QString (*func)(int i, QString s);
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            text = f(arg, args);
    }

    return text;
}

QString Context::libraryQStringLoad(QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);
    QString text;

    if (library.load())
    {
        typedef QString (*func)();
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            text = f();
    }

    return text;
}

void Context::libraryVoidLoad(int arg, QString args, QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);

    if (library.load())
    {
        typedef void (*func)(int i, QString s);
        func f = (func)library.resolve(funcName.toUtf8());

        if (f);
            f(arg, args);
    }
}

void Context::libraryVoidLoad(QString funcName, QString pluginName)
{
    QLibrary library(this->basepath + "/plugins/" + pluginName);

    if (library.load())
    {
        QFunctionPointer func = library.resolve(funcName.toUtf8());

        if (func);
            func();
    }
}

QImage Context::imageOverlay(const QImage& baseImage, const QImage& overlayImage)
{
    QImage imageWithOverlay = QImage(baseImage.size(), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&imageWithOverlay);

    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(imageWithOverlay.rect(), Qt::transparent);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, baseImage);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawImage(0, 0, overlayImage);

    painter.end();

    return imageWithOverlay;
}

void Context::showMoreWindows(int winId, int h, int onTop)
{
    if (onTop == 0)
        this->xreservedSpace(winId, h, false);
    if (onTop == 1)
        this->xreservedSpace(winId, h, true);
}

void Context::sleepDelay(int time)
{
    usleep(time * 1000000);
}

QString Context::launcherFix(QString exec)
{
    QStringList list;
    list << "google-chrome-stable" << "google-chrome";
    //list << "TelegramDesktop" << "Telegram";

    for(int i = 1; i <= list.length(); i++)
    {
        if (list.at(i - 1) == exec && i % 2 == 1)
        {
            return list.at(i);
        }
    }

    return exec;
}

void Context::exec(QString pro)
{
    QString list;

    if (pro.contains("%"))
    {
        list = pro.split(" %")[0];
    }
    else
    {
        list = pro;
    }

    QProcess *process = new QProcess();
    process->start(list);
}

void Context::addDesktopFile(int pid, QString desktopFile)
{
    if (!desktopFile.isEmpty()) {
        this->xaddDesktopFile(pid, desktopFile.remove("file://"));
    }
}

QList<int> Context::windowsBywmclass(QString wmclass)
{
    Display *display = QX11Info::display();
    QList<int> list;

    Atom atom = XInternAtom(display, "_NET_CLIENT_LIST", True);
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop;

    int status = XGetWindowProperty(display, DefaultRootWindow(display), atom, 0, 4096 / 4, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop);

    Window *lists = (Window *)((unsigned long *)prop);

    for (int i = 0; i < nitems; i++)
    {
        if (wmclass == QString(this->xwindowClass(lists[i])).toLower())
        {
            list.append(lists[i]);
        }
    }

    XFree(prop);
    XFlush(display);

    return list;
}

QString Context::windowName(int window)
{
    return QString(this->xwindowName(window));
}

void Context::windowActive(int window)
{
    this->xactive(window);
}

void Context::windowClose(int window)
{
    this->xwindowClose(window);
}

QString Context::defaultIcon()
{
    QProcess process;
    process.start("xfconf-query -c xsettings -p /Net/IconThemeName");
    process.waitForFinished();
    this->defaultIconTheme = process.readLine().replace("\n", "");
    process.close();

    return "/usr/share/icons/" + this->defaultIconTheme + "/apps/scalable/default-application.svg";
}

void Context::configWmclass(){
    if (wmclass.isEmpty()) {
        if (execApp.contains(" "))
        {
            wmclass = execApp.split(" ")[0];
        }
        else
        {
            wmclass = execApp;
        }

        if (wmclass.contains("/"))
        {
            nitems = wmclass.split("/");
            wmclass = nitems[nitems.length() - 1];
        }

        wmclass = this->launcherFix(wmclass);
    }
}
void Context::iconPath(QFileInfo file, QString path)
{
    QStringList exts;
    exts << ".svg" << ".png" << ".xpm";

    foreach (QString ext, exts) {
        icon = path + "/" + this->defaultIconTheme + "/apps/scalable/" + tmp + ext;
        file.setFile(icon);

        if (file.exists()) {
            stop = true;
            break;
        }

        if(!stop) icon = "";
    }

    if (icon.isEmpty()) {
        foreach (QString ext, exts) {
            icon = "/usr/share/pixmaps/" + tmp + ext;
            file.setFile(icon);

            if (file.exists()) { stop = true; break; }

            if(!stop) icon = "";
        }
    }
}

void Context::indentPath(QFileInfo file)
{
    if (tmp.endsWith(".png")) tmp = tmp.remove(".png");
    if (tmp.endsWith(".svg")) tmp = tmp.remove(".png");
    if (tmp.endsWith(".xpm")) tmp = tmp.remove(".png");

    QIcon qicon;
    foreach (QString path, qicon.themeSearchPaths()) {
        this->iconPath(file, path);

        if (icon.isEmpty()) {
            icon = iconDefault;
        }

        if (stop) break;
    }

    list << nome << "file://" + icon << execApp << wmclass.toLower();
}

QStringList Context::addLauncher(QString app)
{
    QStringList list, nitems;
    app = app.replace("file://", "");
    QFileInfo file(app);
    QString nome, icone, tmp, exec, wmclass, iconDefault;
    iconDefault = this->defaultIcon();

    if (file.suffix() == "desktop")
    {
        this->configWmclass();

        if (!tmp.startsWith("/")) {
            indentPath(file);
        } else {
            if (tmp.contains(".ico")) tmp = iconDefault;
            if (tmp == "/") tmp = iconDefault;

            list << nome << "file://" + tmp << exec << wmclass.toLower();
        }
    } else {
        list << "" << "" << "" << "";
    }

    return list;
}

Window Context::windowId(int pid)
{
    Display *display = QX11Info::display();
    int pro_id, status;
    Window winId;
    unsigned long *desktop, size;

    Window *client_list = this->xwindows(display, &size);

    if (client_list == NULL)
    {
        XFree(client_list);
        XFlush(display);
        return winId;
    }
    for (int i = 0; i < size; i++)
    {
        Window tmpId = client_list[i];
        pro_id = this->xwindowPid(tmpId);

        if (pro_id == pid)
        {
            winId = tmpId;
            break;
        }
    }

    XFree(client_list);
    XFlush(display);
    return winId;
}

int Context::iconHidden(Window* lists, unsigned long nitems)
{
    for (int i = 0; i < nitems; i++)
    {
        if (wmclass == QString(this->xwindowClass(lists[i])))
        {
            if ((const char *)this->xwindowState(lists[i]) == "_NET_WM_STATE_HIDDEN")
            {
                XFlush(display);
                return 1;
            }
        }
    }
    return 0;
}

int Context::isMinimized(QString wmclass)
{
    Display *display = QX11Info::display();

    if (!display)
        return 1;

    unsigned long nitems;
    Window *lists = this->xwindows(display, &nitems);

    for (int i = 0; i < nitems; i++)
    {
        if (wmclass == QString(this->xwindowClass(lists[i])))
        {
            if ((const char *)this->xwindowState(lists[i]) == "_NET_WM_STATE_HIDDEN")
            {
                XFlush(display);
                return 1;
            }
        }
    }

    XFlush(display);
    return 0;
}

void Context::manyMinimizes(QString pidname)
{
    this->xminimizeByClass(pidname);
}

int Context::screenHeight(){
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenHeight = screenGeometry.height();
    return screenHeight;
}

void Context::minimizes(int pid)
{
    Display *display = QX11Info::display();
    Window id = this->xwindowID(pid);

    if (id)
    {
        this->xminimizeByClass(QString(this->xwindowClass(id)));
    }

    XFlush(display);
}

void Context::minimize(int pid)
{
    Display *display = QX11Info::display();
    Window id = this->xwindowID(pid);

    if (id)
    {
        this->xminimize(id);
    }

    XFlush(display);
}

void Context::manyActives(QString pidname)
{
    this->xactiveByClass(pidname);
}

void Context::actives(int pid)
{
    Display *display = QX11Info::display();
    Window id = this->xwindowID(pid);

    if (id)
    {
        this->xactiveByClass(QString(this->xwindowClass(id)));
    }

    XFlush(display);
}

void Context::active(int pid)
{
    Display *display = QX11Info::display();
    Window id = this->xwindowID(pid);

    if (id)
    {
       this->xactive(id);
    }

    XFlush(display);
}

int Context::isActive(QString pidname)
{
    return this->xisActive(pidname);
}

int Context::mouseX()
{
    QPoint Mouse = QCursor::pos(this->screen);
    return Mouse.x();
}

int Context::mouseY()
{
    QPoint Mouse = QCursor::pos(this->screen);
    return Mouse.y();
}

QStringList Context::plugins()
{
    QStringList list;

    QDir dir(this->basepath + "/plugins");
    QFileInfoList filelist = dir.entryInfoList(QDir::Dirs | QDir::NoDot | QDir::NoDotAndDotDot);

    for (int i = 0; i < filelist.length(); i++)
    {
        QLocale locale;
        QSettings settings(dir.path() + "/" + filelist.at(i).fileName() + "/config", QSettings::NativeFormat);
        settings.setIniCodec("UTF-8");
        settings.beginGroup("AcessSpeed");

        QString cname = settings.value("Name[" + locale.name() + "]").toString();
        if (cname.isEmpty()) cname = settings.value("Name").toString();

        if (!cname.isEmpty())
            list << cname + ";" + settings.value("Icon").toString() + ";" + settings.value("Qml").toString() + ";" + settings.value("Lib").toString()+ ";" + settings.value("MoreSettingsss").toString();

        settings.endGroup();
    }

    return list;
}

QStringList Context::applications()
{
    QStringList list, tmp, appsPath;

    appsPath << "/usr/share/applications/" << QDir::homePath() + "/.local/share/applications/";

    foreach (QString path, appsPath)
    {
        QDir dir(path);
        QFileInfoList filelist = dir.entryInfoList(QDir::Files);

        for (int i = 0; i < filelist.length(); i++)
        {
            tmp = this->addLauncher(filelist.at(i).filePath());
            list << tmp.at(0) + ";" + tmp.at(1) + ";" + tmp.at(2) + ";" + filelist.at(i).filePath();
        }
    }

    return list;
}

void Context::dragDrop(QString iconDrag, QString app)
{
    QMimeData *mimeData = new QMimeData;
    QList<QUrl> url;
    url.append(QUrl(app));
    mimeData->setUrls(url);

    QImage img(iconDrag, iconDrag.split(".").at(1).toUtf8());
    QImage image = img.scaled(48, 48, Qt::KeepAspectRatio);
    QPixmap pixel;

    pixel.convertFromImage(image);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixel);

    QPoint p;
    p.setX(pixel.width() / 2);
    p.setY(pixel.height() / 2);
    drag->setHotSpot(p);
    drag->exec(drag->defaultAction());
}

QString Context::userName()
{
    return qgetenv("USER").toUpper();
}


QString Context::parseConfigString(QString Key, QString Value)
{
    try
    {
        auto data = toml::parse_file("config.toml");
        std::optional<std::string> out = data[Key.toUtf8().constData()][Value.toUtf8().constData()].value<std::string>();
        return QString::fromStdString(out.value());
    }
    catch (...)
    {
        std::cerr << "Parsing failed:\n";
        return "error";
    }
}

int Context::parseConfigInt(QString Key, QString Value)
{
    try
    {
        auto data = toml::parse_file("config.toml");
        return data[Key.toUtf8().constData()][Value.toUtf8().constData()].value_or(0);
    }
    catch (...)
    {
        std::cerr << "Parsing failed:\n";
        return -1;
    }
}

QStringList Context::getAppletsPanel(){
    QStringList list;
    try
    {
        std::string path = "./";
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            std::string filename = entry.path();
            if (filename.find(".toml") != std::string::npos && filename.find("config.toml") == std::string::npos){
                auto data = toml::parse_file(filename);
                std::optional<std::string> out = data["applet"]["uiPanel"].value<std::string>();
                list.append(out.value().c_str());
            }
        }
        return list;
    }
    catch (...)
    {
        std::cerr << "Parsing failed:\n";
        return list;
    }
}

QStringList Context::getAppletsIcons(){
    QStringList list;
    try
    {
        std::string path = "./";
        for (const auto & entry : std::filesystem::directory_iterator(path)) {
            std::string filename = entry.path();
            if (filename.find(".toml") != std::string::npos && filename.find("config.toml") == std::string::npos){
                auto data = toml::parse_file(filename);
                std::optional<std::string> out = data["applet"]["icon"].value<std::string>();
                list.append(out.value().c_str());
            }
        }
        return list;
    }
    catch (...)
    {
        std::cerr << "Parsing failed:\n";
        return list;
    }
}

void Context::updatePanelWidth(QWindow *window)
{
    QRect geometry = this->primaryDisplayDimensions();
    window->setWidth(geometry.width());
}

void Context::getTrayList()
{
    this->getXTrayList();
}
