#include "f2.h"

/*
* f1 calls the print_stack and f2 functions, passing a local string into f2 when calling it.
* @param num: Takes an unsigned int as its input.
* Function has a void return value.
*/

void f1(unsigned int num) {
    print_stack("in f1", OFFSET);
    char str[] = "ABCD";
    f2(str);
}
