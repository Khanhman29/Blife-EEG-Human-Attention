#pragma once
#include <QWidget>

#include "Key.h"

class TvController : public QWidget
{
	Q_OBJECT
public:
	TvController(QWidget* parent);
	~TvController();
	void SetId(int id);

private slots:
	void homeClick();
	void onClick();
	void offClick();
	void nextChannelClick();
	void prevChannelClick();
	void volumeUpClick();
	void volumeDownClick();
	void numClick();

private:
	QGridLayout* tvLayout;

	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};

