#include "DictionaryController.h"
#include "KeyboardController.h"
DictionaryController::DictionaryController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "dictionaryLayout";
	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* dictionaryWidget = new QWidget;
	mainLayout->addWidget(dictionaryWidget);

	dictionaryLayout = new QGridLayout(dictionaryWidget);
	dictionaryWidget->setLayout(dictionaryLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("DictionaryConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	//read file history
	demPage = 0;
	MAX_DICT = 10;
	choose = 0;
	QFile fileHistory("dictionaryHistory.csv");
	if (fileHistory.open(QIODevice::ReadOnly)) {
		QString data;
		data = fileHistory.readAll();
		dictionaryHistory = read::ReadHistory(data);
	}
	for (int i = 0; i < dataconfig.count(); i++) {

		KeyData keydata(dataconfig[i]);
		Key key;
		key.keyData = keydata;
		QPushButton* btn = new QPushButton();
		//btn->setText(keydata.name);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet(BTN_QSS);
		if (key.keyData.name == "Home") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(HomeClick()));
			//listKeyButton["Home"] = btn;
		}
		else if (key.keyData.name == "FirstPage") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(FirstPageClick()));
			//listKeyButton["FirstPage"] = btn;
		}
		else if (key.keyData.name == "PreviousPage") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(PreviousPageClick()));
			//listKeyButton["Previous"] = btn;
		}
		else if (key.keyData.name == "NextPage") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(NextPageClick()));
			//listKeyButton["NextPageClick"] = btn;
		}
		else if (key.keyData.name == "LastPage") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(LastPageClick()));
			//listKeyButton["LastPageClick"] = btn;
		}
		else if (key.keyData.name == "Delete") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(Delete()));
			//listKeyButton["DeletePageClick"] = btn;
		}
		else if (key.keyData.name == "Keyboard") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(Keyboard()));
			//listKeyButton["Keyboard"] = btn;
			//connect(btn, SIGNAL(clicked()), this, SLOT(clearClick()));
			//listKeyButton["Clear"] = btn;
		}
		else if (key.keyData.name == "Add") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(Add()));
		}
		else if(key.keyData.name.contains("txt")) {
			btnTXT.append(btn);
			connect(btn, SIGNAL(clicked()), this, SLOT(chooseClick()));
		}
		else {
			btnAcronym.append(btn);
			connect(btn, SIGNAL(clicked()), this, SLOT(chooseClick()));
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		dictionaryLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

		// listButton
		for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
			for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
				listButton[keydata.pos + exPosX + COL * exPosY] = btn;
				mapKeyToName[QString::number(keydata.pos + exPosX + COL * exPosY)] = key.keyData.name;
			}
		}

		dictKey[keydata.name] = key;

	}

	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;

	for (int c = 0; c < dictionaryLayout->columnCount(); c++) {
		dictionaryLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < dictionaryLayout->rowCount(); r++) {
		dictionaryLayout->setRowStretch(r, 1);
	}

	displayPage();
}

DictionaryController::~DictionaryController()
{
}
void DictionaryController::HomeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["mainMenuLayout"]);
	CURRENT_WIDGET = "mainMenuLayout";
}
void DictionaryController::Add() {
	if (DICTIONARY_CHOOSE_TYPE == 2 || DICTIONARY_CHOOSE_TYPE == 1) {
		PREVIOUS_WIDGET = "dictionaryLayout";
		KeyboardController* keyboardWidget = new KeyboardController(this);
		stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
		stackedWidgets->addWidget(keyboardWidget);
		LIST_WIDGET_MAP[CURRENT_WIDGET] = keyboardWidget;
		stackedWidgets->setCurrentWidget(keyboardWidget);
	}
}
void DictionaryController::Keyboard() {
	PREVIOUS_WIDGET = "dictionaryLayout";
	KeyboardController* keyboardWidget = new KeyboardController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(keyboardWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = keyboardWidget;
	stackedWidgets->setCurrentWidget(keyboardWidget);
}
void DictionaryController::displayPage() {
	QString strTXT;
	QString strAcronym;
	for (int i = 0; i < MAX_DICT; i++) {
		int t = demPage * MAX_DICT + i;
		//qDebug() << "t" << t;
		if (t >= 0 && t < dictionaryHistory.count()) {
			strAcronym = dictionaryHistory.at(t)["Acronym"];
			strTXT = dictionaryHistory.at(t)["Text"];
			btnAcronym[i]->setText(strAcronym);
			btnTXT[i]->setText(strTXT);
		}
		else {
			btnAcronym[i]->setText("");
			btnTXT[i]->setText("");
		}
	}
}
void DictionaryController::NextPageClick() {
	int countHistory = dictionaryHistory.length();
	int maxPage = countHistory / 10;
	if (demPage < maxPage) {
		demPage++;
	}
	else {
		demPage = 0;
	}

	displayPage();

}
void DictionaryController::FirstPageClick() {
	demPage = 0;
	displayPage();
}
void DictionaryController::PreviousPageClick() {
	if (demPage > 0) {
		demPage--;
	}
	displayPage();
}
void DictionaryController::LastPageClick() {
	int countHistory = dictionaryHistory.count();
	maxPage = countHistory / 10;
	demPage = maxPage;
	displayPage();
}
void DictionaryController::chooseClick() {
	QObject* obj = sender();
	for (int i = 0; i < 10; i++) {
		if (obj == btnTXT[i] ) {
			choose = i;
			DICTIONARY_CHOOSE = i;
			DICTIONARY_CHOOSE_TYPE = 1;
			DICTIONARY_LINE_TEXT = btnTXT[i]->text();
		}
		else if (obj == btnAcronym[i]) {
			DICTIONARY_CHOOSE = i;
			choose = i;
			DICTIONARY_CHOOSE = i;
			DICTIONARY_CHOOSE_TYPE = 2;
			DICTIONARY_LINE_TEXT = btnAcronym[i]->text();
		}
	}
}


// đoạn này thêm slot button add click vào trong đấy gọi cái addHistory

void DictionaryController::addHistory(QString time, QString data) {
	QMap<QString, QString> entry;
	entry["Acronym"] = time;
	entry["Text"] = data;
	dictionaryHistory.append(entry);
	saveHistoryToFile();
}

void DictionaryController::Delete() {
	int xoa = demPage * MAX_DICT + choose;
	dictionaryHistory.removeAt(xoa);
	saveHistoryToFile();
	displayPage();
}

void DictionaryController::saveHistoryToFile() {
	QFile file("dictionaryHistory.csv");
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out.setCodec("UTF-8"); // ...
	out.setGenerateByteOrderMark(false);
	out << "Acronym" << "," << "Text" << "\n";
	for (int i = 0; i < dictionaryHistory.length(); i++) {
		QString text = dictionaryHistory[i]["Acronym"] + "," + dictionaryHistory[i]["Text"] + "\n";
		out << text;
	}
	file.close();
}