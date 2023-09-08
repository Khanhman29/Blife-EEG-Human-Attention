#include "TypingController.h"
#include "TrainingController.h"
#include "TalkController.h"

TypingController::TypingController(QWidget *parent)
	: QWidget(parent)
{
	CURRENT_WIDGET = "typingLayout";

	// tao widget
	QGridLayout* mainLayout = new QGridLayout(this);
	this->setLayout(mainLayout);

	QWidget* typingWidget = new QWidget;
	mainLayout->addWidget(typingWidget);

	typingLayout = new QGridLayout(typingWidget);
	typingWidget->setLayout(typingLayout);

	// read file config
	QFile file("TypingConfig.csv");
	QList<QMap<QString, QString>> dataconfig;
	if (file.open(QIODevice::ReadOnly)) {
		QString data;
		data = file.readAll();
		dataconfig = read::Read(data);

		ROW = dataconfig[0]["numOfRow"].toInt();
		COL = dataconfig[0]["numOfCol"].toInt();

		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["row"] = ROW;
		WIDGET_CONFIG_SIZE[CURRENT_WIDGET]["col"] = COL;
	}

	readDau();
	readVan();

	for (int i = 0; i < dataconfig.count(); i++) {
		KeyData keydata(dataconfig[i]);
		Key newKey;
		newKey.keyData = keydata;

		QPushButton* btn = new QPushButton();
		btn->setText(keydata.key);
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		btn->setStyleSheet("QPushButton{font-size: 40px;}");
		
		if (newKey.keyData.name == "Home") {
			btn->setText("⏎");
			connect(btn, SIGNAL(clicked()), this, SLOT(homeClick()));
			listKeyButton["Home"] = btn;
		}
		else if (newKey.keyData.name == "Prev") {
			connect(btn, SIGNAL(clicked()), this, SLOT(prevClick()));
			listKeyButton["Prev"] = btn;
		}
		else if (newKey.keyData.name == "Next") {
			next = newKey;
			connect(btn, SIGNAL(clicked()), this, SLOT(nextClick()));
			listKeyButton["Next"] = btn;
		}
		else if (newKey.keyData.name == "Words") {
			words = newKey;
			connect(btn, SIGNAL(clicked()), this, SLOT(wordsClick()));
			listKeyButton["Words"] = btn;
		}
		else if (newKey.keyData.name == "Space") {	 
			space = newKey;
			connect(btn, SIGNAL(clicked()), this, SLOT(spaceClick()));
			listKeyButton["Space"] = btn;
		}
		else if (newKey.keyData.name == "Delete") {
			del = newKey;
			connect(btn, SIGNAL(clicked()), this, SLOT(deleteClick()));
			listKeyButton["Delete"] = btn;
		}
		else if (newKey.keyData.name == "Clear") {
			//connect(btn, SIGNAL(clicked()), this, SLOT(clearClick()));
			//listKeyButton["Clear"] = btn;
		}
		else if (newKey.keyData.name.contains("TXT")) {
			keys.append(newKey);
			connect(btn, SIGNAL(clicked()), this, SLOT(txtClick()));
			listKeyButton[newKey.keyData.name] = btn;
		}
		else if (newKey.keyData.name == "timecount") {
			words = newKey;
			connect(btn, SIGNAL(clicked()), this, SLOT(timecount()));
			listKeyButton["timecount"] = btn;
		}

		int r = keydata.pos / COL;
		int c = keydata.pos - (COL * r);
		typingLayout->addWidget(btn, r, c, keydata.spanY, keydata.spanX);

		// listButton
		for (int exPosX = 0; exPosX < keydata.spanX; exPosX++) {
			for (int exPosY = 0; exPosY < keydata.spanY; exPosY++) {
				listButton[keydata.pos + exPosX + COL * exPosY] = btn;
				mapKeyToName[QString::number(keydata.pos + exPosX + COL * exPosY)] = keydata.name;
			}
		}

		// dictKey
		dictKey[newKey.keyData.name] = newKey;

		// keyFrom
		if (newKey.keyData.exFrom != "") {
			if (keyFrom.contains(newKey.keyData.exFrom)) {
				keyFrom[newKey.keyData.exFrom].append(newKey);
			}
			else {
				keyFrom[newKey.keyData.exFrom].append(newKey);
			}
		}
	}

	for (int c = 0; c < typingLayout->columnCount(); c++) {
		typingLayout->setColumnStretch(c, 1);
	}
	for (int r = 0; r < typingLayout->rowCount(); r++) {
		typingLayout->setRowStretch(r, 1);
	}

	// set fixed width
	Sleep(200);
	listKeyButton["Words"]->setFixedWidth(listKeyButton["Words"]->width());

	LIST_DICT_KEY[CURRENT_WIDGET] = dictKey;
	LIST_KEY_FROM[CURRENT_WIDGET] = keyFrom;
	LIST_BUTTON_MAP[CURRENT_WIDGET] = listButton;
	LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET] = mapKeyToName;

	if (TRAINING_LEVEL >= 8 && TRAINING_LEVEL <= 11) {
		isTraining = true;
		if (TRAINING_LEVEL == 8) {
			trainingStr = "tôi đang vui";
			trainingKey = QList<QString>{ "t","tô","tôi"," ","đ","đa","đan","đang"," ","v","vu","vui","talk" };
		}
		if (TRAINING_LEVEL == 9) {
			trainingStr = "tôi khỏe";
			trainingKey = QList<QString>{ "t","tô","tôi"," ","k","kh","kho","khoe","khỏe ","talk" };
		}
		if (TRAINING_LEVEL == 10) {
			trainingStr = "chào mọi người";
			trainingKey = QList<QString>{ "c","ch","cha","chao","chào ","m","mo","moi","mọi ","n","ng","ngư","ngươ","ngươi","người ","talk" };
		}
		if (TRAINING_LEVEL == 11) {
			trainingStr = "cố gắng sẽ thành công";
			trainingKey = QList<QString>{ "c","cô","cố ","g","gă","găn","găng","gắng ","s","se","sẽ ","t","th","tha","than","thanh","thành ","c","cô","côn","công","talk" };
		}
	}

	filterBoard();
}	


