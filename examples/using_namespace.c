#define TWINE_IMPLEMENTATION
#define TWINE_USING_NAMESPACE
#include "../twine.h"

int main(void) {
    String s = Static("Hello World with no namespace!");
    size_t len = LenUTF8(s);
    printf("The string '"Fmt"' is %zu characters long.\n", Arg(s), len);
}

