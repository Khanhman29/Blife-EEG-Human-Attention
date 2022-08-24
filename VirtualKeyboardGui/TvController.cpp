#include "TvController.h"
#include "APIService.h"
#include "SettingService.h"
#include <iostream>

APIService tvApi;
int tvID = 0;

void TvController::SetId(int id)
{
	tvID = id;
}

TvController::TvController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "tvLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* tvWidget = new QWidget;
	mainLayout->addWidget(tvWidget);

	tvLayout = new QGridLayout(tvWidget);
	tvWidget->setLayout(tvLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("TvConfig.csv");
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
		QIcon icon;
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
		else if (keydata.name == "next_channel") {
			btn = SettingService::GetIconButton(btn, "icon/icon_next_channel" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(nextChannelClick()));
		}
		else if (keydata.name == "prev_channel") {
			btn = SettingService::GetIconButton(btn, "icon/icon_prev_channel" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(prevChannelClick()));
		}
		else if (keydata.name == "volume_up") {
			btn = SettingService::GetIconButton(btn, "icon/icon_volume_up" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(volumeUpClick()));
		}
		else if (keydata.name == "volume_down") {
			btn = SettingService::GetIconButton(btn, "icon/icon_volume_down" + prefix, QSize(200, 200));
			connect(btn, SIGNAL(clicked()), this, SLOT(volumeDownClick()));
		}
		else if (keydata.name.left(3) == "num") {
			QString num = keydata.name.at(keydata.name.length() - 1);
			QString path = "icon/" + num + SettingService::GetColorPrefix();
			btn = SettingService::GetIconButton(btn, path, QSize(50, 50));
			connect(btn, SIGNAL(clicked()), this, SLOT(numClick()));
		}
		else if (keydata.name == "target_channel") {
			btn = SettingService::GetIconButton(btn, "icon/icon_target_channel" + prefix, QSize(200, 200));
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}
		else {
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		tvLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < tvLayout->columnCount(); c++) {
		tvLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < tvLayout->rowCount(); r++) {
		tvLayout->setRowStretch(r, 1);
	}
}

TvController::~TvController()
{
}

void TvController::homeClick()
{
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["smartHomeLayout"]);
	CURRENT_WIDGET = "smartHomeLayout";
}

void TvController::nextChannelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_CHANNELUP\"}", tvID);
	int messageLen = std::to_string(tvID).length() + 39;

	tvApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void TvController::prevChannelClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_CHANNELDOWN\"}", tvID);
	int messageLen = std::to_string(tvID).length() + 41;

	tvApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void TvController::volumeUpClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_VOLUMEUP\"}", tvID);
	int messageLen = std::to_string(tvID).length() + 38;

	tvApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void TvController::volumeDownClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_VOLUMEDOWN\"}", tvID);
	int messageLen = std::to_string(tvID).length() + 40;

	tvApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void TvController::onClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_POWER\"}", tvID);
	int messageLen = std::to_string(tvID).length() + 35;

	tvApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void TvController::offClick()
{
	char topic[] = "iot/control";
	char payload[80];
	sprintf(payload, "{\"id\": \"%d\", \"command\": \"KEY_POWER\"}", tvID);
	int messageLen = std::to_string(tvID).length() + 35;

	tvApi.PublishMessage(topic, const_cast<char*>(payload), messageLen);
}

void TvController::numClick()
{
	
}
