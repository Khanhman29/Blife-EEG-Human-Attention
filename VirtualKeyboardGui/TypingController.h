#pragma once

#include <QWidget>
#include "Key.h"

class TypingController : public QWidget
{
	Q_OBJECT

public:
	TypingController(QWidget *parent);
	~TypingController();

private slots:
    void homeClick();
    void prevClick();
    void nextClick();
    void wordsClick();
    void spaceClick();
    void deleteClick();
    void clearClick();
    void txtClick();

private:
    void addHistory(QString, QString, QList<QMap<QString, QString>>);
    void saveHistorytoFile(QList<QMap<QString, QString>>);
    QList<QMap<QString, QString>> getTalkHistory();
    void loadAudio(QString txt);


private:
    QString makeKhongDau(QString);
    QList<QString> createDau(QString);
    void readVan();
    void readDau();
    void displayTXTKeys();
    void filterBoard();
    QList<QMap<QString, QString>> talkHistory;

private:
    QGridLayout* typingLayout;
    QMap<QString, QPushButton*> listKeyButton; // map key name to button
    QMap<int, QPushButton*> listButton; // map pos to button
    QMap<QString, QString> mapKeyToName; // map pos of button to key name
    QMap<QString, Key> dictKey;

	QList<Key> keys;
	QMap<QString, QList<Key>> keyFrom; // keyFrom
	Key words, space, del, next;
	QList<QString> all{ "b","c","d","đ","t","g","h","l","m","n","s","qu","r","v","k","p","x","u","ư","a","ă","â","y","o","ô","ơ","i","e","ê" };
    QMap<QString, QString> khongdau{
        {"ả","a1"}, {"á","a2"}, {"à","a3"}, {"ạ","a4"}, {"ã","a5"},
        {"ẳ","ă1"}, {"ắ","ă2"}, {"ằ","ă3"}, {"ặ","ă4"}, {"ẵ","ă5"},
        {"ẩ","â1"}, {"ấ","â2"}, {"ầ","â3"}, {"ậ","â4"}, {"ẫ","â5"},
        {"ỏ","o1"}, {"ó","o2"}, {"ò","o3"}, {"ọ","o4"}, {"õ","o5"},
        {"ổ","ô1"}, {"ố","ô2"}, {"ồ","ô3"}, {"ộ","ô4"}, {"ỗ","ô5"},
        {"ở","ơ1"}, {"ớ","ơ2"}, {"ờ","ơ3"}, {"ợ","ơ4"}, {"ỡ","ơ5"},
        {"ẻ","e1"}, {"é","e2"}, {"è","e3"}, {"ẹ","e4"}, {"ẽ","e5"},
        {"ể","ê1"}, {"ế","ê2"}, {"ề","ê3"}, {"ệ","ê4"}, {"ễ","ê5"},
        {"ủ","u1"}, {"ú","u2"}, {"ù","u3"}, {"ụ","u4"}, {"ũ","u5"},
        {"ử","ư1"}, {"ứ","ư2"}, {"ừ","ư3"}, {"ự","ư4"}, {"ữ","ư5"},
        {"ỷ","y1"}, {"ý","y2"}, {"ỳ","y3"}, {"ỵ","y4"}, {"ỹ","y5"},
        {"ỉ","i1"}, {"í","i2"}, {"ì","i3"}, {"ị","i4"}, {"ĩ","i5"}
    };
    QList<QString> van;
    QMap<QString, QString> bangdau;
    QList<QString> board;
    int board_start = 0;
    bool isTraining = false;
    bool isTrainingFinished = false;
    QList<QString> trainingKey;
    int currKey = 0;
    QString trainingStr;
    Key keyPrefab;
};
