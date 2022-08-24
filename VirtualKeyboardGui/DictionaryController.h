#pragma once

#include <QWidget>

#include "Key.h"

class DictionaryController : public QWidget
{
	Q_OBJECT

public:
	DictionaryController(QWidget* parent);
	~DictionaryController();

private slots:
	void HomeClick();
	void FirstPageClick();
	void PreviousPageClick();
	void NextPageClick();
	void LastPageClick();
	void Delete();
	void Keyboard();
	void Add();
	void chooseClick();
	void displayPage();

private:
	void addHistory(QString, QString);
	void saveHistoryToFile();

private:
	int dem;
	int demPage;
	int MAX_DICT;
	int count;
	int maxPage;
	int choose;
	//QPushButton* btndau[5];
   // QString* bangdau[200];
	//QString* strTXT = new QString[10];
	QGridLayout* dictionaryLayout;

	QMap<QString, Key> dictKey;
	QMap<int, QPushButton*> listButton;
	QList<QPushButton* > btnTXT;
	QList<QPushButton* > btnAcronym;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
	QList<QMap<QString, QString>> dictionaryHistory;
};