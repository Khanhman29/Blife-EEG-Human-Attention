#pragma once

#include <QWidget>

#include "Key.h"

class GreetingsController : public QWidget
{
	Q_OBJECT

public:
	GreetingsController(QWidget* parent);
	~GreetingsController();

private slots:
	void HomeClick();
	void LockButtonClick();
	void chooseClick();
private:
	void loadAudio(QString txt);

private:
	QGridLayout* greetingsLayout;

	QMap<QString, Key> dictKey;
	QList<QPushButton* > btnTXT;
	QMap<int, QPushButton*> listButton;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};