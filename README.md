# twine
A header-only string handling library that supports Unicode.

One of C's main pain points is strings. Null-terminated ASCII strings leave a
lot to be desired and the frankly arcane string handling functions in the C
standard library makes modern string manipulation code tedious and error-prone.
Twine aims to provide a more modern string handling library that also supports
Unicode, both UTF-8 and UTF-16.

## Example

Here's a short example that splits a string by SPACE and prints each word on a
newline.

```c
#include <stdio.h>

#define TWINE_IMPLEMENTATION
#include "twine.h"

void print_words(twString s) {
    while (s.length > 0) {
        twString word = twSplitUTF8(s, ' ', &s);
        printf(twFmt"\n", twArg(word));
    }
}
```

Here's an example that counts the number of whitespace characters are in a string.

```c
#define TWINE_IMPLEMENTATION
#include "twine.h"

int count_whitespace(twString s) {
    int count = 0;

    twChar c;
    while (twNextUTF8(&s, &c)) {
        if (twIsSpace(c)) {
            count++;
        }
    }

    return count;
}
```
