/****************************************************************************
** Meta object code from reading C++ file 'threads.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.15)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "threads.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'threads.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.15. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Threads_t {
    QByteArrayData data[10];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Threads_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Threads_t qt_meta_stringdata_Threads = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Threads"
QT_MOC_LITERAL(1, 8, 8), // "onCreate"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 9), // "onDestroy"
QT_MOC_LITERAL(4, 28, 15), // "onDesktopWindow"
QT_MOC_LITERAL(5, 44, 4), // "nome"
QT_MOC_LITERAL(6, 49, 7), // "wmclass"
QT_MOC_LITERAL(7, 57, 5), // "winId"
QT_MOC_LITERAL(8, 63, 4), // "size"
QT_MOC_LITERAL(9, 68, 14) // "onClearWindows"

    },
    "Threads\0onCreate\0\0onDestroy\0onDesktopWindow\0"
    "nome\0wmclass\0winId\0size\0onClearWindows"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Threads[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    4,   36,    2, 0x06 /* Public */,
       9,    0,   45,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,    5,    6,    7,    8,
    QMetaType::Void,

       0        // eod
};

void Threads::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Threads *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onCreate(); break;
        case 1: _t->onDestroy(); break;
        case 2: _t->onDesktopWindow((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 3: _t->onClearWindows(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Threads::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Threads::onCreate)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Threads::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Threads::onDestroy)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Threads::*)(QString , QString , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Threads::onDesktopWindow)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Threads::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Threads::onClearWindows)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Threads::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_Threads.data,
    qt_meta_data_Threads,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Threads::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Threads::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Threads.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int Threads::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Threads::onCreate()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Threads::onDestroy()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Threads::onDesktopWindow(QString _t1, QString _t2, int _t3, int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Threads::onClearWindows()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
