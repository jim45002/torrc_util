/****************************************************************************
** Meta object code from reading C++ file 'test_node_lookup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../tor_node_lookup_test/test_node_lookup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'test_node_lookup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_test_node_lookup_t {
    QByteArrayData data[6];
    char stringdata0[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_test_node_lookup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_test_node_lookup_t qt_meta_stringdata_test_node_lookup = {
    {
QT_MOC_LITERAL(0, 0, 16), // "test_node_lookup"
QT_MOC_LITERAL(1, 17, 17), // "request_node_list"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 33), // "request_download_latest_node_..."
QT_MOC_LITERAL(4, 70, 14), // "recv_node_list"
QT_MOC_LITERAL(5, 85, 20) // "recv_download_result"

    },
    "test_node_lookup\0request_node_list\0\0"
    "request_download_latest_node_list\0"
    "recv_node_list\0recv_download_result"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_test_node_lookup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       3,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   40,    2, 0x0a /* Public */,
       5,    1,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    2,    2,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    2,    2,
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void test_node_lookup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        test_node_lookup *_t = static_cast<test_node_lookup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->request_node_list((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 1: _t->request_download_latest_node_list(); break;
        case 2: _t->recv_node_list((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 3: _t->recv_download_result((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (test_node_lookup::*_t)(QString , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&test_node_lookup::request_node_list)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (test_node_lookup::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&test_node_lookup::request_download_latest_node_list)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject test_node_lookup::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_test_node_lookup.data,
      qt_meta_data_test_node_lookup,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *test_node_lookup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *test_node_lookup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_test_node_lookup.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int test_node_lookup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void test_node_lookup::request_node_list(QString _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void test_node_lookup::request_download_latest_node_list()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
