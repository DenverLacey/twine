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
    twAppendUTF8(&buf, twStatic("    "));
    printf("'"twFmt"'\n", twArg(buf));

    twInsertStrUTF8(&buf, 0, twStatic("\t"));
    printf("'"twFmt"'\n", twArg(buf));

    twString s = twBufToString(buf);

    twString left_trimmed = twTrimLeftUTF8(s);
    printf("'"twFmt"'\n", twArg(left_trimmed));

    twString right_trimmed = twTrimRightUTF8(s);
    printf("'"twFmt"'\n", twArg(right_trimmed));

    twString trimmed = twTrimUTF8(s);
    printf("'"twFmt"'\n", twArg(trimmed));

    return 0;
}

