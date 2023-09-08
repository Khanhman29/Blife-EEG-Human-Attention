#pragma once

#include <QPushButton>
#include <QImage>
#include <QString>
#include <QChar>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <QMapIterator>
#include <QFile>
#include <QGridLayout>
#include <QLineEdit>
#include <QStackedWidget>
#include <QtGuI>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QWidget>
#include <QRandomGenerator>

#include <Windows.h>
#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>

#include "KeyData.h"
#include "read.h"

extern tobii_error_t error;
extern tobii_api_t* api;
extern tobii_device_t* device;
extern int ROW;
extern int COL;

extern QString BTN_QSS;
extern QString EDIT_QSS;
extern QString CURRENT_WIDGET;
extern QString PREVIOUS_WIDGET;
extern QString DICTIONARY_LINE_TEXT;
extern int DICTIONARY_CHOOSE;
extern int DICTIONARY_CHOOSE_TYPE;
extern QMap<QString, QWidget*> LIST_WIDGET_MAP;
extern QMap<QString, QMap<int, QPushButton*>> LIST_BUTTON_MAP;
extern QMap<QString, QMap<QString, int>> WIDGET_CONFIG_SIZE;
extern QMap<QString, QMap<QString, QString>> LIST_MAP_KEY_TO_NAME;

extern QStackedWidget* stackedWidgets;

extern QMap<QString, int> LANG;
extern QMap<QString, int> CAPS;
extern QMap<QString, int> KB;

extern int TRAINING_LEVEL;

class Key
{
public:
    KeyData keyData;
    float timeCount = 0;
    QString name;
    bool isHover = true;
    static float gaze_x;
    static float gaze_y;
    QTime time;
    bool isKeyFrom = false;
    QString color = "black";

public:
    Key() {}
    
    void Update() {
        if (isHover) {

        }
    }

    static void playAudio(QString txt) {
        // save url to file 
        QFile file("./speak.txt");
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out.setCodec("UTF-8");
        QString url = "https://translate.google.com/translate_tts?ie=UTF-8&client=tw-ob&tl=vi&q=" + txt;
        out.setGenerateByteOrderMark(false);
        out << url;
        file.close();

        WinExec("python scripts.py", SW_HIDE);
       // system("python scripts.py");
    }

    static void gaze_point_callback(tobii_gaze_point_t const* gaze_point, void* user_data) {
        if (gaze_point->validity == TOBII_VALIDITY_VALID) {
            gaze_x = gaze_point->position_xy[0];
             gaze_y = gaze_point->position_xy[1];
        }
        else {
            gaze_x = 0.0;
            gaze_y = 0.0;
        }
    }

    void updateBackground(QPushButton* btn, QString font_size) {
        float scaled = timeCount / keyData.dwTime;
        if (scaled == 0) {
            `btn->setStyleSheet(QString("QPushButton{font-size: %1px; color: %2;}").arg(font_size).arg(color));
        }
        else {
            QImage img = QImage(":/background/gray_background.png").scaled(btn->size().width(), btn->size().height());
            QImage img2 = img.scaled((int) btn->size().width() * scaled, (int) btn->size().height() * scaled, Qt::IgnoreAspectRatio);
            bool check = img2.save("icon_scale.jpg", 0, -1);
            //qDebug() << check;
            btn->setStyleSheet(QString("QPushButton{font-size: %1px; color: %2; background-image:url(./icon_scale.jpg); background-position:center; background-repeat:no-repeat;}").arg(font_size).arg(color));
        }
    }

    void setColor(QPushButton* btn, QString font_size) {
        btn->setStyleSheet(QString("QPushButton{font-size: %1px; color: %2;}").arg(font_size).arg(color));
    }
};

extern QMap<QString, QMap<QString, Key>> LIST_DICT_KEY;
extern QMap<QString, QMap<QString, QList<Key>>> LIST_KEY_FROM;

