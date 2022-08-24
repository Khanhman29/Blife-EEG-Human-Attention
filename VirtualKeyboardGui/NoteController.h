#pragma once

#include <QWidget>

#include "Key.h"

class NoteController : public QWidget
{
	Q_OBJECT

public:
	NoteController(QWidget* parent);
	~NoteController();

private slots:
	void HomeClick();
	void UpClick();
	void DownClick();
	void AddNewClick();
	void displayPage();

private:
	//QPushButton* btndau[5];
   // QString* bangdau[200];
	//QString* strTXT = new QString[10];
	QGridLayout* noteLayout;

	QMap<QString, Key> dictKey;
	QList<QPushButton* > btnNote;
	QMap<int, QPushButton*> listButton;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
};