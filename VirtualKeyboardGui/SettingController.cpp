#include "SettingController.h"
#include "SettingService.h"
#include "SmartHomeController.h"
#include <string>
#include <iostream>

QPushButton* languageStatusButton;
QPushButton* colorStatusButton;
QPushButton* speedStatusButton;
QPushButton* homeButton;
std::string language = SettingService::CurrentLanguage;

SettingController::SettingController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "settingLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* settingWidget = new QWidget;
	mainLayout->addWidget(settingWidget);

	settingLayout = new QGridLayout(settingWidget);
	settingWidget->setLayout(settingLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("SettingConfig.csv");
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

		listKeyButton[key.keyData.name] = btn;

		if (keydata.name == "Home") {
			btn = SettingService::GetIconButton(btn, "icon/icon_Home" + prefix, QSize(200, 200));
			homeButton = btn;
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
		}
		else if (keydata.name == "title") {
			btn = SettingService::GetIconButton(btn, "icon/st", QSize(100, 100));
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}
		else if (keydata.name == "language_change") {
			btn = SettingService::GetIconButton(btn, "icon/icon_language_change", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(languageChangeClick()));
		}
		else if (keydata.name == "language") {
			languageStatusButton = btn;
			btn->setText(QString::fromUtf8(SettingService::GetLanguageName().c_str()));
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}
		else if (keydata.name == "color_change") {
			btn = SettingService::GetIconButton(btn, "icon/icon_color_change", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(colorChangeClick()));
		}
		else if (keydata.name == "color") {
			colorStatusButton = btn;
			btn->setText(QString::fromUtf8(SettingService::GetColorName().c_str()));
			btn->setEnabled(false);
			key.isHover = false;
			key.color = "black";
			key.setColor(btn, "40");
		}
		else if (keydata.name == "speed_down") {
			btn = SettingService::GetIconButton(btn, "icon/icon_speed_down", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(speedDownClick()));
		}
		else if (keydata.name == "speed_up") {
			btn = SettingService::GetIconButton(btn, "icon/icon_speed_up", QSize(100, 100));
			connect(btn, SIGNAL(clicked()), this, SLOT(speedUpClick()));
		}
		else if (keydata.name == "speed") {
			speedStatusButton = btn;
			btn->setText(QString::number(SettingService::dwTime));
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
		settingLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < settingLayout->columnCount(); c++) {
		settingLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < settingLayout->rowCount(); r++) {
		settingLayout->setRowStretch(r, 1);
	}
}

SettingController::~SettingController()
{
}

void SettingController::homeClick()
{
	PREVIOUS_WIDGET = "mainMenuLayout";
	SmartHomeController* smarthomeWidget = new SmartHomeController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(smarthomeWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = smarthomeWidget;
	stackedWidgets->setCurrentWidget(smarthomeWidget);
}

void SettingController::languageChangeClick()
{
	if (SettingService::CurrentLanguage == "Vietnamese") {
		SettingService::CurrentLanguage = "English";
		languageStatusButton->setText(QString::fromUtf8(SettingService::GetLanguageName().c_str()));
		colorStatusButton->setText(QString::fromUtf8(SettingService::GetColorName().c_str()));
		homeButton = SettingService::GetIconButton(homeButton, "icon/icon_Home" + SettingService::GetPrefix(), QSize(200, 200));
	}
	else {
		SettingService::CurrentLanguage = "Vietnamese";
		languageStatusButton->setText(QString::fromUtf8(SettingService::GetLanguageName().c_str()));
		colorStatusButton->setText(QString::fromUtf8(SettingService::GetColorName().c_str()));
		homeButton = SettingService::GetIconButton(homeButton, "icon/icon_Home" + SettingService::GetPrefix(), QSize(200, 200));
	}
	
}

void SettingController::colorChangeClick()
{
	if (SettingService::CurrentColor == "Blue") {
		SettingService::CurrentColor = "Red";
		colorStatusButton->setText(QString::fromUtf8(SettingService::GetColorName().c_str()));
		homeButton = SettingService::GetIconButton(homeButton, "icon/icon_Home" + SettingService::GetPrefix(), QSize(200, 200));
	}
	else {
		SettingService::CurrentColor = "Blue";
		colorStatusButton->setText(QString::fromUtf8(SettingService::GetColorName().c_str()));
		homeButton = SettingService::GetIconButton(homeButton, "icon/icon_Home" + SettingService::GetPrefix(), QSize(200, 200));
	}

}

void SettingController::speedDownClick()
{
	if (SettingService::dwTime <= 1) return;

	SettingService::dwTime -= 0.2;
	speedStatusButton->setText(QString::number(SettingService::dwTime));
}

void SettingController::speedUpClick()
{
	if (SettingService::dwTime >= 3) return;

	SettingService::dwTime += 0.2;
	speedStatusButton->setText(QString::number(SettingService::dwTime));
}