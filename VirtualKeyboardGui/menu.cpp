#include "menu.h"
#include "read.h"
#include <qstackedwidget.h>
#include <qsize.h>
#include <qsizepolicy.h>
#include "lslStreaming.h"

float Key::gaze_x = 0;
float Key::gaze_y = 0;
int ROW = 0;
int COL = 0;

int DICTIONARY_CHOOSE;
int DICTIONARY_CHOOSE_TYPE;
QString PREVIOUS_WIDGET = "";
QString DICTIONARY_LINE_TEXT = "";
QString CURRENT_WIDGET = "keyboardLayout";
QString BTN_QSS = "QPushButton{font-size: 40px;}";
QString EDIT_QSS = "QLineEdit{font-size: 40px;}";

QMap<QString, QWidget*> LIST_WIDGET_MAP;
QMap<QString, QMap<QString, Key>> LIST_DICT_KEY;
QMap<QString, QMap<int, QPushButton*>> LIST_BUTTON_MAP;
QMap<QString, QMap<QString, int>> WIDGET_CONFIG_SIZE;
QMap<QString, QMap<QString, QString>> LIST_MAP_KEY_TO_NAME;
QMap<QString, QMap<QString, QList<Key>>> LIST_KEY_FROM;

QStackedWidget* stackedWidgets;

QMap<QString, int> LANG;
QMap<QString, int> CAPS;
QMap<QString, int> KB;

int TRAINING_LEVEL = 1;

Outlet ETOutlet;
Inlet EEGInlet;
std::thread threadInitInlet(&Inlet::init, &EEGInlet);
void pullData() {

    for (;;) {
        EEGInlet.startReceive();
    }
}
std::thread threadPullData(pullData);


Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    isBoosting = false;
    CURRENT_WIDGET = "mainWidget";

    // tao widget
    QWidget* mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);
    QGridLayout* mainLayout = new QGridLayout(mainWidget);
    
    //LIST_WIDGET_MAP[CURRENT_WIDGET] = mainWidget;

    //  add to stackedwidgets
    stackedWidgets = new QStackedWidget;

    mainLayout->addWidget(stackedWidgets);
    mainWidget->setLayout(mainLayout);

    // media play
    //player = new QMediaPlayer;
    //connect(player, SIGNAL(positionrChanged(qint64)), this, SLOT(positionChanged(qint64)));

    //test layout
    PREVIOUS_WIDGET = "mainMenuLayout";

    // add main menu layout
    MainMenuController* mainMenuWidget = new MainMenuController(mainWidget);
    LIST_WIDGET_MAP["mainMenuLayout"] = mainMenuWidget;
    stackedWidgets->addWidget(mainMenuWidget);

    CURRENT_WIDGET = "mainMenuLayout";
    stackedWidgets->setCurrentWidget(mainMenuWidget);

    // signal and slot to update position of cursor
    if (error == TOBII_ERROR_NO_ERROR) {
        error = tobii_gaze_point_subscribe(device, Key::gaze_point_callback, 0);
        //if (error != TOBII_ERROR_NO_ERROR) {
        //    qDebug() << tobii_error_message(error) << "\n";
        //}

        assert(error == TOBII_ERROR_NO_ERROR);
        
        // add calib layout to stackedwidgets
        PREVIOUS_WIDGET = "mainMenuLayout";
        calibWidget = new CalibController(mainWidget);
        LIST_WIDGET_MAP["calibLayout"] = calibWidget;
        stackedWidgets->addWidget(calibWidget);
        
        stackedWidgets->setCurrentWidget(calibWidget);

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(16.6667);
    }
    else {
        //QMessageBox::information(this, "Device not found", "Bạn đang ở chế độ dùng chuột, để dùng thiết bị, vui lòng cắm thiết bị vào sau đó bật lại ứng dụng", QMessageBox::Ok);
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start(16.6667);
    }
    threadInitInlet.detach();

    threadPullData.detach();
    
    
    
}

