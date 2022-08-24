#pragma once

#include <QString>
#include <QMap>

class KeyData
{
public:
	KeyData() {}
	KeyData(QMap<QString, QString> data) {
		keyType = data["keyType"].toInt();
		name = data["name"];
		key = data["key"];
		exFrom = data["exFrom"];
		exTime = data["exTime"].toFloat();
		dwTime = data["dwTime"].toFloat();
		origindwTime = dwTime;
		pos = data["pos"].toInt();
		span = data["span"].toInt();
		spanX = data["spanX"].toInt();
		spanY = data["spanY"].toInt();
		scala = data["scala"].toFloat();
		background = data["background"];
		if (data["KB"].length() > 0) {
			KB = data["KB"].toInt();
		}
		temp = 0;
		temp1 = 0;
		//prefix = data["prefix"];
		//r = data["r"];
		//s = data["s"];
		//f = data["f"];
		//j = data["j"];
		//x = data["x"];
	}

	int keyType;
	QString name;
	QString key;
	QString exFrom;
	float exTime;
	float dwTime;
	int pos;
	int span;
	int spanX;
	int spanY;
	float scala;
	QString background;
	int KB;
	float origindwTime;
	float originexTime;
	int temp;
	int temp1;
	//QString prefix;
	//QString r;
	//QString s;
	//QString f;
	//QString j;
	//QString x;
};

