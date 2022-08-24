#pragma once
#include <QWidget>

#include "Key.h"

class LightController : public QWidget
{
	Q_OBJECT
public:
	LightController(QWidget* parent);
	~LightController();
	void SetId(int id);

private slots:
	void homeClick();
	void onClick();
	void offClick();

private:
	QGridLayout* lightLayout;

	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};

