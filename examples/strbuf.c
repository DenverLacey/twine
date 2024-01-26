#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    char mem[256];    
    twStringBuf buf = twStaticBuf(mem);
    
    twPushUTF8(&buf, 'A');
    printf(twFmt"\n", twArg(buf));

    twAppendUTF8(&buf, twStatic("BCD çæµñ"));
    printf(twFmt"\n", twArg(buf));

    twInsertUTF8(&buf, 1, '1');
    printf(twFmt"\n", twArg(buf));
    
    twInsertStrUTF8(&buf, 2, twStatic("234"));
    printf(twFmt"\n", twArg(buf));

    twInsertStrUTF8(&buf, 0, twStatic("    "));
    printf(twFmt"\n", twArg(buf));

    twInsertStrUTF8(&buf, 0, twStatic("\t"));
    printf(twFmt"\n", twArg(buf));

    twString s = twBufToString(buf);
    s = twTrimLeftUTF8(s);
    printf(twFmt"\n", twArg(s));

    return 0;
}
