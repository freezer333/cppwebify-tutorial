#define _exchangeclass
#include <iostream>
#include <functional>
//using namespace std;

class Exchange {
public:
    Exchange(const std::function<void (void *)> & c) {
        this->callback = c;
    }
    void send(int data){
       this->callback(&data);
    }
private:
    std::function<void (void *)> callback;
};

#include "c_exchange.h"
