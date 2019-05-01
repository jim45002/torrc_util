/****************************************************************************
** Meta object code from reading C++ file 'map_paint_layer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../map_paint_layer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'map_paint_layer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_map_paint_layer_t {
    QByteArrayData data[12];
    char stringdata0[276];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_map_paint_layer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_map_paint_layer_t qt_meta_stringdata_map_paint_layer = {
    {
QT_MOC_LITERAL(0, 0, 15), // "map_paint_layer"
QT_MOC_LITERAL(1, 16, 28), // "on_scribble_enable_triggered"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 7), // "checked"
QT_MOC_LITERAL(4, 54, 26), // "on_scribble_save_triggered"
QT_MOC_LITERAL(5, 81, 27), // "on_scribble_color_triggered"
QT_MOC_LITERAL(6, 109, 26), // "on_scribble_size_triggered"
QT_MOC_LITERAL(7, 136, 27), // "on_scribble_erase_triggered"
QT_MOC_LITERAL(8, 164, 29), // "on_scribble_history_triggered"
QT_MOC_LITERAL(9, 194, 27), // "on_scribble_clear_triggered"
QT_MOC_LITERAL(10, 222, 26), // "on_scribble_text_triggered"
QT_MOC_LITERAL(11, 249, 26) // "on_scribble_font_triggered"

    },
    "map_paint_layer\0on_scribble_enable_triggered\0"
    "\0checked\0on_scribble_save_triggered\0"
    "on_scribble_color_triggered\0"
    "on_scribble_size_triggered\0"
    "on_scribble_erase_triggered\0"
    "on_scribble_history_triggered\0"
    "on_scribble_clear_triggered\0"
    "on_scribble_text_triggered\0"
    "on_scribble_font_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_map_paint_layer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x0a /* Public */,
       4,    1,   62,    2, 0x0a /* Public */,
       5,    1,   65,    2, 0x0a /* Public */,
       6,    1,   68,    2, 0x0a /* Public */,
       7,    1,   71,    2, 0x0a /* Public */,
       8,    1,   74,    2, 0x0a /* Public */,
       9,    1,   77,    2, 0x0a /* Public */,
      10,    1,   80,    2, 0x0a /* Public */,
      11,    1,   83,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void map_paint_layer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        map_paint_layer *_t = static_cast<map_paint_layer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_scribble_enable_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_scribble_save_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_scribble_color_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_scribble_size_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_scribble_erase_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_scribble_history_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_scribble_clear_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_scribble_text_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->on_scribble_font_triggered((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject map_paint_layer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_map_paint_layer.data,
      qt_meta_data_map_paint_layer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *map_paint_layer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *map_paint_layer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_map_paint_layer.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int map_paint_layer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
