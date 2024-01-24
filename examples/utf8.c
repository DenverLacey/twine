#include <stdio.h>
#include <string.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    twString example = twStatic("AƩऐ🜏");
    
    twChar c;
    while (twNextUTF8(&example, &c)) {
        printf("U+%04X\n", c);
    }
}
