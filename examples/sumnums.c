#include <stdio.h>
#include <string.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    twString line = twStatic("10 20 30");
    while (line.length > 0) {
        twString word = twSplit(line, ' ', &line);
        printf("%.*s\n", word.length, word.bytes);
    }
    return 0;
}
