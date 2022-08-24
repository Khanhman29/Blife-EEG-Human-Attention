#include "NoteController.h"
#include "TypingController.h"
#include "KeyboardController.h"
#include "NoteArea.h"
#include "Notes.h"

NoteController::NoteController(QWidget* parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "noteLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* noteWidget = new QWidget;
	mainLayout->addWidget(noteWidget);

	noteLayout = new QGridLayout(noteWidget);
	noteWidget->setLayout(noteLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("NoteConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
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
		else if (key.keyData.name == "Up") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(UpClick()));
			//listKeyButton["FirstPage"] = btn;
		}
		else if (key.keyData.name == "Down") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(DownClick()));
			//listKeyButton["Previous"] = btn;
		}
		else if (key.keyData.name == "AddNew") {
			btn->setText(key.keyData.key);
			connect(btn, SIGNAL(clicked()), this, SLOT(AddNewClick()));
			//listKeyButton["NextPageClick"] = btn;
		}
		else {
			btnNote.append(btn);
			connect(btn, SIGNAL(clicked()), this, SLOT(chooseClick()));
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		noteLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

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

	for (int c = 0; c < noteLayout->columnCount(); c++) {
		noteLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < noteLayout->rowCount(); r++) {
		noteLayout->setRowStretch(r, 1);
	}

	displayPage();
}

NoteController::~NoteController()
{
}
void NoteController::HomeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP["mainMenuLayout"]);
	CURRENT_WIDGET = "mainMenuLayout";
}
void NoteController::UpClick() {
	
}
void NoteController::DownClick() {
}
void NoteController::AddNewClick() {
	PREVIOUS_WIDGET = "noteLayout";
	NoteArea* noteAreaWidget = new NoteArea(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(noteAreaWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = noteAreaWidget;
	stackedWidgets->setCurrentWidget(noteAreaWidget);
}
void NoteController::displayPage() {
}
