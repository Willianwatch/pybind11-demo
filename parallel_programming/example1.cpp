#include <iostream>
#include <thread>
#include <unistd.h>

void myprint() {
    std::cout << "My thread start." << std::endl;
    sleep(5);
    std::cout << "My thread end." << std::endl;
}

int main() {
    std::thread mytobj(myprint);
    mytobj.join();

    std::cout << "Main function end." << std::endl;
    return 0;
}