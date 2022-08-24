#pragma once

#include <QWidget>

#include "Key.h"

class SupportController : public QWidget
{
	Q_OBJECT

public:
	SupportController(QWidget* parent);
	~SupportController();

private slots:
	void HomeClick();
	void LockButtonClick();
	void chooseClick();
private:
	void loadAudio(QString txt);
private:
	QGridLayout* supportLayout;

	QMap<QString, Key> dictKey;
	QList<QPushButton* > btnTXT;
	QMap<int, QPushButton*> listButton;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};