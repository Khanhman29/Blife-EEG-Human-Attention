#include "TalkController.h"
#include "TypingController.h"
#include "KeyboardController.h"
TalkController::TalkController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "talkLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* talkWidget = new QWidget;
	mainLayout->addWidget(talkWidget);

	talkLayout = new QGridLayout(talkWidget);
	talkWidget->setLayout(talkLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("TalkConfig.csv");
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
	MAX_TALK = 10;
	choose = 0;
	QFile fileHistory("talkHistory.csv");
	if (fileHistory.open(QIODevice::ReadOnly)) {
		QString data;
		data = fileHistory.readAll();
		talkHistory = read::ReadHistory(data);
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
		else if (key.keyData.name == "Typing") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(Typing()));
			//listKeyButton["Typing"] = btn;
			//connect(btn, SIGNAL(clicked()), this, SLOT(clearClick()));
			//listKeyButton["Clear"] = btn;
		}
		else {
			btnTXT.append(btn);
			connect(btn, SIGNAL(clicked()), this, SLOT(chooseClick()));
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		talkLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < talkLayout->columnCount(); c++) {
		talkLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < talkLayout->rowCount(); r++) {
		talkLayout->setRowStretch(r, 1);
	}

	displayPage();
}

TalkController::~TalkController()
{
}
void TalkController::HomeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["mainMenuLayout"]);
	CURRENT_WIDGET = "mainMenuLayout";
}
void TalkController::Typing() {
	PREVIOUS_WIDGET = "talkLayout";
	TypingController* typingWidget = new TypingController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(typingWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = typingWidget;
	stackedWidgets->setCurrentWidget(typingWidget);
}
void TalkController::Keyboard() {
	PREVIOUS_WIDGET = "talkLayout";
	KeyboardController* keyboardWidget = new KeyboardController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(keyboardWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = keyboardWidget;
	stackedWidgets->setCurrentWidget(keyboardWidget);
}
void TalkController::displayPage() {
	QString strTXT;

	for (int i = 0; i < MAX_TALK; i++) {
		int t = demPage * MAX_TALK + i;
		qDebug() << "t" << t;
		if (t >= 0 && t < talkHistory.count()) {
			strTXT = talkHistory.at(t)["Content"];
			btnTXT[i]->setText(strTXT);
		}
		else {
			btnTXT[i]->setText("");
		}
	}
}
void TalkController::NextPageClick() {
	int countHistory = talkHistory.length();
	int maxPage = countHistory / 10;

	if (demPage < maxPage) {
		demPage++;
	}
	else {
		demPage = 0;
	}

	displayPage();
	
}
void TalkController::FirstPageClick() {
	demPage = 0;
	displayPage();
}
void TalkController::PreviousPageClick() {
	if (demPage > 0) {
		demPage--;
	}
	displayPage();
}
void TalkController::LastPageClick() {
	int countHistory = talkHistory.count();
	maxPage = countHistory / 10;
	demPage = maxPage;
	displayPage();
}
void TalkController::chooseClick() {
	QObject* obj = sender();
	for (int i = 0; i < 10; i++) {
		if (obj == btnTXT[i]) {
			loadAudio(btnTXT[i]->text());
			choose = i;
		}
	}
}


// đoạn này thêm slot button add click vào trong đấy gọi cái addHistory

void TalkController::addHistory(QString time, QString data) {
	QMap<QString, QString> entry;
	entry["Time"] = time;
	entry["Content"] = data;
	talkHistory.append(entry);
	saveHistoryToFile();
}

void TalkController::Delete() {
	int xoa = demPage * MAX_TALK + choose;
	talkHistory.removeAt(xoa);
	saveHistoryToFile();
	displayPage();
}

void TalkController::saveHistoryToFile() {
	QFile file("talkHistory.csv");
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);
	out.setCodec("UTF-8"); // ...
	out.setGenerateByteOrderMark(false);
	out << "Time" << "," << "Content" << "\n";
	for (int i = 0; i < talkHistory.length(); i++) {
		QString text = talkHistory[i]["Time"] + "," + talkHistory[i]["Content"] + "\n";
		out << text;
	}
	file.close();
}

void TalkController::loadAudio(QString txt)
{
	Key::playAudio(txt);
}
