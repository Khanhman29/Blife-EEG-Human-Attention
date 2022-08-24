#include "DianosController.h"
DianosController::DianosController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "dianosLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* talkWidget = new QWidget;
	mainLayout->addWidget(talkWidget);

	dianosLayout = new QGridLayout(talkWidget);
	talkWidget->setLayout(dianosLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("DianosticsConfig.csv");
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
		dianosLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < dianosLayout->columnCount(); c++) {
		dianosLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < dianosLayout->rowCount(); r++) {
		dianosLayout->setRowStretch(r, 1);
	}
}

DianosController::~DianosController()
{
}
void DianosController::chooseClick() {
	QObject* obj = sender();
	for (int i = 0; i < 10; i++) {
		if (obj == btnTXT[i]) {
			loadAudio(btnTXT[i]->text());
		}
	}
}
void DianosController::HomeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["mainMenuLayout"]);
	CURRENT_WIDGET = "mainMenuLayout";
}
void DianosController::LockButtonClick() {

}

void DianosController::loadAudio(QString txt)
{
	Key::playAudio(txt);
}
