#pragma once

#include <QObject>
//#include "Controller.h"
#include "Key.h"

class CalibController : public QWidget
{
	Q_OBJECT

public:
	CalibController(QWidget *parent);
	~CalibController();

private slots:
	void homeClick();
	void calibClick();
	void trainClick();
	void HDLapDatClick();
	void HDSuDungClick();
	void click();

private:
	QGridLayout* calibLayout;
	QList<QMap<QString, QString>> dataconfig;

	QMap<QString, Key> dictKey;
	QMap<int, QPushButton*> listButton;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};
