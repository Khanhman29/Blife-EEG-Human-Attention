#pragma once
#include <QWidget>

#include "Key.h"

class FanController : public QWidget
{
	Q_OBJECT
public:
	FanController(QWidget* parent);
	~FanController();
	void SetId(int id);

private slots:
	void homeClick();
	void onClick();
	void offClick();
	void firstLevelClick();
	void secondLevelClick();
	void thirdLevelClick();

private:
	QGridLayout* fanLayout;

	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};

