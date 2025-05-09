/****************************************************************************
** Meta object code from reading C++ file 'changepassword.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../changepassword.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'changepassword.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSChangePasswordENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSChangePasswordENDCLASS = QtMocHelpers::stringData(
    "ChangePassword",
    "customClose",
    "",
    "changePassword1",
    "json",
    "changePassword2",
    "on_but_deletewindow_clicked",
    "on_line_account_textChanged",
    "arg1",
    "on_line_password_textChanged",
    "on_line_newpassword_textChanged",
    "on_line_newpassword2_textChanged",
    "on_but_yes_clicked",
    "on_but_yes_2_clicked",
    "changePasswordAns1",
    "changePasswordAns2"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSChangePasswordENDCLASS_t {
    uint offsetsAndSizes[32];
    char stringdata0[15];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[5];
    char stringdata5[16];
    char stringdata6[28];
    char stringdata7[28];
    char stringdata8[5];
    char stringdata9[29];
    char stringdata10[32];
    char stringdata11[33];
    char stringdata12[19];
    char stringdata13[21];
    char stringdata14[19];
    char stringdata15[19];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSChangePasswordENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSChangePasswordENDCLASS_t qt_meta_stringdata_CLASSChangePasswordENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "ChangePassword"
        QT_MOC_LITERAL(15, 11),  // "customClose"
        QT_MOC_LITERAL(27, 0),  // ""
        QT_MOC_LITERAL(28, 15),  // "changePassword1"
        QT_MOC_LITERAL(44, 4),  // "json"
        QT_MOC_LITERAL(49, 15),  // "changePassword2"
        QT_MOC_LITERAL(65, 27),  // "on_but_deletewindow_clicked"
        QT_MOC_LITERAL(93, 27),  // "on_line_account_textChanged"
        QT_MOC_LITERAL(121, 4),  // "arg1"
        QT_MOC_LITERAL(126, 28),  // "on_line_password_textChanged"
        QT_MOC_LITERAL(155, 31),  // "on_line_newpassword_textChanged"
        QT_MOC_LITERAL(187, 32),  // "on_line_newpassword2_textChanged"
        QT_MOC_LITERAL(220, 18),  // "on_but_yes_clicked"
        QT_MOC_LITERAL(239, 20),  // "on_but_yes_2_clicked"
        QT_MOC_LITERAL(260, 18),  // "changePasswordAns1"
        QT_MOC_LITERAL(279, 18)   // "changePasswordAns2"
    },
    "ChangePassword",
    "customClose",
    "",
    "changePassword1",
    "json",
    "changePassword2",
    "on_but_deletewindow_clicked",
    "on_line_account_textChanged",
    "arg1",
    "on_line_password_textChanged",
    "on_line_newpassword_textChanged",
    "on_line_newpassword2_textChanged",
    "on_but_yes_clicked",
    "on_but_yes_2_clicked",
    "changePasswordAns1",
    "changePasswordAns2"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSChangePasswordENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   86,    2, 0x06,    1 /* Public */,
       3,    1,   87,    2, 0x06,    2 /* Public */,
       5,    1,   90,    2, 0x06,    4 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    0,   93,    2, 0x08,    6 /* Private */,
       7,    1,   94,    2, 0x08,    7 /* Private */,
       9,    1,   97,    2, 0x08,    9 /* Private */,
      10,    1,  100,    2, 0x08,   11 /* Private */,
      11,    1,  103,    2, 0x08,   13 /* Private */,
      12,    0,  106,    2, 0x08,   15 /* Private */,
      13,    0,  107,    2, 0x08,   16 /* Private */,
      14,    1,  108,    2, 0x0a,   17 /* Public */,
      15,    1,  111,    2, 0x0a,   19 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    4,
    QMetaType::Void, QMetaType::QJsonObject,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QJsonObject,    4,
    QMetaType::Void, QMetaType::QJsonObject,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject ChangePassword::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CLASSChangePasswordENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSChangePasswordENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSChangePasswordENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ChangePassword, std::true_type>,
        // method 'customClose'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changePassword1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'changePassword2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'on_but_deletewindow_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_line_account_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_line_password_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_line_newpassword_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_line_newpassword2_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_but_yes_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_but_yes_2_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changePasswordAns1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'changePasswordAns2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>
    >,
    nullptr
} };

void ChangePassword::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ChangePassword *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->customClose(); break;
        case 1: _t->changePassword1((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 2: _t->changePassword2((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 3: _t->on_but_deletewindow_clicked(); break;
        case 4: _t->on_line_account_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->on_line_password_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->on_line_newpassword_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->on_line_newpassword2_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 8: _t->on_but_yes_clicked(); break;
        case 9: _t->on_but_yes_2_clicked(); break;
        case 10: _t->changePasswordAns1((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 11: _t->changePasswordAns2((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ChangePassword::*)();
            if (_t _q_method = &ChangePassword::customClose; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ChangePassword::*)(const QJsonObject & );
            if (_t _q_method = &ChangePassword::changePassword1; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ChangePassword::*)(const QJsonObject & );
            if (_t _q_method = &ChangePassword::changePassword2; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *ChangePassword::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChangePassword::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSChangePasswordENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ChangePassword::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void ChangePassword::customClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ChangePassword::changePassword1(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ChangePassword::changePassword2(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSLineChangePassENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLineChangePassENDCLASS = QtMocHelpers::stringData(
    "LineChangePass",
    "focusInEvent",
    "",
    "QFocusEvent*",
    "event",
    "focusOutEvent"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLineChangePassENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[15];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[6];
    char stringdata5[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLineChangePassENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLineChangePassENDCLASS_t qt_meta_stringdata_CLASSLineChangePassENDCLASS = {
    {
        QT_MOC_LITERAL(0, 14),  // "LineChangePass"
        QT_MOC_LITERAL(15, 12),  // "focusInEvent"
        QT_MOC_LITERAL(28, 0),  // ""
        QT_MOC_LITERAL(29, 12),  // "QFocusEvent*"
        QT_MOC_LITERAL(42, 5),  // "event"
        QT_MOC_LITERAL(48, 13)   // "focusOutEvent"
    },
    "LineChangePass",
    "focusInEvent",
    "",
    "QFocusEvent*",
    "event",
    "focusOutEvent"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLineChangePassENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   26,    2, 0x0a,    1 /* Public */,
       5,    1,   29,    2, 0x0a,    3 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject LineChangePass::staticMetaObject = { {
    QMetaObject::SuperData::link<QLineEdit::staticMetaObject>(),
    qt_meta_stringdata_CLASSLineChangePassENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLineChangePassENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLineChangePassENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LineChangePass, std::true_type>,
        // method 'focusInEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QFocusEvent *, std::false_type>,
        // method 'focusOutEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QFocusEvent *, std::false_type>
    >,
    nullptr
} };

void LineChangePass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LineChangePass *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->focusInEvent((*reinterpret_cast< std::add_pointer_t<QFocusEvent*>>(_a[1]))); break;
        case 1: _t->focusOutEvent((*reinterpret_cast< std::add_pointer_t<QFocusEvent*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LineChangePass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineChangePass::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLineChangePassENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QLineEdit::qt_metacast(_clname);
}

int LineChangePass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
