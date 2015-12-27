#include "exchange.h"

void pass(void * exchanger, int data) {
    exchange * x = (exchange *) exchanger;
    x->send(data);
}

