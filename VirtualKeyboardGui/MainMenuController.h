#pragma once

#include <QWidget>

#include "Key.h"

class MainMenuController : public QWidget
{
	Q_OBJECT

public:
	MainMenuController(QWidget *parent);
	~MainMenuController();

private slots:
	void helpClick();
	void dianosClick();
	void smartHomeClick();
	void lockClick();
	void greetingsClick();
	void qaClick();
	void supportClick();
	void talkClick();
	void noteClick();
	void dictionaryClick();
	void typingClick();
	void browserClick();
	void socialNetworkClick();

private:
	QGridLayout* mainMenuLayout;
	QList<QPushButton* > btnLayout;
	bool lock = false;
	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};
