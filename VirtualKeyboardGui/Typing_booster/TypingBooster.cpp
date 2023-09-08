#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
using namespace std;

class TypingBooster {
public:
    void RunpythonCode() {
        putenv("PATH=C:\\Users\\ASUS\\AppData\\Local\\Programs\\Python\\Python311");
        putenv("PYTHONHOME=C:\\Users\\ASUS\\AppData\\Local\\Programs\\Python\\Python311");
        putenv("PYTHONPATH=C:\\Users\\ASUS\\AppData\\Local\\Programs\\Python\\Python311\\Lib;C:\\Users\\ASUS\\AppData\\Local\\Programs\\Python\\Python311\\DLLs");
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen("python C:\\Users\\ASUS\\source\\repos\\blife-iot\\VirtualKeyboardGui\\Typing_booster\\service_2.pyw", "r"), &_pclose);
    }
};





//C:\Users\ASUS\AppData\Local\Programs\Python\Python311