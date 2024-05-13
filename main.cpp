#include <iostream>
#include <thread>

#include "lights_monitor.hpp"

using namespace std;

void sig_handler(sig_atomic_t s){
    cout << endl << "***stopping monitoring client***" << endl;
    joshai::lights_monitor::stop();
    exit(0); 
}

int main(int, char**){
    
    signal (SIGINT,sig_handler);

    std::thread t([]{
        joshai::lights_monitor m;
        m.start();
    });
    t.join();
}