//#pragma once
//
//#include <QMediaPlayer>
//
//#include <iostream>
//#include <string>
//#include "curl/curl.h"
//
//
//class uAudioPlayer {
//public:
//	QString targetFile;
//	QString inputVoice;
//	QMediaPlayer* player;
//	std::string result;
//	CURL* curl = NULL;
//	CURLcode res = CURLE_COULDNT_CONNECT;
//	size_t size = 0;
//public:
//	uAudioPlayer(QMediaPlayer *p) {
//		this->player = p;
//	}
//
//	void setInputVoice(QString input) {
//		this->inputVoice = input;
//	}
//	
//	static size_t write_to_string(void* ptr, size_t size, size_t count, void* stream) {
//		((std::string*)stream)->append((char*)ptr, 0, size * count);
//		return size * count;
//	}
//
//	void getVoice() {
//		curl_global_init(CURL_GLOBAL_DEFAULT);
//		curl = curl_easy_init();
//		if (curl) {
//			std::string temp(u8"thế thì thôi");
//			char* input = new char[temp.length() + 1];
//			strcpy(input, temp.c_str());
//			std::string data_encode = curl_easy_escape(curl, input, temp.length());
//			struct curl_slist* headers = NULL;
//			headers = curl_slist_append(headers, "apikey: EOpf2QfVFmLsT4DTgmMbIIhd3lodjL3i");
//			curl_easy_setopt(curl, CURLOPT_URL, "https://api.zalo.ai/v1/tts/synthesize");
//			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//			std::string datafield = "input=" + data_encode;
//			std::string tempdata = "";
//			for (int i = 0; i < datafield.length(); i++) {
//				if (datafield[i] == '%') {
//					tempdata.append("%%");
//				}
//				else {
//					tempdata.push_back(datafield[i]);
//				}
//			}
//			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, u8"input=à thế à");
//			
//			std::string response;
//			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
//			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//			res = curl_easy_perform(curl);
//			if (res != CURLE_OK) {
//				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//			}
//			curl_easy_cleanup(curl);
//		}
//		curl_global_cleanup();
//	}
//
//	void start() {
//
//	}
//};