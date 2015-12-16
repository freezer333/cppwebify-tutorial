#include <iostream>
#include <stdio.h>
#include "prime_sieve.h"
using namespace std;
// Pretty standard app - just pass the command line arguments
// along to primesieve.  The output is always printed to stdout
// - this is a good example if you can't modify your legacy C++
// source code, and it always prints to standard out.
int main(int argc, char ** argvs) {
    generate_args(argc, argvs, stdout);
}
