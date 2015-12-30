#include <nan.h>
#include <functional>
#include <iostream>

#include "exchange.h"
#include "prime_sieve.h"


using namespace Nan;
using namespace std;
using namespace v8;

// Synchronous access to the `getPrimes()` function
NAN_METHOD(CalculatePrimes) {
    Nan:: HandleScope scope;

    int under = To<int>(info[0]).FromJust();
    v8::Local<v8::Array> results = New<v8::Array>(under);

    int i = 0;
    Exchange x(
        [&](void * data) {
            Nan::Set(results, i, New<v8::Number>(*((int *) data)));
            i++;
       });

    generate_primes(under, (void*)&x);

    info.GetReturnValue().Set(results);
}


NAN_MODULE_INIT(Init) {
    Nan::Set(target, New<String>("getPrimes").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(CalculatePrimes)).ToLocalChecked());
}

NODE_MODULE(addon, Init)
