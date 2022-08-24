#pragma once

#include <QWidget>

#include "Key.h"

class TalkController : public QWidget
{
	Q_OBJECT

public:
	TalkController(QWidget* parent);
	~TalkController();

private slots:
	void HomeClick();
	void FirstPageClick();
	void PreviousPageClick();
	void NextPageClick();
	void LastPageClick();
	void Delete();
	void Keyboard();
	void Typing();
	void chooseClick();
	void displayPage();

private:
	void addHistory(QString, QString);
	void saveHistoryToFile();
	void loadAudio(QString txt);

private:
	int dem;
	int demPage ;
	int MAX_TALK ;
	int count;
	int maxPage;
	int choose;
	//QPushButton* btndau[5];
   // QString* bangdau[200];
	//QString* strTXT = new QString[10];
	QGridLayout* talkLayout;

	QMap<QString, Key> dictKey;
	QMap<int, QPushButton*> listButton;
	QList<QPushButton* > btnTXT;
	QMap<QString, QPushButton*> listKeyButton;
	QList<Key> keys;
	QList<QString> listKey;
	QMap<QString, QString> mapKeyToName;
	QList<QMap<QString, QString>> talkHistory;
};