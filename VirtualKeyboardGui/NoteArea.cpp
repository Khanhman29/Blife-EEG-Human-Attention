#include "NoteArea.h"
#include "NoteController.h"
#include "TypingController.h"
#include "KeyboardController.h"
NoteArea::NoteArea(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "noteAreaLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* noteAreaWidget = new QWidget;
	mainLayout->addWidget(noteAreaWidget);

	noteAreaLayout = new QGridLayout(noteAreaWidget);
	noteAreaWidget->setLayout(noteAreaLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("textAreaConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	lineEdit = new QLineEdit();
	lineEdit->setPlaceholderText("Enter text...");
	lineEdit->setFocus();
	lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	lineEdit->setStyleSheet(EDIT_QSS);

	for (int i = 0; i < dataconfig.count(); i++) {
		KeyData keydata(dataconfig[i]);
		if (keydata.keyType == 2) {
			int pos = keydata.pos;
			if (pos < COL) {
				noteAreaLayout->addWidget(lineEdit, 0, pos, keydata.spanY, keydata.spanX);
			}
			else {
				int y = pos / COL;
				int x = pos - (COL * y);
				noteAreaLayout->addWidget(lineEdit, y, x, keydata.spanY, keydata.spanX);
			}
		}
		// kb < 2
		else if (keydata.keyType == 1 && keydata.KB < 2) {
			Key key;
			key.keyData = keydata;
			//main key
			if (key.keyData.exFrom == "") {
				QPushButton* btn = new QPushButton();
				btn->setText(keydata.key);
				btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
				btn->setStyleSheet(BTN_QSS);
				connect(btn, SIGNAL(clicked()), this, SLOT(on_clicked()));
				int r = keydata.pos / COL;
				int c = keydata.pos - (COL * r);
				noteAreaLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

				// listButton
				for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
					for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
						listButton[keydata.pos + exPosX + COL * exPosY] = btn;
						//mapKeyToName[QString::number(keydata.pos + exPosX + COL * exPosY)] = key.keyData.name; // chỗ này ông phải thống nhất 1 kiểu
					}
				}
				mapKeyToName[key.keyData.key] = key.keyData.name;
				dictKey[keydata.name] = key;
				
			}
			//exfrom key
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

	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;
	LIST_KEY_FROM[CURRENT_WIDGET] = keyFrom;

	for (int c = 0; c < noteAreaLayout->columnCount(); c++) {
		noteAreaLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < noteAreaLayout->rowCount(); r++) {
		noteAreaLayout->setRowStretch(r, 1);
	}
}

NoteArea::~NoteArea()
{
}

void NoteArea::on_clicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    Key key;

    if (key.keyData.name == "Return") {
        //
    }
    else if (key.keyData.name == "Keyboard") {

    }
    else if (key.keyData.name == "Paste") {

    }
    else if (key.keyData.name == "Copy") {

    }
    else if (key.keyData.name == "Backspace") {
        lineEdit->backspace();
        lineEdit->setFocus();
    }
	else if (key.keyData.name == "Clear") {
		lineEdit->clear();
		lineEdit->setFocus();
	}
    else if (key.keyData.name == "Left") {
        lineEdit->cursorBackward(false, 1);
        lineEdit->setFocus();
    }
	else if (key.keyData.name == "LineBegin") {
	}
	else if (key.keyData.name == "TextBegin") {
	}
	else if (key.keyData.name == "Up") {
	}
	else if (key.keyData.name == "Select") {
	}
	else if (key.keyData.name == "Down") {
	}
    else if (key.keyData.name == "Right") {
        lineEdit->cursorForward(false, 1);
        lineEdit->setFocus();
    }
	else if (key.keyData.name == "LineEnd") {
	}
	else if (key.keyData.name == "TextEnd") {
	}
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
        lineEdit->setText(prev_text + current_text + next_text);
        lineEdit->setCursorPosition(current_pos + current_text.length());
        lineEdit->setFocus();
    }
}
