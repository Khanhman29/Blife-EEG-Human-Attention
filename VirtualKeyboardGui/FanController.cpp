#include "FanController.h"
#include "APIService.h"
#include "SettingService.h"
#include <iostream>

APIService fanApi;
int fanID = 0;

void FanController::SetId(int id)
{
	fanID = id;
}

FanController::FanController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "fanLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* fanWidget = new QWidget;
	mainLayout->addWidget(fanWidget);

	fanLayout = new QGridLayout(fanWidget);
	fanWidget->setLayout(fanLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("FanConfig.csv");
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
		else if (keydata.name == "on") {
			connect(btn, SIGNAL(clicked()), this, SLOT(onClick()));
			btn = SettingService::GetIconButton(btn, "icon/icon_on" + prefix, QSize(200, 200));
		}
		else if (keydata.name == "off") {
			connect(btn, SIGNAL(clicked()), this, SLOT(offClick()));
			btn = SettingService::GetIconButton(btn, "icon/icon_off" + prefix, QSize(200, 200));
		}
		else if (keydata.name == "level_1") {
			btn = SettingService::GetIconButton(btn, "icon/icon_level_1", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(firstLevelClick()));
		}
		else if (keydata.name == "level_2") {
			btn = SettingService::GetIconButton(btn, "icon/icon_level_2", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(secondLevelClick()));
		}
		else if (keydata.name == "level_3") {
			btn = SettingService::GetIconButton(btn, "icon/icon_level_3", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(thirdLevelClick()));
		}
		else {
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		fanLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < fanLayout->columnCount(); c++) {
		fanLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < fanLayout->rowCount(); r++) {
		fanLayout->setRowStretch(r, 1);
	}
}

FanController::~FanController()
{
}

void FanController::homeClick()
{
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["smartHomeLayout"]);
	CURRENT_WIDGET = "smartHomeLayout";
}

void FanController::firstLevelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_SPDLOW\"}", fanID);
	int messageLen = std::to_string(fanID).length() + 36;

	fanApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void FanController::secondLevelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_SPDMED\"}", fanID);
	int messageLen = std::to_string(fanID).length() + 36;

	fanApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void FanController::thirdLevelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_SPDHIGH\"}", fanID);
	int messageLen = std::to_string(fanID).length() + 37;

	fanApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void FanController::offClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_POWER\"}", fanID);
	int messageLen = std::to_string(fanID).length() + 35;

	fanApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void FanController::onClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_POWER\"}", fanID);
	int messageLen = std::to_string(fanID).length() + 35;

	fanApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

