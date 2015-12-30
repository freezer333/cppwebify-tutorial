#include "exchange.h"

void pass(void * exchanger, int data) {
    Exchange * x = (Exchange *) exchanger;
    x->send(data);
}
