#include "SettingService.h"
#include <string>
#include <QWidget>

#include "Key.h"

std::string SettingService::CurrentLanguage = "Vietnamese";
std::string SettingService::CurrentColor = "Blue";
float SettingService::dwTime = 2.00f;

std::string SettingService::GetColorName()
{
	if (CurrentLanguage == "Vietnamese") {
		if (CurrentColor == "Blue") {
			return "Xanh";
		}
		else {
			return "Đỏ";
		}
	}
	else {
		return CurrentColor;
	}
}

std::string SettingService::GetLanguageName()
{
	if (CurrentLanguage == "Vietnamese") {
		return "Tiếng Việt";
	}
	else {
		return CurrentLanguage;
	}
}

QString SettingService::GetPrefix()
{
	if (CurrentLanguage == "Vietnamese") {
		if (CurrentColor == "Blue") {
			return "_vn_b";
		}
		else {
			return "_vn_r";
		}
	}
	else {
		if (CurrentColor == "Blue") {
			return "_en_b";
		}
		else {
			return "_en_r";
		}
	}
}

QString SettingService::GetColorPrefix()
{
	if (CurrentColor == "Blue") {
		return "_b";
	}
	
	return "_r";
}

QPushButton* SettingService::GetIconButton(QPushButton* button, QString path, QSize size)
{
	QIcon icon;
	icon.addPixmap(QPixmap(path), QIcon::Disabled, QIcon::On);
	icon.addPixmap(QPixmap(path), QIcon::Disabled, QIcon::Off);
	button->setIcon(icon);
	button->setIconSize(size);

	return button;
}


