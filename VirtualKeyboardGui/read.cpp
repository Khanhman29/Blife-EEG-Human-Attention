#include "read.h"

QRegularExpression read::SPLIT_RE(",(?=(?:[^\"]*\"[^\"]*\")*(?![^\"]*\"))");

QRegularExpression read::LINE_SPLIT_RE("\\r\\n|\\n\\r|\\n|\\r");

QList<QMap<QString, QString>> read::Read(QString data) {
	QList<QMap<QString, QString>> list;
	
	QStringList lines = data.split(LINE_SPLIT_RE);
	if (lines.length() <= 1) {
		return list;
	}

	QStringList header = lines[0].split(SPLIT_RE);

	// add to list 
	for (int i = 1; i < lines.length(); i++) {
		QStringList values = lines[i].split(SPLIT_RE);
		if (values.length() == 0 || values[0] == "") continue;

		QMap<QString, QString> entry;
		for (int j = 0; j < header.length() && j < values.length(); j++) {
			QString value = values[j].remove(QRegularExpression("\\\""));
			entry[header[j]] = value;
		}
		list.append(entry);
	}
	
	return list;
}

QList<QMap<QString, QString>> read::ReadHistory(QString data) {
	QList<QMap<QString, QString>> stack;

	QStringList lines = data.split(LINE_SPLIT_RE);

	if (lines.length() <= 1) return stack;

	QStringList header = lines[0].split(SPLIT_RE);

	// add to list
	for (int i = 1; i < lines.length(); i++) {
		QStringList values = lines[i].split(SPLIT_RE);
		if (values.length() == 0 || values[0] == "") continue;

		QMap<QString, QString> entry;
		for (int j = 0; j < header.length() && j < values.length(); j++) {
			QString value = values[j];
			entry[header[j]] = value;
		}
		stack.append(entry);
	}
	return stack;
}

QMap<QString, QString> read::ReadSetting(QString data) {
	QMap<QString, QString> list;
	
	auto lines = data.split(LINE_SPLIT_RE);

	if (lines.length() <= 1) return list;
	for (int i = 0; i < lines.length(); i++) {
		auto values = lines[i].split(SPLIT_RE);
		if (values.length() == 0 || values[0] == "") continue;

		QString header = values[0];
		QString value = values[1];
		list[header] = value;
	}

	return list;
}