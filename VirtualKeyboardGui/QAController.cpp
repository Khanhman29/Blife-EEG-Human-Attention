#include "QAController.h"
QAController::QAController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "qaLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* qaWidget = new QWidget;
	mainLayout->addWidget(qaWidget);

	qaLayout = new QGridLayout(qaWidget);
	qaWidget->setLayout(qaLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("QAConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	for (int i = 0; i < dataconfig.count(); i++) {

		KeyData keydata(dataconfig[i]);
		Key key;
		key.keyData = keydata;
		QPushButton* btn = new QPushButton();
		//btn->setText(keydata.name);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet(BTN_QSS);

		if (key.keyData.name == "Home") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(HomeClick()));
			//listKeyButton["Home"] = btn;
		}
		else if (key.keyData.name == "lockButton") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(FirstPageClick()));
			//listKeyButton["FirstPage"] = btn;
		}
		else {
			btnTXT.append(btn);
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(chooseClick()));
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		qaLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

		// listButton
		for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
			for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
				listButton[keydata.pos + exPosX + COL * exPosY] = btn;
				mapKeyToName[QString::number(keydata.pos + exPosX + COL * exPosY)] = key.keyData.name;
			}
		}

		dictKey[keydata.name] = key;

	}

	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;

	for (int c = 0; c < qaLayout->columnCount(); c++) {
		qaLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < qaLayout->rowCount(); r++) {
		qaLayout->setRowStretch(r, 1);
	}
}

QAController::~QAController()
{
}
void QAController::HomeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["mainMenuLayout"]);
	CURRENT_WIDGET = "mainMenuLayout";
}
void QAController::LockButtonClick() {

}

void QAController::chooseClick()
{
	QObject* obj = sender();
	for (int i = 0; i < 10; i++) {
		if (obj == btnTXT[i]) {
			loadAudio(btnTXT[i]->text());
		}
	}
}

void QAController::loadAudio(QString txt)
{
	Key::playAudio(txt);
}
