#include <iostream>
#include<windows.h>
using namespace std;
int main() {
    cout << "First line of text.";
    cout.flush();

    Sleep(2000);

    // \r = carriage return, \x1b[2K clears the entire line
    cout << "\r\x1b[2KSecond";
    cout.flush();

    Sleep(2000);

    cout << "\n";

    return 0;
}
