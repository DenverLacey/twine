#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

#define TAB_SIZE (4)

int main(int argc, const char **argv) {
    twString input;
    if (argc <= 1) {
        input = twStatic("HI!");
    } else {
        input = twStr(argv[1]);
    }

    int i = 0;
    twChar c;
    while (twNextUTF8(&input, &c)) {
        printf("%*s%c\n", i * TAB_SIZE, "", c);
        i++;
    }

    return 0;
}

