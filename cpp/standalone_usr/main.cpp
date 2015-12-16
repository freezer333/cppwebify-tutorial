#include <iostream>
#include <stdio.h>
#include "prime_sieve.h"

using namespace std;


// Pretty standard app.  Here the app asks the user for the
// input needed - which is less fun to automate.
// The output is always printed to stdout
// - this is a good example if you can't modify your legacy C++
// source code, and it always prints to standard out.
int main(int argc, char ** argvs) {
    int max;
    cout << "Please enter the maximum number:  ";
    cin  >> max;
    generate(max, stdout);
}