TypingController::~TypingController()
{
}

void TypingController::loadAudio(QString txt) {
	Key::playAudio(txt);
}

QList<QMap<QString, QString>> TypingController::getTalkHistory() {
	QList<QMap<QString, QString>> talkHistory;
	QFile fileHistory("talkHistory.csv");
	if (fileHistory.open(QIODevice::ReadOnly)) {
		QString data;
		data = fileHistory.readAll();
		talkHistory = read::ReadHistory(data);
	}
	return talkHistory;
}

void TypingController::addHistory(QString time, QString data, QList<QMap<QString, QString>> talkHistory)
{
	QMap<QString, QString> entry;
	entry["Time"] = time;
	entry["Content"] = data;
	talkHistory.append(entry);
	saveHistorytoFile(talkHistory);
}

void TypingController::saveHistorytoFile(QList<QMap<QString, QString>> talkHistory)
{
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

void TypingController::wordsClick() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	// training
	if (isTraining && currKey < trainingKey.count()) {
		if (trainingKey[currKey] == "talk") {
			currKey++;
			if (currKey == trainingKey.count()) isTrainingFinished = true;
			displayTXTKeys();
		}
	}
	if (btn->text() != "") {
		loadAudio(btn->text());
	}
}
void TypingController::timecount() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	// training
	if (isTraining && currKey < trainingKey.count()) {
		if (trainingKey[currKey] == "talk") {
			currKey++;
			if (currKey == trainingKey.count()) isTrainingFinished = true;
			displayTXTKeys();
		}
	}
	if (btn->text() != "") {
		loadAudio(btn->text());
	}
}

void TypingController::clearClick() {
	if (isTraining) {
		currKey = 0;
	}
	listKeyButton["Words"]->setText("");
	filterBoard();
}

