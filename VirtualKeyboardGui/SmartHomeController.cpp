#include "SmartHomeController.h"
#include "CalibController.h"
#include "LightController.h"
#include "TvController.h"
#include "FanController.h"
#include "AcController.h"
#include "SettingController.h"
#include "SettingService.h"
#include "APIService.h"
#include "HomeService.h"
#include <iostream>
#include <string>

QJsonObject lightDeviceList[10];
QJsonObject fanDeviceList[10];
QJsonObject tvDeviceList[10];
QJsonObject acDeviceList[10];

void GetDataFromJson(QString data) {
	QString val = data;
	val.replace("\n", "");
	val.replace("'", "\"");

	QJsonDocument jsonResponse = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject jsonObject = jsonResponse.object();
	QJsonArray jsonArray = jsonObject["data"].toArray();

	int lightCount = 0;
	int fanCount = 0;
	int tvCount = 0;
	int acCount = 0;

	foreach(const QJsonValue & value, jsonArray) {
		QJsonObject obj = value.toObject();
		QString type = obj["productType"].toString();
		if (type == "switch") {
			lightDeviceList[lightCount] = obj;
			lightCount++;
		}
		else if (type == "fan") {
			fanDeviceList[fanCount] = obj;
			fanCount++;
		}
		else if (type == "tv") {
			tvDeviceList[tvCount] = obj;
			tvCount++;
		}
		else if (type == "air-conditioner") {
			acDeviceList[acCount] = obj;
			acCount++;
		}
	}

}

