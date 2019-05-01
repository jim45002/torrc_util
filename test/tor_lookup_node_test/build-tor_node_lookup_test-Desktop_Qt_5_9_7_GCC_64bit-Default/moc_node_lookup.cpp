/****************************************************************************
** Meta object code from reading C++ file 'node_lookup.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../node_lookup.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'node_lookup.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_node_lookup_t {
    QByteArrayData data[7];
    char stringdata0[89];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_node_lookup_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_node_lookup_t qt_meta_stringdata_node_lookup = {
    {
QT_MOC_LITERAL(0, 0, 11), // "node_lookup"
QT_MOC_LITERAL(1, 12, 11), // "onReadyRead"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 13), // "get_node_list"
QT_MOC_LITERAL(4, 39, 13), // "country_abbrv"
QT_MOC_LITERAL(5, 53, 17), // "make_country_file"
QT_MOC_LITERAL(6, 71, 17) // "download_nodelist"

    },
    "node_lookup\0onReadyRead\0\0get_node_list\0"
    "country_abbrv\0make_country_file\0"
    "download_nodelist"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_node_lookup[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x0a /* Public */,
       3,    2,   30,    2, 0x0a /* Public */,
       6,    0,   35,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,    4,    5,
    QMetaType::Void,

       0        // eod
};

void node_lookup::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        node_lookup *_t = static_cast<node_lookup *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onReadyRead(); break;
        case 1: _t->get_node_list((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->download_nodelist(); break;
        default: ;
        }
    }
}

const QMetaObject node_lookup::staticMetaObject = {
    { &node_lookup_interface::staticMetaObject, qt_meta_stringdata_node_lookup.data,
      qt_meta_data_node_lookup,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *node_lookup::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *node_lookup::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_node_lookup.stringdata0))
        return static_cast<void*>(this);
    return node_lookup_interface::qt_metacast(_clname);
}

int node_lookup::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = node_lookup_interface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
