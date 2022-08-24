#include "CalibController.h"
#include "TrainingController.h"
#include "read.h"

CalibController::CalibController(QWidget *parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "calibLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* calibWidget = new QWidget;
	mainLayout->addWidget(calibWidget);
	
	calibLayout = new QGridLayout(calibWidget);
	calibWidget->setLayout(calibLayout);
	
	// read file config
	QFile file("CalibConfig.csv");
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);
		
		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE["calibLayout"]["row"] = ROW;
		WIDGET_CONFIG_SIZE["calibLayout"]["col"] = COL;
	}

	for (int i = 0; i < dataconfig.count(); i++) {
		KeyData keydata(dataconfig[i]);
		Key key;
		key.keyData = keydata;
		QPushButton* btn = new QPushButton();
		btn->setText(keydata.key);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet("QPushButton{font-size: 40px;}");

		if (keydata.name == "Home") {
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
		}
		else if (keydata.name == "calib") {
			connect(btn, SIGNAL(clicked()), this, SLOT(calibClick()));
		}
		else if (keydata.name == "train") {
			connect(btn, SIGNAL(clicked()), this, SLOT(trainClick()));
		}
		else if (keydata.name == "disp") {
			btn->setEnabled(false);
			key.isHover = false;
		}
		else {
			connect(btn, SIGNAL(clicked()), this, SLOT(click()));
		}

		int x = keydata.pos / COL;
		int y = keydata.pos - (COL * x);
		calibLayout->addWidget(btn, x, y, keydata.spanY, keydata.spanX);

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

	//for (int i = 0; i < listButton.count(); i++) {
	//	qDebug() << i << " : " << listButton[i]->text();
	//}

	for (int c = 0; c < calibLayout->columnCount(); c++) {
		calibLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < calibLayout->rowCount(); r++) {
		calibLayout->setRowStretch(r, 1);
	}
}

CalibController::~CalibController()
{
}

void CalibController::click() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	qDebug() << "here: " << btn->text();
	
}

void CalibController::homeClick() {
	PREVIOUS_WIDGET = "mainMenuLayout";
	stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP[PREVIOUS_WIDGET]);
	swap(PREVIOUS_WIDGET, CURRENT_WIDGET);
}

void CalibController::calibClick() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (error != TOBII_ERROR_NO_ERROR) {
		btn->setText("Không có kết nối!");

	}
	else {
		system("\"C:\\Program Files (x86)\\Tobii\\Tobii EyeX Config\\Tobii.EyeX.Configuration.exe\" -Q");
	}
}

void CalibController::trainClick() {
	TRAINING_LEVEL = 1;
	PREVIOUS_WIDGET = CURRENT_WIDGET;
	CURRENT_WIDGET = "trainingLayout";

	LIST_BUTTON_MAP[CURRENT_WIDGET].clear();
	LIST_DICT_KEY[CURRENT_WIDGET].clear();
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET].clear();

	TrainingController* trainingWidget = new TrainingController(this);

	stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
	stackedWidgets->addWidget(trainingWidget);
	stackedWidgets->setCurrentWidget(trainingWidget);

	LIST_WIDGET_MAP[CURRENT_WIDGET] = trainingWidget;
}

void CalibController::HDLapDatClick() {

}

void CalibController::HDSuDungClick() {

}