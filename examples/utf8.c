#include <stdio.h>
#include <string.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    twString example = twStatic("AƩऐ🜏");
    
    twString it = example;
    twChar c;
    while (twNextUTF8(&it, &c)) {
        printf("U+%04X\n", c);
    }

    printf("\n");

    it = example;
    while (twNextRevUTF8(&it, &c)) {
        printf("U+%04X\n", c);
    }
}
