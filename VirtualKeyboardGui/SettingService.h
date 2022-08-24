#pragma once
#include <string>
#include <QWidget>

#include "Key.h"

class SettingService
{
private:
public:
	static std::string CurrentLanguage;
	static std::string CurrentColor;
	static float dwTime;

	static std::string GetLanguageName();
	static std::string GetColorName();
	static QString GetPrefix();
	static QString GetColorPrefix();
	static QPushButton* GetIconButton(QPushButton* button, QString path, QSize size);
};

