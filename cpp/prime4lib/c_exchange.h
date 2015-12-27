
/*

    This should not be included directly from C++ code.  Instead, 
    always include exchange.h

    C code cannot include exchange.h, and should only include this file.

    Using _exchangeclass a guard because g++ on Mac wasn't properly
    setting the _cplusplus definition!

*/

#ifdef _exchangeclass
extern "C" {
#endif

void pass(void * exchanger, int data);

#ifdef _exchangeclass
}
#endif

