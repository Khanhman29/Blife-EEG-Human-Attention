#pragma once
#include <QWidget>

#include "Key.h"

class AcController : public QWidget
{
	Q_OBJECT
public:
	AcController(QWidget* parent);
	~AcController();
	void SetId(int id);

private slots:
	void homeClick();
	void onClick();
	void offClick();
	void firstLevelClick();
	void secondLevelClick();
	void thirdLevelClick();
	void tempUpClick();
	void tempDownClick();
	void swingClick();

private:
	QGridLayout* acLayout;

	QMap<QString, Key> dictKey;
	QMap<QString, QPushButton*> listKeyButton; // map key name to button
	QMap<int, QPushButton*> listButton;
	QMap<QString, QString> mapKeyToName; // map pos of button to key name
};

