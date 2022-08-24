#pragma once

#include <QWidget>
#include "Key.h"
#include "KeyData.h"

class KeyboardController : public QWidget
{
	Q_OBJECT

public:
	KeyboardController(QWidget *parent);
	~KeyboardController();

private slots:
	void on_clicked();
   

private:
    void addHistory(QString, QString, QList<QMap<QString, QString>>);
    void saveHistorytoFile(QList<QMap<QString, QString>>);
    QList<QMap<QString, QString>> getTalkHistory();

private:
    void addHistoryDict(QString, QString, QList<QMap<QString, QString>>);
    void saveHistoryDicttoFile(QList<QMap<QString, QString>>);
    void editTXTHistory(QString, QString, int, QList<QMap<QString, QString>>);
    QList<QMap<QString, QString>> getDictHistory();
private:
	QStackedWidget* stackedwidgets;
	QGridLayout* keyboardLayout;
	QLineEdit* lineEdit;

	QMap<QString, Key> dictKey;
	QMap<QString, QList<Key>> keyFrom;

	QMap<int, QPushButton*> listButton;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
	QMap<QString, QString> mapNewToOldKey;
	QMap<int, QString> normalKeyboard;
	QMap<int, QString> specialKeyboard;

    int kb = 1; // kb=0: control keys; kb=1: letters keyboard; kb=2: special characters
    int caps = 0; // caps=0: lower; caps = 1: UPPER
    int lang = 0; // lang=0: vietnamese; lang=1: English
    QList<QString> ctrl_keys{ "Speak","Left", "Right", "BackSpace", "Lang", "one", "dot", "Return" };
    QMap<QString, QString> bangdau;
    QMap<QChar, QString> khongdau{
        {'ả',"a1"}, {'á',"a2"}, {'à',"a3"}, {'ạ',"a4"}, {'ã',"a5"},
        {'ẳ',"ă1"}, {'ắ',"ă2"}, {'ằ',"ă3"}, {'ặ',"ă4"}, {'ẵ',"ă5"},
        {'ẩ',"â1"}, {'ấ',"â2"}, {'ầ',"â3"}, {'ậ',"â4"}, {'ẫ',"â5"},
        {'ỏ',"o1"}, {'ó',"o2"}, {'ò',"o3"}, {'ọ',"o4"}, {'õ',"o5"},
        {'ổ',"ô1"}, {'ố',"ô2"}, {'ồ',"ô3"}, {'ộ',"ô4"}, {'ỗ',"ô5"},
        {'ở',"ơ1"}, {'ớ',"ơ2"}, {'ờ',"ơ3"}, {'ợ',"ơ4"}, {'ỡ',"ơ5"},
        {'ẻ',"e1"}, {'é',"e2"}, {'è',"e3"}, {'ẹ',"e4"}, {'ẽ',"e5"},
        {'ể',"ê1"}, {'ế',"ê2"}, {'ề',"ê3"}, {'ệ',"ê4"}, {'ễ',"ê5"},
        {'ủ',"u1"}, {'ú',"u2"}, {'ù',"u3"}, {'ụ',"u4"}, {'ũ',"u5"},
        {'ử',"ư1"}, {'ứ',"ư2"}, {'ừ',"ư3"}, {'ự',"ư4"}, {'ữ',"ư5"},
        {'ỷ',"y1"}, {'ý',"y2"}, {'ỳ',"y3"}, {'ỵ',"y4"}, {'ỹ',"y5"},
        {'ỉ',"i1"}, {'í',"i2"}, {'ì',"i3"}, {'ị',"i4"}, {'ĩ',"i5"}
    };
};
