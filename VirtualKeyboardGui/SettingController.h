#pragma once
#include <QWidget>
#include "Key.h"
#include <qwidget.h>
class SettingController : public QWidget
{
	Q_OBJECT
public:
	SettingController(QWidget* parent);
	~SettingController();

private slots:
	void homeClick();
	void languageChangeClick();
	void colorChangeClick();
	void speedDownClick();
	void speedUpClick();

private:
	QGridLayout* settingLayout;

	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};

