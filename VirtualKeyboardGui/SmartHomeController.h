#pragma once

#include <QWidget>
#include "Key.h"

class SmartHomeController : public QWidget
{
	Q_OBJECT

public:
	SmartHomeController(QWidget *parent);
	~SmartHomeController();

private slots:
	void homeClick();
	void light1Click();
	void light2Click();
	void light3Click();
	void tv1Click();
	void tv2Click();
	void tv3Click();
	void fan1Click();
	void fan2Click();
	void fan3Click();
	void ac1Click();
	void ac2Click();
	void ac3Click();
	void settingClick();

private:
	QGridLayout* settingLayout;

	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};
