#pragma once

#include <QWidget>

#include "Key.h"

class QAController : public QWidget
{
	Q_OBJECT

public:
	QAController(QWidget* parent);
	~QAController();

private slots:
	void HomeClick();
	void LockButtonClick();
	void chooseClick();
private:
	void loadAudio(QString txt);

private:
	QGridLayout* qaLayout;

	QMap<QString, Key> dictKey;
	QList<QPushButton* > btnTXT;
	QMap<int, QPushButton*> listButton;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};