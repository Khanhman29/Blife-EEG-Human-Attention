#include "KeyboardController.h"
#include "TalkController.h"
#include "DictionaryController.h"
#include "read.h"
#include "qdatetime.h"

KeyboardController::KeyboardController(QWidget *parent)
	: QWidget(parent)
{
	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* keyboardWidget = new QWidget;
	mainLayout->addWidget(keyboardWidget);

	keyboardLayout = new QGridLayout(keyboardWidget);
	keyboardWidget->setLayout(keyboardLayout);

    CURRENT_WIDGET = "keyboardLayout";

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("KeyboardConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data = file.readAll();
		dataconfig = read::Read(data);
		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	// read bangdau config
	QList<QMap<QString, QString>> dataconfigdau;
	QFile filedau("bangdau.csv");
	if (filedau.open(QIODevice::ReadOnly)) {
		QString datadau = filedau.readAll();
		dataconfigdau = read::Read(datadau);
		QList<QString> surfix{ "r","s","f","j","x","z","w","a","e","o" };
		for each (QMap<QString, QString> row in dataconfigdau)
		{
			QString prefix = row["prefix"];
			for (int s = 0; s < surfix.count(); s++) {
				QString key = prefix + surfix[s];
				bangdau[key] = row[surfix[s]];
			}
		}
	}

	lineEdit = new QLineEdit();
	lineEdit->setPlaceholderText("Enter text...");
	lineEdit->setFocus();
	lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	lineEdit->setStyleSheet(EDIT_QSS);
    if (DICTIONARY_CHOOSE_TYPE == 1 || DICTIONARY_CHOOSE_TYPE == 2) {
        lineEdit->setText(DICTIONARY_LINE_TEXT);
    }

	for (int i = 0; i < dataconfig.count(); i++) {
		KeyData keydata(dataconfig[i]);
		if (keydata.keyType == 2) {
			int pos = keydata.pos;
			if (pos < COL) {
				keyboardLayout->addWidget(lineEdit, 0, pos, keydata.spanY, keydata.spanX);
			}
			else {
				int y = pos / COL;
				int x = pos - (COL * y);
				keyboardLayout->addWidget(lineEdit, y, x, keydata.spanY, keydata.spanX);
			}
		}
		// KB < 2
		else if (keydata.keyType == 1 && keydata.KB < 2) {
			Key key;
			key.keyData = keydata;

			// main key
			if (key.keyData.exFrom == "") {
				QPushButton* button = new QPushButton();
				button->setText(keydata.key);
				button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
				button->setStyleSheet(BTN_QSS);
				connect(button, SIGNAL(clicked()), this, SLOT(on_clicked()));

				int y = keydata.pos / COL;
				int x = keydata.pos - (COL * y);
				keyboardLayout->addWidget(button, y, x, keydata.spanY, keydata.spanX);

				for (int exPos = 0; exPos < keydata.spanX; exPos++) {
					listButton[keydata.pos + exPos] = button;
				}

				dictKey[key.keyData.name] = key;
				mapKeyToName[key.keyData.key] = key.keyData.name;
				normalKeyboard[key.keyData.pos] = key.keyData.key;

			}
			// exFrom key
			else {
				if (keyFrom.contains(key.keyData.exFrom)) {
					key.isKeyFrom = true;
					keyFrom[key.keyData.exFrom].append(key);
					dictKey[key.keyData.name] = key;
					mapKeyToName[key.keyData.key] = key.keyData.name;
				}
				else {
					key.isKeyFrom = true;
					keyFrom[key.keyData.exFrom].append(key);
					dictKey[key.keyData.name] = key;
					mapKeyToName[key.keyData.key] = key.keyData.name;
				}
			}
		}
		// KB = 2
		else {
			Key key;
			key.keyData = keydata;
			if (key.keyData.exFrom == "") {
				dictKey[key.keyData.name] = key;
				mapKeyToName[key.keyData.key] = key.keyData.name;
				specialKeyboard[key.keyData.pos] = key.keyData.key;
			}
			else {
				if (keyFrom.contains(key.keyData.exFrom)) {
					key.isKeyFrom = true;
					keyFrom[key.keyData.exFrom].append(key);
					dictKey[key.keyData.name] = key;
					mapKeyToName[key.keyData.key] = key.keyData.name;
				}
				else {
					key.isKeyFrom = true;
					keyFrom[key.keyData.exFrom].append(key);
					dictKey[key.keyData.name] = key;
					mapKeyToName[key.keyData.key] = key.keyData.name;
				}
			}
		}
	}

    LANG["keyboardLayout"] = lang;
    CAPS["keyboardLayout"] = caps;
    KB["keyboardLayout"] = kb;

	LIST_BUTTON_MAP["keyboardLayout"] = listButton;
	LIST_MAP_KEY_TO_NAME["keyboardLayout"] = mapKeyToName;
	LIST_DICT_KEY["keyboardLayout"] = dictKey;
    LIST_KEY_FROM["keyboardLayout"] = keyFrom;

	for (int c = 0; c < keyboardLayout->columnCount(); c++) {
		keyboardLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < keyboardLayout->rowCount(); r++) {
		keyboardLayout->setRowStretch(r, 1);
	}
}

KeyboardController::~KeyboardController()
{
}


QList<QMap<QString, QString>> KeyboardController::getDictHistory()
{
    QList<QMap<QString, QString>> dictionaryHistory;
    QFile fileHistory("dictionaryHistory.csv");
    if (fileHistory.open(QIODevice::ReadOnly)) {
        QString data;
        data = fileHistory.readAll();
        dictionaryHistory = read::ReadHistory(data);
    }
    return dictionaryHistory;
}
void KeyboardController::addHistoryDict(QString Acronym, QString Text, QList<QMap<QString, QString>> dictionaryHistory)
{
    QMap<QString, QString> entry;
    entry["Acronym"] = Acronym;
    entry["Text"] = Text;
    dictionaryHistory.append(entry);
    saveHistoryDicttoFile(dictionaryHistory);
}
void KeyboardController::saveHistoryDicttoFile(QList<QMap<QString, QString>> dictionaryHistory)
{
    QFile file("dictionaryHistory.csv");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-8"); // ...
    out.setGenerateByteOrderMark(false);
    out << "Acronym" << "," << "Text" << "\n";
    for (int i = 0; i < dictionaryHistory.length(); i++) {
        QString text = dictionaryHistory[i]["Acronym"] + "," + dictionaryHistory[i]["Text"] + "\n";
        out << text;
    }
    file.close();
}

void KeyboardController::editTXTHistory(QString Acronym,QString Text, int pos, QList<QMap<QString, QString>> dictionaryHistory)
{
    QMap<QString, QString> entry;
    entry["Acronym"] = Acronym;
    entry["Text"] = Text;
    dictionaryHistory.insert(pos, entry);
    saveHistoryDicttoFile(dictionaryHistory);
}



QList<QMap<QString, QString>> KeyboardController::getTalkHistory() {
    QList<QMap<QString, QString>> talkHistory;
    QFile fileHistory("talkHistory.csv");
    if (fileHistory.open(QIODevice::ReadOnly)) {
        QString data;
        data = fileHistory.readAll();
        talkHistory = read::ReadHistory(data);
    }
    return talkHistory;
}

void KeyboardController::addHistory(QString time, QString data, QList<QMap<QString, QString>> talkHistory)
{
    QMap<QString, QString> entry;
    entry["Time"] = time;
    entry["Content"] = data;
    talkHistory.append(entry);
    saveHistorytoFile(talkHistory);
}


void KeyboardController::saveHistorytoFile(QList<QMap<QString, QString>> talkHistory)
{
    QFile file("talkHistory.csv");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setCodec("UTF-8"); // ...
    out.setGenerateByteOrderMark(false);
    out << "Time" << "," << "Content" << "\n";
    for (int i = 0; i < talkHistory.length(); i++) {
        QString text = talkHistory[i]["Time"] + "," + talkHistory[i]["Content"] + "\n";
        out << text;
    }
    file.close();
}


void KeyboardController::on_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    Key key;
    if (btn->text() == "&&") {
        key = dictKey["and"];
    }
    else {
        key = dictKey[mapKeyToName[btn->text()]];
    }

    if (key.keyData.name == "Speak") {
        //
    }
    else if (key.keyData.name == "Left") {
        lineEdit->cursorBackward(false, 1);
        lineEdit->setFocus();
    }
    else if (key.keyData.name == "Right") {
        lineEdit->cursorForward(false, 1);
        lineEdit->setFocus();
    }
    else if (key.keyData.name == "BackSpace") {
        lineEdit->backspace();
        lineEdit->setFocus();
    }
    else if (key.keyData.name == "Return") { 
        if (PREVIOUS_WIDGET == "talkLayout") {
            if (lineEdit->text() != "") { 
                QDateTime time = QDateTime::currentDateTime();
                QString time_text = time.toString("dd/MM/yyyy") + " " + time.toString("t");
                auto talkHistory = getTalkHistory();
                addHistory(time_text, lineEdit->text(), talkHistory);
                TalkController* talkWidget = new TalkController(this);
                stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
                stackedWidgets->addWidget(talkWidget);
                LIST_WIDGET_MAP[CURRENT_WIDGET] = talkWidget;
                stackedWidgets->setCurrentWidget(talkWidget);
            }
            else {
                stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP[PREVIOUS_WIDGET]);
                swap(PREVIOUS_WIDGET, CURRENT_WIDGET);
            }
        }
        else if (PREVIOUS_WIDGET == "dictionaryLayout") {
            QString text = lineEdit->text();
            if (lineEdit->text() != "") {
                auto dictionaryHistory = getDictHistory();
                if (DICTIONARY_CHOOSE_TYPE == 1) { 
                    QString Acronym = dictionaryHistory.at(DICTIONARY_CHOOSE)["Acronym"];
                    dictionaryHistory.removeAt(DICTIONARY_CHOOSE);
                    editTXTHistory(Acronym, text,DICTIONARY_CHOOSE ,dictionaryHistory);
                }
                else if (DICTIONARY_CHOOSE_TYPE == 2) {
                    QString TXT = dictionaryHistory.at(DICTIONARY_CHOOSE)["Text"];
                    dictionaryHistory.removeAt(DICTIONARY_CHOOSE);
                    editTXTHistory(text, TXT, DICTIONARY_CHOOSE, dictionaryHistory);
                }
                else {
                    QString Acronym = text[0];
                    for (int i = 0; i < text.length(); i++) {
                        if (text[i] == " ") {
                            Acronym += text[i + 1];
                            addHistoryDict(Acronym, text, dictionaryHistory);
                        }
                    }
                }   
                DictionaryController* dictionaryWidget = new DictionaryController(this);
                stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
                stackedWidgets->addWidget(dictionaryWidget);
                LIST_WIDGET_MAP[CURRENT_WIDGET] = dictionaryWidget;
                stackedWidgets->setCurrentWidget(dictionaryWidget);
            }
            else {
                stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP[PREVIOUS_WIDGET]);
                swap(PREVIOUS_WIDGET, CURRENT_WIDGET);
            }
        }
    }
    else if (key.keyData.name == "Clear") { // đoạn này kiểm tra clear toàn là name thôi

        lineEdit->clear();
        lineEdit->setFocus();
    }
    else if (key.keyData.name == "Lang") {
        if (key.keyData.key == "Vi") {
            lang = 0;
            key.keyData.key = "En";
            //btn->setText(key.keyData.key);
        }
        else {
            lang = 1;
            key.keyData.key = "Vi";
            //btn->setText(key.keyData.key);
        }
        dictKey[key.keyData.name] = key;
        keyFrom["Speak"][0] = key;
        mapKeyToName[key.keyData.key] = key.keyData.name;
        LANG["keyboardLayout"] = lang;
        LIST_DICT_KEY["keyboardLayout"] = dictKey;
        LIST_KEY_FROM["keyboardLayout"] = keyFrom;
        LIST_MAP_KEY_TO_NAME["keyboardLayout"] = mapKeyToName;

        Sleep(50);
    }
    else if (key.keyData.name == "KB" && key.keyData.key == "@#$") {
        QMapIterator<int, QString> iter(specialKeyboard);
        while (iter.hasNext()) {
            iter.next();

            QPushButton* addBtn = listButton[iter.key()];
            // xoa button space
            if (iter.key() == 37) {
                keyboardLayout->removeWidget(addBtn);
                addBtn->setParent(NULL);
                delete addBtn;
            }
            // them 3 button thay the cho space
            if (iter.key() >= 37) {
                addBtn = new QPushButton();
                addBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                addBtn->setStyleSheet(BTN_QSS);
                connect(addBtn, SIGNAL(clicked()), this, SLOT(on_clicked()));
                int x = iter.key() / 8;
                int y = iter.key() - (8 * x);
                keyboardLayout->addWidget(addBtn, x, y, 1, 1);
                listButton[iter.key()] = addBtn;
            }
            else {
                addBtn = listButton[iter.key()];
            }
            addBtn->setText(iter.value());

            if (iter.value() == "&") {
                addBtn->setText("&&");
            }

        }
        kb = 2;
        key.keyData.key = "abc";
        dictKey[key.keyData.name] = key;
        keyFrom["Return"][0] = key;
        mapKeyToName[key.keyData.key] = key.keyData.name;

        KB["keyboardLayout"] = kb;
        LIST_BUTTON_MAP["keyboardLayout"] = listButton;
        LIST_DICT_KEY["keyboardLayout"] = dictKey;
        LIST_KEY_FROM["keyboardLayout"] = keyFrom;
        LIST_MAP_KEY_TO_NAME["keyboardLayout"] = mapKeyToName;
    }
    else if (key.keyData.name == "KB" && key.keyData.key == "abc") {
        QMapIterator<int, QString> iter(normalKeyboard);
        while (iter.hasNext()) {
            iter.next();

            QPushButton* addBtn = listButton[iter.key()];
            // them button space thay the cho 3 button ky tu
            if (iter.key() >= 37) {
                if (iter.key() > 37) {
                    keyboardLayout->removeWidget(addBtn);
                    addBtn->setParent(NULL);
                    delete addBtn;
                }
                else {
                    keyboardLayout->removeWidget(addBtn);
                    addBtn = new QPushButton();
                    addBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                    addBtn->setStyleSheet(BTN_QSS);
                    connect(addBtn, SIGNAL(clicked()), this, SLOT(on_clicked()));
                    int x = iter.key() / 8;
                    int y = iter.key() - (8 * x);
                    keyboardLayout->addWidget(addBtn, x, y, 1, 3);
                    listButton[iter.key()] = addBtn;
                }
            }
            else {
                addBtn = listButton[iter.key()];
            }
            addBtn->setText(iter.value());
        }
        // gan vi tri 38, 39 cho button space
        listButton[38] = listButton[37];
        listButton[39] = listButton[37];

        kb = 1;
        key.keyData.key = "@#$";
        dictKey[key.keyData.name] = key;
        keyFrom["Return"][0] = key;

        KB["keyboardLayout"] = kb;
        LIST_BUTTON_MAP["keyboardLayout"] = listButton;
        LIST_DICT_KEY["keyboardLayout"] = dictKey;
        LIST_KEY_FROM["keyboardLayout"] = keyFrom;
    }
    //else if (key.keyData.name == "Huyen") {

    //}
    //else if (key.keyData.name == "Hoi") {

    //}
    //else if (key.keyData.name == "Sac") {

    //}
    //else if (key.keyData.name == "Nga") {

    //}
    //else if (key.keyData.name == "Nang") {

    //}
    else if (key.keyData.name == "newline") {
        QString txt = lineEdit->text();
        lineEdit->setText(txt + "\n");
    }
    else {
        int current_pos = lineEdit->cursorPosition();
        QString textLine = lineEdit->text();
        QString prev_text = textLine.mid(0, current_pos);
        QString next_text = textLine.mid(current_pos, textLine.length());
        QString current_text = btn->text();
        if (key.keyData.name == "Space") {
            current_text = " ";
        }

        if (key.keyData.name == "and") {
            current_text = "&";
        }

        if (lang == 0) {
            QString kname = key.keyData.name.toLower();
            if (textLine.length() > 0) {
                if (kname == "z" || kname == "r" || kname == "s" || kname == "f" || kname == "j" || kname == "x" ||
                    kname == "w" || kname == "a" || kname == "o" || kname == "d" || kname == "e" || kname == "u") {
                    int len = 4;

                    if (prev_text.length() < len) len = prev_text.length();

                    for (int k = len; k > 0; k--) {
                        QString l1 = prev_text.mid(0, prev_text.length() - k);
                        QString prefix = prev_text.mid(prev_text.length() - k, k);
                        QString vkey = prefix + kname;
                        QString dau;
                        if (bangdau.contains(vkey) && bangdau[vkey] != "") {
                            dau = bangdau[vkey];
                            QString txt = l1 + dau;
                            if (kname == "z" || kname == "r" || kname == "s" || kname == "f" || kname == "j" || kname == "x") {
                                txt += " ";
                            }
                            prev_text = txt;
                            current_pos = txt.length();
                            current_text = "";
                            break;
                        }
                    }
                }
            }
        }

        lineEdit->setText(prev_text + current_text + next_text);
        lineEdit->setCursorPosition(current_pos + current_text.length());
        lineEdit->setFocus();
    }
}

