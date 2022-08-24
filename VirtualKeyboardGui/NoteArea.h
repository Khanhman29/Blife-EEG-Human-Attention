#pragma once

#include <QWidget>

#include "Key.h"

class NoteArea : public QWidget
{
	Q_OBJECT

public:
	NoteArea(QWidget* parent);
	~NoteArea();

private slots:
	void on_clicked();

private:
	//QPushButton* btndau[5];
   // QString* bangdau[200];
	//QString* strTXT = new QString[10];
	QGridLayout* noteAreaLayout;
	QLineEdit* lineEdit;
	QMap<QString, Key> dictKey;
	QMap<QString, QList<Key>> keyFrom;
	QMap<int, QPushButton*> listButton;
	QMap<QString, QPushButton*> listKeyButton;
	QMap<int, QString> normalKeyboard;
	QMap<int, QString> specialKeyboard;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};