SmartHomeController::SmartHomeController(QWidget *parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "smartHomeLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* settingWidget = new QWidget;
	mainLayout->addWidget(settingWidget);

	settingLayout = new QGridLayout(settingWidget);
	settingWidget->setLayout(settingLayout);

	//std::string data = APIService::GetDevicesAPI();
	//GetDataFromJson(data);
	APIService::GetDevicesAPI();
	QString data = APIService::ReadFile();
	GetDataFromJson(data);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("SmartHomeConfig.csv");
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
		keydata.dwTime = SettingService::dwTime;
		Key key;
		key.keyData = keydata;
		QPushButton* btn = new QPushButton();
		QIcon icon;
		btn->setText(keydata.key);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet(BTN_QSS);

		if (keydata.name == "Home") {
			icon.addPixmap(QPixmap("icon/home_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/home_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setIconSize(QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
		}
		else if (keydata.name == "light_device1") {
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Light 1");
			btn->setIconSize(QSize(40, 40));
			if (lightDeviceList[0]["deviceName"].toString() != "") {
				btn->setText(lightDeviceList[0]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(light1Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "light_device2") {
			
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Light 2");
			btn->setIconSize(QSize(40, 40));
			if (lightDeviceList[1]["deviceName"].toString() != "") {
				btn->setText(lightDeviceList[1]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(light2Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "light_device3") {
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Light 3");
			btn->setIconSize(QSize(40, 40));
			if (lightDeviceList[2]["deviceName"].toString() != "") {
				btn->setText(lightDeviceList[2]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(light3Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "tv_device1") {
			
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Television 1");
			btn->setIconSize(QSize(40, 40));
			if (tvDeviceList[0]["deviceName"].toString() != "") {
				btn->setText(tvDeviceList[0]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(tv1Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "tv_device2") {
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Television 2");
			btn->setIconSize(QSize(40, 40));
			if (tvDeviceList[1]["deviceName"].toString() != "") {
				btn->setText(tvDeviceList[1]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(tv2Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "tv_device3") {
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Television 3");
			btn->setIconSize(QSize(40, 40));
			if (tvDeviceList[2]["deviceName"].toString() != "") {
				btn->setText(tvDeviceList[2]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(tv3Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "fan_device1") {
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Fan 1");
			btn->setIconSize(QSize(40, 40));
			if (fanDeviceList[0]["deviceName"].toString() != "") {
				btn->setText(fanDeviceList[0]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(fan1Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "fan_device2") {
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Fan 2");
			btn->setIconSize(QSize(40, 40));
			if (fanDeviceList[1]["deviceName"].toString() != "") {
				btn->setText(fanDeviceList[1]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(fan2Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "fan_device3") {
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Fan 3");
			btn->setIconSize(QSize(40, 40));
			if (fanDeviceList[2]["deviceName"].toString() != "") {
				btn->setText(fanDeviceList[2]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(fan3Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "ac_device1") {
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Air conditioner 1");
			btn->setIconSize(QSize(40, 40));
			if (acDeviceList[0]["deviceName"].toString() != "") {
				btn->setText(acDeviceList[0]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(ac1Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "ac_device2") {
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Air conditioner 2");
			btn->setIconSize(QSize(40, 40));
			if (acDeviceList[1]["deviceName"].toString() != "") {
				btn->setText(acDeviceList[1]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(ac2Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "ac_device3") {
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setText("Air conditioner 3");
			btn->setIconSize(QSize(40, 40));
			if (acDeviceList[2]["deviceName"].toString() != "") {
				btn->setText(acDeviceList[2]["deviceName"].toString());
				connect(btn, SIGNAL(clicked()), this, SLOT(ac3Click()));
			}
			else {
				btn->setEnabled(false);
				key.isHover = false;
			}
		}
		else if (keydata.name == "light_devices") {
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/light_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setIconSize(QSize(250, 250));
			btn->setEnabled(false);
			key.isHover = false;
		}
		else if (keydata.name == "tv_devices") {
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/tv_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setIconSize(QSize(250, 250));
			btn->setEnabled(false);
			key.isHover = false;
		}
		else if (keydata.name == "fan_devices") {
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/fan_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setIconSize(QSize(250, 250));
			btn->setEnabled(false);
			key.isHover = false;
		}
		else if (keydata.name == "ac_devices") {
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/ac_background.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setIconSize(QSize(250, 250));
			btn->setEnabled(false);
			key.isHover = false;
		}
		else if (keydata.name == "layout_setting") {
			icon.addPixmap(QPixmap("icon/st.png"), QIcon::Disabled, QIcon::On);
			icon.addPixmap(QPixmap("icon/st.png"), QIcon::Disabled, QIcon::Off);
			btn->setIcon(icon);
			btn->setIconSize(QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(settingClick()));
		}
		else {
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}

		std::cout << "Reading ", key.keyData.name;
		listKeyButton[key.keyData.name] = btn;

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		settingLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

		// listButton
		for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
			for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
				listButton[keydata.pos + exPosX + COL * exPosY] = btn;
				mapKeyToName[QString::number(keydata.pos + exPosX + COL * exPosY)] = key.keyData.name;
			}
		}

		dictKey[keydata.name] = key;
		
		//TestJson();
	}

	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;

	for (int c = 0; c < settingLayout->columnCount(); c++) {
		settingLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < settingLayout->rowCount(); r++) {
		settingLayout->setRowStretch(r, 1);
	}

}

SmartHomeController::~SmartHomeController()
{
}

void SmartHomeController::homeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["mainMenuLayout"]);
	CURRENT_WIDGET = "mainMenuLayout";
}



void SmartHomeController::light1Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	LightController* lightWidget = new LightController(this);
	lightWidget->SetId(lightDeviceList[0]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(lightWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = lightWidget;
	stackedWidgets->setCurrentWidget(lightWidget);
}

void SmartHomeController::light2Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	LightController* lightWidget = new LightController(this);
	lightWidget->SetId(lightDeviceList[1]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(lightWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = lightWidget;
	stackedWidgets->setCurrentWidget(lightWidget);
}

void SmartHomeController::light3Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	LightController* lightWidget = new LightController(this);
	lightWidget->SetId(lightDeviceList[2]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(lightWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = lightWidget;
	stackedWidgets->setCurrentWidget(lightWidget);
}



void SmartHomeController::tv1Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	TvController* tvWidget = new TvController(this);
	tvWidget->SetId(tvDeviceList[0]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(tvWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = tvWidget;
	stackedWidgets->setCurrentWidget(tvWidget);
}

void SmartHomeController::tv2Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	TvController* tvWidget = new TvController(this);
	tvWidget->SetId(tvDeviceList[1]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(tvWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = tvWidget;
	stackedWidgets->setCurrentWidget(tvWidget);
}

void SmartHomeController::tv3Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	TvController* tvWidget = new TvController(this);
	tvWidget->SetId(tvDeviceList[2]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(tvWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = tvWidget;
	stackedWidgets->setCurrentWidget(tvWidget);
}


void SmartHomeController::fan1Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	FanController* fanWidget = new FanController(this);
	fanWidget->SetId(fanDeviceList[0]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(fanWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = fanWidget;
	stackedWidgets->setCurrentWidget(fanWidget);
}

void SmartHomeController::fan2Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	FanController* fanWidget = new FanController(this);
	fanWidget->SetId(fanDeviceList[1]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(fanWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = fanWidget;
	stackedWidgets->setCurrentWidget(fanWidget);
}

void SmartHomeController::fan3Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	FanController* fanWidget = new FanController(this);
	fanWidget->SetId(fanDeviceList[2]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(fanWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = fanWidget;
	stackedWidgets->setCurrentWidget(fanWidget);
}


void SmartHomeController::ac1Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	AcController* acWidget = new AcController(this);
	acWidget->SetId(acDeviceList[0]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(acWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = acWidget;
	stackedWidgets->setCurrentWidget(acWidget);
}

void SmartHomeController::ac2Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	AcController* acWidget = new AcController(this);
	acWidget->SetId(acDeviceList[1]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(acWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = acWidget;
	stackedWidgets->setCurrentWidget(acWidget);
}

void SmartHomeController::ac3Click() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	AcController* acWidget = new AcController(this);
	acWidget->SetId(acDeviceList[2]["deviceId"].toInt());
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(acWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = acWidget;
	stackedWidgets->setCurrentWidget(acWidget);
}


void SmartHomeController::settingClick() {
	PREVIOUS_WIDGET = "smartHomeLayout";
	SettingController* settingWidget = new SettingController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(settingWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = settingWidget;
	stackedWidgets->setCurrentWidget(settingWidget);
}