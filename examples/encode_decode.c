#include <assert.h>
#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "../twine.h"

int main(void) {
    twString ascii = twStatic("A");
    twString utf_2 = twStatic("Á");
    twString utf_3 = twStatic("ऄ");
    twString utf_4 = twStatic("𐂂");

    // Test Decode
    {
        twChar c;

        int ascii_length = twDecodeUTF8(ascii, &c);
        assert(ascii_length == 1);
        assert(c == 65);
        printf("%.*s == U+%04X\n", ascii.length, ascii.bytes, c);

        int utf_2_length = twDecodeUTF8(utf_2, &c);
        assert(utf_2_length == 2);
        assert(c == 0x00C1);
        printf("%.*s == U+%04X\n", utf_2.length, utf_2.bytes, c);

        int utf_3_length = twDecodeUTF8(utf_3, &c);
        assert(utf_3_length == 3);
        assert(c == 0x0904);
        printf("%.*s == U+%04X\n", utf_3.length, utf_3.bytes, c);

        int utf_4_length = twDecodeUTF8(utf_4, &c);
        assert(utf_4_length == 4);
        assert(c == 0x10082);
        printf("%.*s == U+%04X\n", utf_4.length, utf_4.bytes, c);
    }

    // Test Encode
    {
        char buf[5];
        twString s = (twString){ .bytes = buf, .length = 4 };

        int ascii_length = twEncodeUTF8('A', &s);
        assert(ascii_length == 1);
        assert(s.length == 1);
        assert(twEqual(s, ascii));
        printf("U+%04X == %.*s\n", 'A', s.length, s.bytes);

        s.length = 4;

        int utf_2_length = twEncodeUTF8(0x00C1, &s);
        assert(utf_2_length == 2);
        assert(s.length == 2);
        assert(twEqual(s, utf_2));
        printf("U+%04X == %.*s\n", 0x00C1, s.length, s.bytes);

        s.length = 4;

        int utf_3_length = twEncodeUTF8(0x0904, &s);
        assert(utf_3_length == 3);
        assert(s.length == 3);
        assert(twEqual(s, utf_3));
        printf("U+%04X == %.*s\n", 0x0904, s.length, s.bytes);

        s.length = 4;

        int utf_4_length = twEncodeUTF8(0x10082, &s);
        assert(utf_4_length == 4);
        assert(s.length == 4);
        assert(twEqual(s, utf_4));
        printf("U+%04X == %.*s\n", 0x10082, s.length, s.bytes);
    }

    return 0;
}
