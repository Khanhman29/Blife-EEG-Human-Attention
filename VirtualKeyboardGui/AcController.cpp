#include "AcController.h"
#include "APIService.h"
#include "SettingService.h"
#include "HomeService.h"
#include <iostream>

APIService acApi;
int acID = 0;
QPushButton* currentTemp;

void AcController::SetId(int id)
{
	acID = id;
}

AcController::AcController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "fanLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* acWidget = new QWidget;
	mainLayout->addWidget(acWidget);

	acLayout = new QGridLayout(acWidget);
	acWidget->setLayout(acLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("AcConfig.csv");
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
			btn = SettingService::GetIconButton(btn, "icon/icon_Home" + prefix, QSize(400, 400));
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
		}
		else if (keydata.name == "on") {
			btn = SettingService::GetIconButton(btn, "icon/icon_on" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(onClick()));
		}
		else if (keydata.name == "off") {
			btn = SettingService::GetIconButton(btn, "icon/icon_off" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(offClick()));
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
		else if (keydata.name == "current_temp") {
			btn = SettingService::GetIconButton(btn, "icon/icon_thermometer.png", QSize(100, 100));
			btn->setText(" = " + QString::number(HomeService::currentTemp));
			btn->setEnabled(false);
			currentTemp = btn;
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}
		else if (keydata.name == "temp_up") {
			btn = SettingService::GetIconButton(btn, "icon/icon_temp_up.png", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(tempUpClick()));
		}
		else if (keydata.name == "temp_down") {
			btn = SettingService::GetIconButton(btn, "icon/icon_temp_down.png", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(tempDownClick()));
		}
		else if (keydata.name == "swing") {
			btn = SettingService::GetIconButton(btn, "icon/icon_swing.png", QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(swingClick()));
		}
		else {
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		acLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < acLayout->columnCount(); c++) {
		acLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < acLayout->rowCount(); r++) {
		acLayout->setRowStretch(r, 1);
	}
}

AcController::~AcController()
{
}

void AcController::homeClick()
{
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["smartHomeLayout"]);
	CURRENT_WIDGET = "smartHomeLayout";
}

void AcController::firstLevelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_AIRLOW\"}", acID);
	int messageLen = std::to_string(acID).length() + 36;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::secondLevelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_AIRMED\"}", acID);
	int messageLen = std::to_string(acID).length() + 36;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::thirdLevelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_AIRHIGH\"}", acID);
	int messageLen = std::to_string(acID).length() + 37;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::tempUpClick()
{
	if (HomeService::currentTemp < 30) {
		HomeService::currentTemp++;
		currentTemp->setText(" = " + QString::number(HomeService::currentTemp));
	}
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_%d\"}", acID, HomeService::currentTemp);
	int messageLen = std::to_string(acID).length() + 32;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::tempDownClick()
{
	if (HomeService::currentTemp > 18) {
		HomeService::currentTemp--;
		currentTemp->setText(" = " + QString::number(HomeService::currentTemp));
	}
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_%d\"}", acID, HomeService::currentTemp);
	int messageLen = std::to_string(acID).length() + 32;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::swingClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_SWING\"}", acID);
	int messageLen = std::to_string(acID).length() + 35;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::offClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_OFF\"}", acID);
	int messageLen = std::to_string(acID).length() + 33;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void AcController::onClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_ON\"}", acID);
	int messageLen = std::to_string(acID).length() + 32;

	acApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}