void TypingController::displayTXTKeys() {
	// training
	if (isTraining && isTrainingFinished) {
		for (int i = 0; i < keys.count(); i++) {
			// disable key
			QPushButton* btn = listKeyButton[keys[i].keyData.name];
			btn->setEnabled(false);
		}
		// set Disable
		listKeyButton["Space"]->setEnabled(false);
		listKeyButton["Delete"]->setEnabled(false);
		listKeyButton["Next"]->setText("Bài tiếp theo");
	}

	if (isTraining && currKey < trainingKey.count()) {
		if (trainingKey[currKey] == " ") {
			// do something
		}
		if (trainingKey[currKey] == "talk") {
			// do something
		}
	}

	for (int i = 0; i < keys.count(); i++) {
		QString str;
		int bi = board_start + i;
		if (bi < board.count()) {
			str = board[bi];
		}
		else {
			str = "";
		}
		
		QPushButton* btn = listKeyButton[keys[i].keyData.name];
		btn->setText(str);

		if (str.length() > 0 && str[str.length() - 1] == ' ') {
			if (isTraining && currKey < trainingKey.count()) {
				if (btn->text() == trainingKey[currKey]) {
					//keys[i].color = "black";
					//keys[i].setColor(btn, "40");
					btn->setEnabled(true);
				}
				else {
					keys[i].color = "black";
					keys[i].setColor(btn, "40");
					btn->setEnabled(false);
				}
			}
			else {
				keys[i].color = "blue";
				keys[i].setColor(btn, "40");
			}
		}
		else {
			if (isTraining && currKey < trainingKey.count()) {
				if (btn->text() == trainingKey[currKey]) {
					//keys[i].color = "black";
					//keys[i].setColor(btn, "40");
					btn->setEnabled(true);
				}
				else {
					keys[i].color = "black";
					keys[i].setColor(btn, "40");
					btn->setEnabled(false);
				}
			}
			else {
				keys[i].color = "black";
				keys[i].setColor(btn, "40");
			}
		}
		dictKey[keys[i].keyData.name] = keys[i];
	}
}

void TypingController::homeClick() {
	if (listKeyButton["Words"]->text() != "" ) {
		QDateTime time = QDateTime::currentDateTime();
		QString time_text = time.toString("dd/MM/yyyy") + " " + time.toString("t");
		auto talkHistory = getTalkHistory();
		addHistory(time_text, listKeyButton["Words"]->text(), talkHistory);
		TalkController* talkWidget = new TalkController(this);
		stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
		stackedWidgets->addWidget(talkWidget);
		LIST_WIDGET_MAP[CURRENT_WIDGET] = talkWidget;
		stackedWidgets->setCurrentWidget(talkWidget);
	}
	else {
		stackedWidgets->setCurrentWidget(LIST_WIDGET_MAP[PREVIOUS_WIDGET]);
		swap(PREVIOUS_WIDGET, CURRENT_WIDGET);
	}
}

void TypingController::prevClick() {
	if (board_start >= keys.count()) {
		board_start -= keys.count();
		displayTXTKeys();
	}
}

