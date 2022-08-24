#include "LightController.h"
#include "APIService.h"
#include "SettingService.h"
#include "HomeService.h"
#include <iostream>

APIService lightApi;
int lightID = 0;

void LightController::SetId(int id)
{
	lightID = id;
}

LightController::LightController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "lightLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* lightWidget = new QWidget;
	mainLayout->addWidget(lightWidget);

	lightLayout = new QGridLayout(lightWidget);
	lightWidget->setLayout(lightLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("LightConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	QString prefix = SettingService::GetPrefix();

	for (int i = 0; i < dataconfig.count(); i++) {
		KeyData keydata(dataconfig[i]);
		keydata.dwTime = SettingService::dwTime;
		Key key;
		key.keyData = keydata;
		QPushButton* btn = new QPushButton();
		btn->setText(keydata.key);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet(BTN_QSS);

		std::cout << "Reading ", key.keyData.name;
		listKeyButton[key.keyData.name] = btn;

		if (keydata.name == "Home") {
			btn = SettingService::GetIconButton(btn, "icon/icon_Home" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
		}
		else if (keydata.name == "title") {
			btn = SettingService::GetIconButton(btn, "icon/light_background", QSize(100, 100));
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}
		else if (keydata.name == "on") {
			btn = SettingService::GetIconButton(btn, "icon/icon_on" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(onClick()));
		}
		else if (keydata.name == "off") {
			btn = SettingService::GetIconButton(btn, "icon/icon_off" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(offClick()));
		}
		else {
			btn->setEnabled(false);
			//btn->setText(QString::fromStdString(std::to_string(ID)));
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		lightLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < lightLayout->columnCount(); c++) {
		lightLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < lightLayout->rowCount(); r++) {
		lightLayout->setRowStretch(r, 1);
	}
}

LightController::~LightController()
{
}

void LightController::homeClick()
{
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["smartHomeLayout"]);
	CURRENT_WIDGET = "smartHomeLayout";
}

void LightController::onClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_ON\"}", lightID);
	int messageLen = std::to_string(lightID).length() + 32;

	lightApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void LightController::offClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_OFF\"}", lightID);
	int messageLen = std::to_string(lightID).length() + 33;

	lightApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}
