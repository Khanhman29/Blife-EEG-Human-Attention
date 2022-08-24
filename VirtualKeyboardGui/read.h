#pragma once

#include <QDebug>
#include <QString>
#include <QList>
#include <QRegExp>
#include <QMap>
#include <QRegularExpression>

class read
{

public:
	static QRegularExpression SPLIT_RE;
	static QRegularExpression LINE_SPLIT_RE;
	static QRegularExpression TRIM_CHARS;

	static QList<QMap<QString, QString>> Read(QString);
	static QList<QMap<QString, QString>> ReadHistory(QString);
	static QMap<QString, QString> ReadSetting(QString);
};

