#pragma once

#include <QObject>
#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "ui_menu.h"

#include "Key.h"
#include "KeyData.h"

#include "CalibController.h"
#include "MainMenuController.h"

//#include "uAudio/Scripts/Visualizer/uAudioPlayer.h"


class Menu : public QMainWindow
{
    Q_OBJECT

public:
    Menu(QWidget *parent = Q_NULLPTR);

private slots:
    void update();

private:
    Ui::MenuClass ui;
    int trainingLevel = 0;
    CalibController* calibWidget;
    int abcWordsSize = 0;
    int abcTextSize = 0;

    QMap<QString, Key> dictKey;
    QMap<QString, QList<Key>> keyFrom;

    QString prevWidget = "keyboardLayout";
    QMap<int, QPushButton*> listButton;
    QMap<QString, QString> mapKeytoName;
    QList<QString> listKey;
    QMap<QString, QString> mapNewToOldKey;
    QMap<int, QString> normalKeyboard;
    QMap<int, QString> specialKeyboard;

    int kb = 1; // kb=0: control keys; kb=1: letters keyboard; kb=2: special characters
    int caps = 0; // caps=0: lower; caps = 1: UPPER
    int lang = 0; // lang=0: vietnamese; lang=1: English
    QList<QMap<QString, QString>> dataconfig;
    QList<QString> ctrl_keys{ "Speak","Left", "Right", "BackSpace", "Lang", "one", "dot", "Return"};

    QTimer* timer;
    QMediaPlayer* player;

    bool isInDictKey = false;
    int numOfRow;
    int numOfCol;
    int numOfPos = 0;

    bool isBoosting;
};
