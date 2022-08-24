#pragma once

#include <QWidget>

#include "Key.h"

class DianosController : public QWidget
{
	Q_OBJECT

public:
	DianosController(QWidget* parent);
	~DianosController();

private slots:
	void HomeClick();
	void LockButtonClick();
	void chooseClick();

private:
	void loadAudio(QString txt);

private:
	QGridLayout* dianosLayout;

	QMap<QString, Key> dictKey;
	QList<QPushButton* > btnTXT;
	QMap<int, QPushButton*> listButton;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};