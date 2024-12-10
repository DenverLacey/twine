#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    twString line = twStatic("10 20 30");
    while (line.length > 0) {
        twString word = twSplitUTF8(line, ' ', &line);
        printf(twFmt"\n", twArg(word));
    }
    return 0;
}

