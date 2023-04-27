#define OFFSET 32 // Offset macro.
#include "f1.h" // Including f1.h header file.

/*
* The main function simply calls print_stack and f1. This will result in print_stack being used in main(), f1(), and f2().
* Returns 0 when finished.
*/

int main() {
    print_stack("in main", OFFSET);
    f1(10);

    return 0;
}
