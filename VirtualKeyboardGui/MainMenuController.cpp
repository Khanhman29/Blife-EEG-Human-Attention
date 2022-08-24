#include "MainMenuController.h"
#include "TypingController.h"
#include "SmartHomeController.h"
#include "TalkController.h"
#include "DictionaryController.h"
#include "NoteController.h"
#include "DianosController.h"
#include "GreetingsController.h"
#include "QAController.h"
#include "SupportController.h"

MainMenuController::MainMenuController(QWidget *parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "mainMenuLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* mainMenuWidget = new QWidget;
	mainLayout->addWidget(mainMenuWidget);

	mainMenuLayout = new QGridLayout(mainMenuWidget);
	mainMenuWidget->setLayout(mainMenuLayout);

	// read file config
	QList<QMap<QString, QString>> dataconfig;
	QFile file("MainMenuConfig.csv");
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
		Key newKey;
		newKey.keyData = keydata;
		QPushButton* btn = new QPushButton();
		//btn->setText(keydata.name);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet(BTN_QSS);

		listKeyButton[newKey.keyData.name] = btn;

		if (keydata.name == "helpButton") {
			btnLayout.append(btn);
			btn->setText("Help");
			btn->setEnabled(false);
			newKey.isHover = false;
			connect(btn, SIGNAL(clicked()), this, SLOT(helpClick()));
		}
		else if (keydata.name == "dianosButton") {
			btnLayout.append(btn);
			btn->setText("Dianos");
			connect(btn, SIGNAL(clicked()), this, SLOT(dianosClick()));
		}
		else if (keydata.name == "dictionaryButton") {
			btnLayout.append(btn);
			btn->setText("Dictionary");
			connect(btn, SIGNAL(clicked()), this, SLOT(dictionaryClick()));
		}
		else if (keydata.name == "lockButton") {
			btn->setText("Lock");
			connect(btn, SIGNAL(clicked()), this, SLOT(lockClick()));
		}
		else if (keydata.name == "greetingsButton") {
			btnLayout.append(btn);
			btn->setText("Greetings");
			connect(btn, SIGNAL(clicked()), this, SLOT(greetingsClick()));
		}
		else if (keydata.name == "qaButton") {
			btnLayout.append(btn);
			btn->setText("QA");
			connect(btn, SIGNAL(clicked()), this, SLOT(qaClick()));
		}
		else if (keydata.name == "supportButton") {
			btnLayout.append(btn);
			btn->setText("Support");
			connect(btn, SIGNAL(clicked()), this, SLOT(supportClick()));
		}
		else if (keydata.name == "talkButton") {
			btnLayout.append(btn);
			btn->setText("Talk");
			connect(btn, SIGNAL(clicked()), this, SLOT(talkClick()));
		}
		else if (keydata.name == "noteButton") {
			btnLayout.append(btn);
			btn->setText("Notes");
			btn->setEnabled(false);
			//connect(btn, SIGNAL(clicked()), this, SLOT(noteClick()));
		}
		else if (keydata.name == "smarthomeButton") {
			btnLayout.append(btn);
			btn->setText("Smart Home");
			connect(btn, SIGNAL(clicked()), this, SLOT(smartHomeClick()));
		}
		else if (keydata.name == "typeButton") {
			btnLayout.append(btn);
			btn->setText("Typing");
			connect(btn, SIGNAL(clicked()), this, SLOT(typingClick()));
		}
		else if (keydata.name == "browserButton") {
			btn->setText("Browser");
			btn->setEnabled(false);
			newKey.isHover = false;
			connect(btn, SIGNAL(clicked()), this, SLOT(browserClick()));
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		mainMenuLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

		// listButton
		for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
			for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
				listButton[keydata.pos + exPosX + COL * exPosY] = btn;
			mapKeyToName[QString::number(keydata.pos + exPosX + COL * exPosY)] = newKey.keyData.name;
			}
		}

		dictKey[keydata.name] = newKey;
	}

	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;

	for (int c = 0; c < mainMenuLayout->columnCount(); c++) {
		mainMenuLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < mainMenuLayout->rowCount(); r++) {
		mainMenuLayout->setRowStretch(r, 1);
	}
}

MainMenuController::~MainMenuController()
{
}

void MainMenuController::helpClick() {

}

void MainMenuController::talkClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	TalkController* talkWidget = new TalkController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(talkWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = talkWidget;
	stackedWidgets->setCurrentWidget(talkWidget);
}

void MainMenuController::dictionaryClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	DictionaryController* dictionaryWidget = new DictionaryController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(dictionaryWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = dictionaryWidget;
	stackedWidgets->setCurrentWidget(dictionaryWidget);
}

void MainMenuController::noteClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	NoteController* noteWidget = new NoteController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(noteWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = noteWidget;
	stackedWidgets->setCurrentWidget(noteWidget);
}

void MainMenuController::browserClick() {

}

void MainMenuController::smartHomeClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	SmartHomeController* smarthomeWidget = new SmartHomeController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(smarthomeWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = smarthomeWidget;
	stackedWidgets->setCurrentWidget(smarthomeWidget);
}

void MainMenuController::socialNetworkClick() {

}

void MainMenuController::lockClick() {

	/*for (int i = 0; i < 10; i++) {
		if (lock == false) {
			btnLayout[i]->setDisabled(true);
			lock = true;
		}
		else if (lock == true) {
			btnLayout[i]->setDisabled(true);
			lock = true;
		}	
		//newKey.isHover = false;
	}*/
}

void MainMenuController::greetingsClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	GreetingsController* greetingsWidget = new GreetingsController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(greetingsWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = greetingsWidget;
	stackedWidgets->setCurrentWidget(greetingsWidget);
}

void MainMenuController::qaClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	QAController* qaWidget = new QAController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(qaWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = qaWidget;
	stackedWidgets->setCurrentWidget(qaWidget);
}

void MainMenuController::supportClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	SupportController* supportWidget = new SupportController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(supportWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = supportWidget;
	stackedWidgets->setCurrentWidget(supportWidget);
}

void MainMenuController::dianosClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	DianosController* dianosWidget = new DianosController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(dianosWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = dianosWidget;
	stackedWidgets->setCurrentWidget(dianosWidget);
}

void MainMenuController::typingClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	TypingController* typingWidget = new TypingController(this);
	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(typingWidget);
	LIST_WIDGET_MAP[CURRENT_WIDGET] = typingWidget;

	stackedWidgets->setCurrentWidget(typingWidget);
}