void TypingController::nextClick() {
	// training
	if (isTraining && isTrainingFinished) {
		//TRAINING_LEVEL = 11;
		if (TRAINING_LEVEL < 11) {
			TRAINING_LEVEL++;
			// load next training
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
		else {
			TRAINING_LEVEL++;
			// return main training
			CURRENT_WIDGET = "trainingLayout";
			LIST_BUTTON_MAP[CURRENT_WIDGET].clear();
			LIST_DICT_KEY[CURRENT_WIDGET].clear();
			LIST_MAP_KEY_TO_NAME[CURRENT_WIDGET].clear();

			TrainingController* nextLayout = new TrainingController(this);

			stackedWidgets->removeWidget(LIST_WIDGET_MAP[CURRENT_WIDGET]);
			stackedWidgets->addWidget(nextLayout);
			stackedWidgets->setCurrentWidget(nextLayout);

			LIST_WIDGET_MAP[CURRENT_WIDGET] = nextLayout;
		}
	}
	// normal
	if (board_start + keys.count() < board.count()) {
		board_start += keys.count();
	}
	else {
		board_start = 0;
	}
	displayTXTKeys();
}

void TypingController::spaceClick() {
	// training
	if (isTraining & currKey < trainingKey.count()) {
		if (trainingKey[currKey] != " ") {
			return;
		}

		currKey++;

		if (currKey == trainingKey.count()) {
			isTrainingFinished = true;
			// finished training
		}
	}

	// normal
	QPushButton* btn = listKeyButton["Words"];
	btn->setText(btn->text() + " ");
	filterBoard();
}

void TypingController::deleteClick() {
	QString w = listKeyButton["Words"]->text();
	if (w.length() > 0) {
		w = w.mid(0, w.length() - 1);
		if (currKey > 0) currKey--;
	}
	listKeyButton["Words"]->setText(w);
	filterBoard();
}

void TypingController::txtClick() {
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	QString k = btn->text();
	if (k == "") {
		return;
	}
	else if (k == "⌧") {
		clearClick();
		return;
	}

	// training 
	if (isTraining && currKey < trainingKey.count()) {
		if (btn->text() != trainingKey[currKey]) {
			return;
		} 

		currKey++;

		if (currKey == trainingKey.count()) {
			isTrainingFinished = true;
		}

		if (currKey < trainingKey.count() && trainingKey[currKey] == "talk") {
			// talk finished
		}
	}

	// normal
	QString w = listKeyButton["Words"]->text();

	if (w != "") {
		int i = w.length();
		while (i > 0 && w[i - 1] != ' ') i--;
		w = w.remove(i, w.length() - i);
	}

	// audio
	//if (k[k.length() - 1] == ' ') {
	//	//
	//}

	listKeyButton["Words"]->setText(w + k);
	filterBoard();
}


QString TypingController::makeKhongDau(QString input) {
	QString output = "";
	for (int i = 0; i < input.length(); i++) {
		QString c = input[i];
		QString o;
		if (khongdau.contains(c)) {
			o = khongdau[c];
			output += o[0];
		}
		else {
			output += c;
		}
	}
	return output;
}

QList<QString> TypingController::createDau(QString d) {
	QList<QString> daus;
	d = makeKhongDau(d);
	int len = 4;
	int pos = d.length();
	QList<QString> surfix{ "r","s","f","j","x" };
	bool complete = false;

	// check if it is a complete word
	for (int i = 0; i < van.count(); i++) {
		if (van[i] == d) {
			complete = true;
			break;
		} 
	}

	if (!complete) return daus;

	if (d.length() < len) len = d.length();

	for (int k = len; k > 0; k--) {
		for (int s = 0; s < surfix.count(); s++) {
			QString l1 = d.mid(0, d.length() - k);
			QString prefix = d.mid(d.length() - k, k);
			QString vkey = prefix + surfix[s];
			QString dau = bangdau.contains(vkey) ? bangdau[vkey] : "";
			if (bangdau.contains(vkey) && dau != "") {
				QString txt = l1 + dau;
				daus.append(txt + " ");
			}
		}
		if (daus.count() > 0) {
			break;
		}
	}
	return daus;
}

void TypingController::filterBoard() {
	QString w = listKeyButton["Words"]->text();
	QString d = "";

	if (w != "") {
		int i = w.length();
		while (i > 0 && w[i - 1] != ' ') i--;
		d = w.mid(i, w.length() - i);
	}

	if (d == "") {
		board = all;
	}
	else {
		board.clear();
		for each (auto item in all) 
		{
			QString dx = d + item;
			for (int i = 0; i < van.count(); i++) {
				if (van[i].startsWith(dx)) {
					board.append(dx);
					break;
				}
			}
		}

		QList<QString> daus = createDau(d);
		for each (auto item in daus) {
			board.append(item);
		}
	}

	board_start = 0;
	displayTXTKeys();
}

void TypingController::readVan() {
	QString txt;
	QFile file("van.csv");
	if (file.open(QIODevice::ReadOnly)) {
		txt = file.readAll();
		auto data = read::Read(txt);

		for each (auto row in data) {
			for (int col = 0; col < 28; col++) {
				QString v = row[QString::number(col)];
				if (v != "") {
					van.append(v);
				}
			}
		}
	}
}

void TypingController::readDau() {
	QString txt;
	QFile file("dau.csv");
	if (file.open(QIODevice::ReadOnly)) {
		txt = file.readAll();
		auto data = read::Read(txt);

		QList<QString> surfix{ "r","s","f","j","x" };

		for each (auto row in data) {
			QString prefix = row["prefix"];
			for (int s = 0; s < surfix.count(); s++) {
				QString key = prefix + surfix[s];
				QString str = row[surfix[s]];
				if (str != "") {
					bangdau[key] = str;
				}
			}
		}
	}
}
