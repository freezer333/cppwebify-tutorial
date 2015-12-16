#include <iostream>
#include <stdio.h>
#include "prime_sieve.h"

using namespace std;


// Simulating a legacy C++ app that reads
// it's input from a user-specified file via command line
// arguments, and outputs to a similarly specified file.
int main(int argc, char ** argvs) {
    FILE * in = fopen(argvs[1], "r");
    int i;
    fscanf (in, "%d", &i);
    fclose(in);

    FILE * out = fopen(argvs[2], "w");
    generate(i, out);
    fprintf(stdout, "Output saved in %s\n", argvs[2]);
    fclose(out);
}
