#include <assert.h>
#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    const char *ascii = "A";
    const char *utf_2 = "Á";
    const char *utf_3 = "ऄ";
    const char *utf_4 = "𐂂";

    // Test Decode
    {
        twChar c;

        twDecodeUTF8(ascii, &c, 1);
        assert(c == 65);
        printf("%s == U+%04X\n", ascii, c);

        twDecodeUTF8(utf_2, &c, 1);
        assert(c == 0x00C1);
        printf("%s == U+%04X\n", utf_2, c);

        twDecodeUTF8(utf_3, &c, 1);
        assert(c == 0x0904);
        printf("%s == U+%04X\n", utf_3, c);

        twDecodeUTF8(utf_4, &c, 1);
        assert(c == 0x10082);
        printf("%s == U+%04X\n", utf_4, c);
    }

    // Test Encode
    {
        char s[5] = {0};

        int ascii_length = twEncodeUTF8(s, 4,'A');
        assert(ascii_length == 1);
        printf("U+%04X == %s\n", 'A', s);

        int utf_2_length = twEncodeUTF8(s, 4, 0x00C1);
        assert(utf_2_length == 2);
        printf("U+%04X == %s\n", 0x00C1, s);

        int utf_3_length = twEncodeUTF8(s, 4, 0x0904);
        assert(utf_3_length == 3);
        printf("U+%04X == %s\n", 0x0904, s);

        int utf_4_length = twEncodeUTF8(s, 4, 0x10082);
        assert(utf_4_length == 4);
        printf("U+%04X == %s\n", 0x10082, s);
    }

    return 0;
}

