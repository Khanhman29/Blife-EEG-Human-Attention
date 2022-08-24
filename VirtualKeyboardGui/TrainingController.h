#pragma once

#include <QWidget>
#include "Key.h"

class TrainingController : public QWidget
{
	Q_OBJECT

public:
	TrainingController(QWidget *parent);
	~TrainingController();

private slots:
	void homeClick();
	void btClick();
	void nextClick();

private:
	int btCount = 0;
	int selected = 0;
	QList<int> selOrder;
	QList<Key> btKey;
	Key next;
	QMap<QString, QPushButton*> MapKeyNameToBT;

	QGridLayout* trainingLayout;
	QStackedWidget* trainingStackedWidgets;

	QMap<QString, Key> dictKey;
	QMap<int, QPushButton*> listButton;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
	
	void createSelOder();
	void displayBtKeys();
};
