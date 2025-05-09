/****************************************************************************
** Meta object code from reading C++ file 'login.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../login.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'login.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_CLASSLoginENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLoginENDCLASS = QtMocHelpers::stringData(
    "Login",
    "sendAvator",
    "",
    "zhuCeShiBai",
    "zhuCeChengGong",
    "qqnum",
    "findPass1",
    "jsonObj",
    "findPass2",
    "findPass3",
    "loginSucceed",
    "account",
    "onReadyRead",
    "on_gologin_clicked",
    "sendLogin",
    "updateLoginText",
    "on_line_password_returnPressed",
    "on_line_qqnum_returnPressed",
    "mousePressEvent",
    "QMouseEvent*",
    "event",
    "on_line_password_textChanged",
    "arg1",
    "on_line_qqnum_textChanged",
    "on_auto_ckbox_toggled",
    "checked",
    "on_ckbox_remeber_toggled",
    "setSty",
    "on_rgs_pbt_clicked",
    "on_fgt_pbt_clicked",
    "updateGradient",
    "on_but_deletewindow_clicked"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLoginENDCLASS_t {
    uint offsetsAndSizes[64];
    char stringdata0[6];
    char stringdata1[11];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[15];
    char stringdata5[6];
    char stringdata6[10];
    char stringdata7[8];
    char stringdata8[10];
    char stringdata9[10];
    char stringdata10[13];
    char stringdata11[8];
    char stringdata12[12];
    char stringdata13[19];
    char stringdata14[10];
    char stringdata15[16];
    char stringdata16[31];
    char stringdata17[28];
    char stringdata18[16];
    char stringdata19[13];
    char stringdata20[6];
    char stringdata21[29];
    char stringdata22[5];
    char stringdata23[26];
    char stringdata24[22];
    char stringdata25[8];
    char stringdata26[25];
    char stringdata27[7];
    char stringdata28[19];
    char stringdata29[19];
    char stringdata30[15];
    char stringdata31[28];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLoginENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLoginENDCLASS_t qt_meta_stringdata_CLASSLoginENDCLASS = {
    {
        QT_MOC_LITERAL(0, 5),  // "Login"
        QT_MOC_LITERAL(6, 10),  // "sendAvator"
        QT_MOC_LITERAL(17, 0),  // ""
        QT_MOC_LITERAL(18, 11),  // "zhuCeShiBai"
        QT_MOC_LITERAL(30, 14),  // "zhuCeChengGong"
        QT_MOC_LITERAL(45, 5),  // "qqnum"
        QT_MOC_LITERAL(51, 9),  // "findPass1"
        QT_MOC_LITERAL(61, 7),  // "jsonObj"
        QT_MOC_LITERAL(69, 9),  // "findPass2"
        QT_MOC_LITERAL(79, 9),  // "findPass3"
        QT_MOC_LITERAL(89, 12),  // "loginSucceed"
        QT_MOC_LITERAL(102, 7),  // "account"
        QT_MOC_LITERAL(110, 11),  // "onReadyRead"
        QT_MOC_LITERAL(122, 18),  // "on_gologin_clicked"
        QT_MOC_LITERAL(141, 9),  // "sendLogin"
        QT_MOC_LITERAL(151, 15),  // "updateLoginText"
        QT_MOC_LITERAL(167, 30),  // "on_line_password_returnPressed"
        QT_MOC_LITERAL(198, 27),  // "on_line_qqnum_returnPressed"
        QT_MOC_LITERAL(226, 15),  // "mousePressEvent"
        QT_MOC_LITERAL(242, 12),  // "QMouseEvent*"
        QT_MOC_LITERAL(255, 5),  // "event"
        QT_MOC_LITERAL(261, 28),  // "on_line_password_textChanged"
        QT_MOC_LITERAL(290, 4),  // "arg1"
        QT_MOC_LITERAL(295, 25),  // "on_line_qqnum_textChanged"
        QT_MOC_LITERAL(321, 21),  // "on_auto_ckbox_toggled"
        QT_MOC_LITERAL(343, 7),  // "checked"
        QT_MOC_LITERAL(351, 24),  // "on_ckbox_remeber_toggled"
        QT_MOC_LITERAL(376, 6),  // "setSty"
        QT_MOC_LITERAL(383, 18),  // "on_rgs_pbt_clicked"
        QT_MOC_LITERAL(402, 18),  // "on_fgt_pbt_clicked"
        QT_MOC_LITERAL(421, 14),  // "updateGradient"
        QT_MOC_LITERAL(436, 27)   // "on_but_deletewindow_clicked"
    },
    "Login",
    "sendAvator",
    "",
    "zhuCeShiBai",
    "zhuCeChengGong",
    "qqnum",
    "findPass1",
    "jsonObj",
    "findPass2",
    "findPass3",
    "loginSucceed",
    "account",
    "onReadyRead",
    "on_gologin_clicked",
    "sendLogin",
    "updateLoginText",
    "on_line_password_returnPressed",
    "on_line_qqnum_returnPressed",
    "mousePressEvent",
    "QMouseEvent*",
    "event",
    "on_line_password_textChanged",
    "arg1",
    "on_line_qqnum_textChanged",
    "on_auto_ckbox_toggled",
    "checked",
    "on_ckbox_remeber_toggled",
    "setSty",
    "on_rgs_pbt_clicked",
    "on_fgt_pbt_clicked",
    "updateGradient",
    "on_but_deletewindow_clicked"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLoginENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  152,    2, 0x06,    1 /* Public */,
       3,    0,  153,    2, 0x06,    2 /* Public */,
       4,    1,  154,    2, 0x06,    3 /* Public */,
       6,    1,  157,    2, 0x06,    5 /* Public */,
       8,    1,  160,    2, 0x06,    7 /* Public */,
       9,    1,  163,    2, 0x06,    9 /* Public */,
      10,    1,  166,    2, 0x06,   11 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    0,  169,    2, 0x08,   13 /* Private */,
      13,    0,  170,    2, 0x08,   14 /* Private */,
      14,    0,  171,    2, 0x08,   15 /* Private */,
      15,    0,  172,    2, 0x08,   16 /* Private */,
      16,    0,  173,    2, 0x08,   17 /* Private */,
      17,    0,  174,    2, 0x08,   18 /* Private */,
      18,    1,  175,    2, 0x08,   19 /* Private */,
      21,    1,  178,    2, 0x08,   21 /* Private */,
      23,    1,  181,    2, 0x08,   23 /* Private */,
      24,    1,  184,    2, 0x08,   25 /* Private */,
      26,    1,  187,    2, 0x08,   27 /* Private */,
      27,    0,  190,    2, 0x08,   29 /* Private */,
      28,    0,  191,    2, 0x08,   30 /* Private */,
      29,    0,  192,    2, 0x08,   31 /* Private */,
      30,    0,  193,    2, 0x08,   32 /* Private */,
      31,    0,  194,    2, 0x08,   33 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QJsonObject,    7,
    QMetaType::Void, QMetaType::QJsonObject,    7,
    QMetaType::Void, QMetaType::QJsonObject,    7,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::Bool,   25,
    QMetaType::Void, QMetaType::Bool,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject Login::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSLoginENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLoginENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLoginENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Login, std::true_type>,
        // method 'sendAvator'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zhuCeShiBai'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zhuCeChengGong'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString, std::false_type>,
        // method 'findPass1'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'findPass2'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'findPass3'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'loginSucceed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString, std::false_type>,
        // method 'onReadyRead'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_gologin_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sendLogin'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateLoginText'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_line_password_returnPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_line_qqnum_returnPressed'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'mousePressEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QMouseEvent *, std::false_type>,
        // method 'on_line_password_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_line_qqnum_textChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'on_auto_ckbox_toggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'on_ckbox_remeber_toggled'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setSty'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_rgs_pbt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_fgt_pbt_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'updateGradient'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_but_deletewindow_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void Login::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Login *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->sendAvator(); break;
        case 1: _t->zhuCeShiBai(); break;
        case 2: _t->zhuCeChengGong((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->findPass1((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 4: _t->findPass2((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 5: _t->findPass3((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 6: _t->loginSucceed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->onReadyRead(); break;
        case 8: _t->on_gologin_clicked(); break;
        case 9: _t->sendLogin(); break;
        case 10: _t->updateLoginText(); break;
        case 11: _t->on_line_password_returnPressed(); break;
        case 12: _t->on_line_qqnum_returnPressed(); break;
        case 13: _t->mousePressEvent((*reinterpret_cast< std::add_pointer_t<QMouseEvent*>>(_a[1]))); break;
        case 14: _t->on_line_password_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 15: _t->on_line_qqnum_textChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 16: _t->on_auto_ckbox_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 17: _t->on_ckbox_remeber_toggled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 18: _t->setSty(); break;
        case 19: _t->on_rgs_pbt_clicked(); break;
        case 20: _t->on_fgt_pbt_clicked(); break;
        case 21: _t->updateGradient(); break;
        case 22: _t->on_but_deletewindow_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Login::*)();
            if (_t _q_method = &Login::sendAvator; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Login::*)();
            if (_t _q_method = &Login::zhuCeShiBai; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Login::*)(QString );
            if (_t _q_method = &Login::zhuCeChengGong; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Login::*)(const QJsonObject & );
            if (_t _q_method = &Login::findPass1; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (Login::*)(const QJsonObject & );
            if (_t _q_method = &Login::findPass2; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (Login::*)(const QJsonObject & );
            if (_t _q_method = &Login::findPass3; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (Login::*)(const QString );
            if (_t _q_method = &Login::loginSucceed; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject *Login::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Login::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLoginENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int Login::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void Login::sendAvator()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Login::zhuCeShiBai()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Login::zhuCeChengGong(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Login::findPass1(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Login::findPass2(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Login::findPass3(const QJsonObject & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Login::loginSucceed(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSLineInputENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSLineInputENDCLASS = QtMocHelpers::stringData(
    "LineInput",
    "focusInEvent",
    "",
    "QFocusEvent*",
    "event",
    "focusOutEvent"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSLineInputENDCLASS_t {
    uint offsetsAndSizes[12];
    char stringdata0[10];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[6];
    char stringdata5[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSLineInputENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSLineInputENDCLASS_t qt_meta_stringdata_CLASSLineInputENDCLASS = {
    {
        QT_MOC_LITERAL(0, 9),  // "LineInput"
        QT_MOC_LITERAL(10, 12),  // "focusInEvent"
        QT_MOC_LITERAL(23, 0),  // ""
        QT_MOC_LITERAL(24, 12),  // "QFocusEvent*"
        QT_MOC_LITERAL(37, 5),  // "event"
        QT_MOC_LITERAL(43, 13)   // "focusOutEvent"
    },
    "LineInput",
    "focusInEvent",
    "",
    "QFocusEvent*",
    "event",
    "focusOutEvent"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSLineInputENDCLASS[] = {

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

Q_CONSTINIT const QMetaObject LineInput::staticMetaObject = { {
    QMetaObject::SuperData::link<QLineEdit::staticMetaObject>(),
    qt_meta_stringdata_CLASSLineInputENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSLineInputENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSLineInputENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<LineInput, std::true_type>,
        // method 'focusInEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QFocusEvent *, std::false_type>,
        // method 'focusOutEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QFocusEvent *, std::false_type>
    >,
    nullptr
} };

void LineInput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LineInput *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->focusInEvent((*reinterpret_cast< std::add_pointer_t<QFocusEvent*>>(_a[1]))); break;
        case 1: _t->focusOutEvent((*reinterpret_cast< std::add_pointer_t<QFocusEvent*>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *LineInput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineInput::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSLineInputENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QLineEdit::qt_metacast(_clname);
}

int LineInput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
