#include <iostream>
#include <stdio.h>
#include <functional>

#include "exchange.h"
#include "prime_sieve.h"
#include "primeapi.h"
using namespace std;



// it's assumed primes is a pre-allocated array of at least "under" size.
// the function fills in the primes array with all prime numbers less
// than "under".  It returns the number of prime numbers
// found by the prime_sieve algorithm.
int getPrimes(int under, int primes[]) {
	int count = 0;
    exchange x(
        [&](void * data) { 
            int * iptr = (int *) data;
            primes[count++] = *iptr;
        }
    );

    generate_primes(under, (void*)&x);

    return count;
}


