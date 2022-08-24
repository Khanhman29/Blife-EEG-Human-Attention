#pragma once
#include <string>
#include <QFile>
#include <QTextStream>

class APIService
{
public:
	void PublishMessage(char*, char*, int);
	static void GetDevicesAPI();
	static QString ReadFile();
};