void Menu::update() {

    int cursor_x = this->mapFromGlobal(QCursor::pos()).x();
    int cursor_y = this->mapFromGlobal(QCursor::pos()).y();
    int width = QApplication::desktop()->screenGeometry().width();
    int height = QApplication::desktop()->screenGeometry().height();
    if (error == TOBII_ERROR_NO_ERROR) {
        SetCursorPos((int)width * Key::gaze_x, (int)height * Key::gaze_y);
        ETOutlet.startSend((int)width * Key::gaze_x * 100 / width, (int)height * Key::gaze_y * 100 / height);
        // Optionally block this thread until data is available. Especially useful if running in a separate thread.
        error = tobii_wait_for_callbacks(1, &device);
        assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

        // Process callbacks on this thread if data is available
        error = tobii_device_process_callbacks(device);
        assert(error == TOBII_ERROR_NO_ERROR);
    }
    else {
        ETOutlet.startSend(cursor_x*100/width, cursor_y*100/height);
    }
    
    ROW = WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"];
    COL = WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"];

    

    

    if (cursor_x < 0 || cursor_y < 0 || cursor_x > this->size().width() || cursor_y > this->size().height()) {
        numOfPos = 99;
    }
    else {
        //convert x,y coordinates to number in position map
        numOfPos = COL * abs(((int)(cursor_y * ROW) / this->height())) + (int)(cursor_x * COL) / this->width();
    }
    
    qDebug() << numOfPos;

    listButton = LIST_BUTTON_MAP[CURRENT_WIDGET];
    dictKey = LIST_DICT_KEY[CURRENT_WIDGET];
    mapKeytoName = LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET];
    keyFrom.clear();
    if (CURRENT_WIDGET == "keyboardLayout") {
        keyFrom = LIST_KEY_FROM[CURRENT_WIDGET];
        lang = LANG[CURRENT_WIDGET];
        caps = CAPS[CURRENT_WIDGET];
        kb = KB[CURRENT_WIDGET];
    }
    else if (CURRENT_WIDGET == "typingLayout") {
        keyFrom = LIST_KEY_FROM["typingLayout"];
        // set size words button
        listButton[0]->setFixedWidth((int)(0.3925 * this->width()));
    }
    //else if (CURRENT_WIDGET == "noteAreaLayout") {
    //    keyFrom = LIST_KEY_FROM["noteAreaLayout"];
    //}
    //if (currentWidget == "keyboardLayout") {
    //    numOfPos = col * abs(((int)(cursor_y * row) / this->height())) + (int)(cursor_x * col) / this->width();
    //    dictKey = listDictKey["keyboardLayout"];
    //}

    if (listButton.contains(numOfPos)) {
        QPushButton* btn = listButton[numOfPos];
        Key key;
        // dùng trong trường hợp map text của button với key
        if (CURRENT_WIDGET == "keyboardLayout") {
            if (btn->text() == "&&") {
                key = dictKey["and"];
            } 
            else if (btn->text() == "1" && kb == 1) {
                key = dictKey["one"];
            }
            else if (btn->text() == "2" && kb == 1) {
                key = dictKey["two"];
            }
            else if (btn->text() == "3" && kb == 1) {
                key = dictKey["three"];
            }
            else {
                key = dictKey[mapKeytoName[btn->text()]];
            }
        }

        // chỉ dùng trong trường hợp cái widget map số thứ thự button với key
        else if (CURRENT_WIDGET == "typingLayout" || CURRENT_WIDGET == "smartHomeLayout" || CURRENT_WIDGET == "mainMenuLayout" || CURRENT_WIDGET == "talkLayout" || CURRENT_WIDGET == "dictionaryLayout" || CURRENT_WIDGET == "noteLayout" || CURRENT_WIDGET == "dianosLayout" || CURRENT_WIDGET == "greetingsLayout" || CURRENT_WIDGET == "supportLayout" || CURRENT_WIDGET == "qaLayout"
                || CURRENT_WIDGET == "lightLayout" || CURRENT_WIDGET == "tvLayout" || CURRENT_WIDGET == "fanLayout" || CURRENT_WIDGET == "acLayout" || CURRENT_WIDGET == "settingLayout") {
            if (btn->text() == "⌧") {
                key = dictKey["Clear"];
            }
            else {
                key = dictKey[mapKeytoName[QString::number(numOfPos)]];
            }
        }
        else {
            key = dictKey[mapKeytoName[btn->text()]];
        }

        qDebug() << btn->text();

        if ((listKey.isEmpty() || !listKey.contains(key.keyData.name)) && key.isHover == true) {
            key.time = QTime::currentTime();
            key.timeCount = 0;
            listKey.append(key.keyData.name);
            dictKey[key.keyData.name] = key;
        }
        else {
            for each (QString name in listKey)
            {
                if (!dictKey.contains(name)) {
                    listKey.removeAt(listKey.indexOf(name));
                }
                Key getKey = dictKey[name];
                if (getKey.keyData.name == key.keyData.name) {
                    float boostTime = 2;
                    getKey.timeCount += (float) 1.0* getKey.time.msecsTo(QTime::currentTime())/1000;
                    if (EEGInlet.getLastestData()) {
                        getKey.timeCount += boostTime;
                    }
                    
                    getKey.time = QTime::currentTime();
                    
                    // exTime
                    if (getKey.timeCount >= getKey.keyData.exTime && getKey.keyData.exTime != 0) {

                        // if keyboardLayout
                        if (CURRENT_WIDGET == "keyboardLayout") {
                            if (keyFrom.contains(getKey.keyData.name) && (lang == 0 || ctrl_keys.contains(getKey.keyData.name))) {

                                if (getKey.keyData.name == "Space") {
                                    // DO NOTHING
                                    //int pos = line->cursorPosition();
                                    //QString txt = line->text().mid(0, pos);
                                    //QString van;
                                    //QString shuyen = "";
                                    //QString shoi = "";
                                    //QString ssac = "";
                                    //QString snga = "";
                                    //QString snang = "";
                                }
                                else {
                                    for each (Key exKey in keyFrom[getKey.keyData.name])
                                    {
                                        if (listKey.contains(exKey.keyData.name)) {
                                            break;
                                        }
                                        QPushButton* exBtn = listButton[exKey.keyData.pos];
                                        // map new key to old key
                                        mapNewToOldKey[exKey.keyData.key] = exBtn->text();
                                
                                        exBtn->setText(exKey.keyData.key);
                                        listKey.append(exKey.keyData.name);
                                        exKey.time = QTime::currentTime();
                                        exKey.timeCount = getKey.keyData.dwTime - getKey.keyData.exTime;
                                        exKey.updateBackground(exBtn, "40");
                                
                                        dictKey[exKey.keyData.name] = exKey;
                                    }
                                }
                            }
                        }

                        // if typingLayout
                        if (CURRENT_WIDGET == "typingLayout") {
                            for each (Key exKey in keyFrom[getKey.keyData.name])
                            {
                                if (listKey.contains(exKey.keyData.name)) {
                                    break;
                                }
                                QPushButton* exBtn = listButton[exKey.keyData.pos];
                                // map new key to old key
                                mapNewToOldKey["Clear"] = exBtn->text();

                                exBtn->setText(exKey.keyData.key);
                                listKey.append(exKey.keyData.name);
                                exKey.time = QTime::currentTime();
                                exKey.timeCount = exKey.keyData.dwTime - exKey.keyData.exTime;
                                exKey.updateBackground(exBtn, "40");

                                dictKey[exKey.keyData.name] = exKey;
                            }
                        }
                    }

                    if (getKey.timeCount >= getKey.keyData.dwTime) {
                        getKey.timeCount = 0;
                        getKey.color = "black";
                        listKey.removeAt(listKey.indexOf(getKey.keyData.name));
                        btn->clicked();
                    }
                    getKey.updateBackground(btn, "40");
                    dictKey[getKey.keyData.name] = getKey;
                }
                else {
                    getKey.timeCount -= (float) 1.0*getKey.time.msecsTo(QTime::currentTime()) / 1000;
                    getKey.time = QTime::currentTime();
                    if (getKey.timeCount <= 0) {
                        getKey.timeCount = 0;
                        if (getKey.keyData.exFrom != "") {
                            QPushButton* exBtn = listButton[getKey.keyData.pos];
                            if (CURRENT_WIDGET == "typingLayout") {
                                exBtn->setText(mapNewToOldKey["Clear"]);
                            }
                        }
                        listKey.removeAt(listKey.indexOf(getKey.keyData.name));
                    }
                    QPushButton* btn_over = listButton[getKey.keyData.pos];
                    getKey.updateBackground(btn_over, "40");
                    dictKey[getKey.keyData.name] = getKey;
                }
            }
        }
        if (CURRENT_WIDGET != prevWidget) {
            prevWidget = CURRENT_WIDGET;
            listKey.clear();
            mapNewToOldKey.clear();

        }
        else if (CURRENT_WIDGET == "trainingLayout" && trainingLevel != TRAINING_LEVEL) {
            listKey.clear();
            mapNewToOldKey.clear();
            trainingLevel = TRAINING_LEVEL;
        }
        else {
            LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
            if (CURRENT_WIDGET == "keyboardLayout") {
                LIST_KEY_FROM["keyboardLayout"] = keyFrom;
            }
        }
    }
}

