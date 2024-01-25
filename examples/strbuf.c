#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    char mem[256];    
    twStringBuf buf = twStaticBuf(mem);
    
    twPushUTF8(&buf, 'A');
    twAppendUTF8(&buf, twStatic("BCD çæµñ"));

    twString str = twBufToString(buf);
    printf(twFmt"\n", twArg(str));

    return 0;
}
