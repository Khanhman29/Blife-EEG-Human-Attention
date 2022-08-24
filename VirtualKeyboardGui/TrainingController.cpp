#include "TrainingController.h"
#include "TypingController.h"

TrainingController::TrainingController(QWidget *parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "trainingLayout";
	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* trainingWidget = new QWidget;
	mainLayout->addWidget(trainingWidget);

	trainingLayout = new QGridLayout(trainingWidget);
	trainingWidget->setLayout(trainingLayout);

	// read file config
	QString trainingLevel = "Training" + QString::number(TRAINING_LEVEL) + "Config.csv";
	QList<QMap<QString, QString>> dataconfig;
	QFile file(trainingLevel);
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	btKey.clear();
	
	for (int i = 0; i < dataconfig.count(); i++) {
		KeyData keydata(dataconfig[i]);
		Key key;
		key.keyData = keydata;
		QPushButton* btn = new QPushButton();
		btn->setText(keydata.key);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet(BTN_QSS);

		if (key.keyData.name == "Home") {
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
		}
		else if (key.keyData.name.contains("BT")) {
			connect(btn, SIGNAL(clicked()), this, SLOT(btClick()));
			key.keyData.temp = 0;
			key.keyData.temp1 = btCount;
			btKey.append(key);
			btCount++;
			MapKeyNameToBT[key.keyData.name] = btn;
		}
		else if (key.keyData.name == "Next") {
			connect(btn, SIGNAL(clicked()), this, SLOT(nextClick()));
			//key.isHover = false;
			btn->setEnabled(false);
			MapKeyNameToBT[key.keyData.name] = btn;
			next = key;
		}
		else {
			btn->setEnabled(false);
			key.isHover = false;
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		trainingLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

		// listButton
		for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
			for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
				listButton[keydata.pos + exPosX + COL * exPosY] = btn;
			}
		}

		dictKey[keydata.name] = key;
		mapKeyToName[key.keyData.key] = key.keyData.name;
	}

	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;

	createSelOder();
	displayBtKeys();

	for (int c = 0; c < trainingLayout->columnCount(); c++) {
		trainingLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < trainingLayout->rowCount(); r++) {
		trainingLayout->setRowStretch(r, 1);
	}
}

TrainingController::~TrainingController()
{
}

void TrainingController::homeClick() {
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP[PREVIOUS_WIDGET]);
	swap(PREVIOUS_WIDGET, CURRENT_WIDGET);
}

void TrainingController::btClick() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	Key key = dictKey[mapKeyToName[btn->text()]];
	if (key.keyData.temp1 == selected) {
		key.keyData.temp = 1;
		dictKey[key.keyData.name] = key;

		createSelOder();
		displayBtKeys();
	}
}

void TrainingController::createSelOder() {
	QList<int> leftBT;
	for (int i = 0; i < btKey.count(); i++) {
		Key key = dictKey[btKey[i].keyData.name];
		if (key.keyData.temp == 0) {
			leftBT.append(i);
		}
	}

	if (leftBT.count() > 0) {
		if (leftBT.count() == 1) {
			selected = leftBT[0];
		}
		else {
			int r = QRandomGenerator::global()->bounded(0, leftBT.count() - 1);
			selected = leftBT[r];
		}
	}
	else {
		selected = -1;
	}

	if (selected == -1 && TRAINING_LEVEL < 12) {
		next.isHover = true;
	}
}

void TrainingController::displayBtKeys() {
	int i;
	for (i = 0; i < btKey.count(); i++) {
		QPushButton* btn = MapKeyNameToBT[btKey[i].keyData.name];
		Key key = dictKey[btKey[i].keyData.name];
		if (key.keyData.temp == 1) {
			// Disable key
			key.isHover = false;
			MapKeyNameToBT[key.keyData.name]->setEnabled(false);
		}
		if (i == selected && (TRAINING_LEVEL < 8 || TRAINING_LEVEL == 12)) {
			key.color = "blue";
			key.setColor(btn, "40");
		}
		else {
			key.color = "black";
			key.setColor(btn, "40");
		}
		dictKey[key.keyData.name] = key;
	}
	if (TRAINING_LEVEL < 12) {
		if (selected != -1) {
			QPushButton* nextBT = MapKeyNameToBT[next.keyData.name];
			next.color = "black";
			nextBT->setEnabled(false);
			next.setColor(nextBT, "40");
		}
		else if (next.isHover == true && selected == -1) {
			QPushButton* nextBT = MapKeyNameToBT[next.keyData.name];
			next.color = "blue";
			nextBT->setEnabled(true);
			next.setColor(nextBT, "40");
		}
		dictKey[next.keyData.name] = next;
		LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	}
}

void TrainingController::nextClick() {
	TRAINING_LEVEL += 1;
	if (selected == -1) {
		if (TRAINING_LEVEL < 8) {
			LIST_BUTTON_MAP[CURRENT_WIDGET].clear();
			LIST_DICT_KEY[CURRENT_WIDGET].clear();
			LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET].clear();

			TrainingController* nextLayout = new TrainingController(this);
	
			stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
			stackedWidgets->addWidget(nextLayout);
			stackedWidgets->setCurrentWidget(nextLayout);
	
			LIST_WIDGET_MAP[CURRENT_WIDGET] = nextLayout;
		}
		else {
			// load next training
			CURRENT_WIDGET = "typingLayout";
			LIST_BUTTON_MAP[CURRENT_WIDGET].clear();
			LIST_DICT_KEY[CURRENT_WIDGET].clear();
			LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET].clear();
			LIST_KEY_FROM[CURRENT_WIDGET].clear();

			TypingController* nextTranining = new TypingController(this);

			stackedWidgets->removeWidget(LIST_WIDGET_MAP["typingLayout"]);
			stackedWidgets->addWidget(nextTranining);
			stackedWidgets->setCurrentWidget(nextTranining);

			LIST_WIDGET_MAP[CURRENT_WIDGET] = nextTranining;
		}
	}
}