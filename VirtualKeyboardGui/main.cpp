#include "menu.h"
#include <QtWidgets/QApplication>
#include "Typing_booster/TypingBooster.cpp"
#include <stdlib.h>
#include <iostream>
#include <stdio.h>

tobii_api_t* api;
tobii_device_t* device;
tobii_error_t error = TOBII_ERROR_INTERNAL;

static void url_receiver(char const* url, void* user_data) {
    char* buffer = (char*)user_data;
    if (*buffer != '\0') {
        return;
    }
    if (strlen(url) < 256) {
        strcpy(buffer, url);
    }
}
int main(int argc, char* argv[])
    
{
    //std::thread t1([] {
      //  TypingBooster typingBooster;
        //typingBooster.RunpythonCode();
        //});
    //t1.detach();
    // create api
    error = tobii_api_create(&api, NULL, NULL);

    if (error == TOBII_ERROR_NO_ERROR) {
        qDebug() << "api successful";
    }

    // get url
    char url[256] = { 0 };
    error = tobii_enumerate_local_device_urls(api, url_receiver, url);
    assert(error == TOBII_ERROR_NO_ERROR);

    // create device
    error = tobii_device_create(api, url, TOBII_FIELD_OF_USE_INTERACTIVE, &device);
    if (error != TOBII_ERROR_NO_ERROR) {
        qDebug() << tobii_error_message(error);
        tobii_device_destroy(device);
        tobii_api_destroy(api);
    }

    //error = tobii_api_destroy(api);

    QApplication a(argc, argv);
    Menu w;
    //w.showFullScreen();
    w.setWindowState(Qt::WindowMaximized);
    w.show();
    return a.exec();
}