#ifndef _TWINE_H_
#define _TWINE_H_

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// @brief Simple pointer-and-length string.
typedef struct twString {
    const char *bytes;
    size_t      length;
} twString;

/// @brief A growable and mutable string buffer capable of dynamic string construction.
typedef struct twStringBuf {
    char  *bytes;
    size_t length;
    size_t capacity;
    size_t max_capacity;
} twStringBuf;

/// @brief a 32-bit integer used to represent characters as Unicode codepoints.
typedef uint32_t twChar;

//
// C-String functions
//

/// @brief Encodes a unicode codepoint as UTF-8 into a given buffer.
/// @param bytes [OUT] Destination bytes of the encoded character.
/// @param nbytes Number of bytes available for encoding.
/// @param c The character to encode.
/// @return The codepoint length of `c`.
int twEncodeUTF8(char *bytes, int nbytes, twChar c);

/// @brief Encodes a unicode codepoint as UTF-16 into a given buffer.
/// @param bytes [OUT] Destination bytes of the encoded character.
/// @param nbytes Number of bytes available for encoding.
/// @param c The character to encode.
/// @return The codepoint length of `c`.
int twEncodeUTF16(char *bytes, int nbytes, twChar c);

//
// `twString` functions
//

/// @brief Creates a `twString` from a C string. (Doesn't allocate.)
twString twStr(const char *s);

/// @brief Creates a `twString` from a static C string without walking the
///        string to find it's length.
#define twStatic(S) (twString){ .bytes = S, .length = sizeof(S) - 1 }

/// @brief Calculates the number of graphemes in `s`.
/// @param s A UTF-8 encoded string.
size_t twLenUTF8(twString s);

/// @brief Calculates the number of graphemes in `s`.
/// @param s A UTF-16 encoded string.
size_t twLenUTF16(twString s);

/// @brief Checks that `s` is a valid sequence of UTF-16.
bool twIsValidUTF8(twString s);

/// @brief Checks that `s` is a valid sequence of UTF-16.
bool twIsValidUTF16(twString s);

/// @brief Comapres two strings.
/// @return Returns `true` if the strings are equal, and `false` if not.
bool twEqual(twString a, twString b);

/// @brief Splits a string by a character. (The split character is not included.)
/// @param s The UTF-8 encoded string to split.
/// @param c The charcter to search for.
/// @param remainder [OUT, OPT] The rest of the string after the split character.
/// @return A string slice of the contents of `s` before the first instance of `c`.
twString twSplitUTF8(twString s, twChar c, twString *remainder);

/// @brief Splits a string by a character. (The split character is not included.)
/// @param s The UTF-16 encoded string to split.
/// @param c The charcter to search for.
/// @param remainder [OUT, OPT] The rest of the string after the split character.
/// @return A string slice of the contents of `s` before the first instance of `c`.
twString twSplitUTF16(twString s, twChar c, twString *remainder);

/// @brief The first character of `s` as a `twString`.
/// @param s A UTF-8 encoded string.
/// @return The in-place bytes of the first character.
twString twHeadUTF8(twString s);

/// @brief The first character of `s` as a `twString`.
/// @param s A UTF-16 encoded string.
/// @return The in-place bytes of the first character.
twString twHeadUTF16(twString s);

/// @param s A UTF-8 encoded string.
/// @return The tail of `s` (All except the first character.)
twString twTailUTF8(twString s);

/// @param s A UTF-16 encoded string.
/// @return The tail of `s` (All except the first character.)
twString twTailUTF16(twString s);

/// @brief The last character in `s`.
/// @return If `s` isn't empty, the function returns the last character in `s`.
///         Otherwise, it returns 0. Check length to determine if the last
///         character was actually 0 or if there is no last character.
twChar twLastASCII(twString s);

/// @brief The last character in `s`.
/// @return If `s` isn't empty, the function returns the last character in `s`.
///         Otherwise, it returns 0. Check length to determine if the last
///         character was actually 0 or if there is no last character.
twChar twLastUTF8(twString s);

/// @brief The last UTF-16 character in `s`.
/// @return If `s` isn't empty, the function returns the last character in `s`.
///         Otherwise, it returns 0. Check length to determine if the last
///         character was actually 0 or if there is no last character.
twChar twLastUTF16(twString s);

/// @brief Drop the first `n` bytes of `s`.
/// @param n Number of bytes to drop.
/// @return New `twString` with first `n` bytes of `s` removed.
twString twDrop(twString s, size_t n);

/// @brief Removes all leading whitespace characters.
/// @param s A UTF-8 encoded string.
/// @return A `twString` with leading whitesapce removed. (Points to original data.)
twString twTrimLeftUTF8(twString s);

/// @brief Removes all leading whitespace characters.
/// @param s A UTF-16 encoded string.
/// @return A `twString` with leading whitesapce removed. (Points to original data.)
twString twTrimLeftUTF16(twString s);

/// @brief Removes all trailing whitespace characters.
/// @param s A UTF-8 encoded string.
/// @return A `twString` with trailing whitesapce removed. (Points to original data.)
twString twTrimRightUTF8(twString s);

/// @brief Removes all trailing whitespace characters.
/// @param s A UTF-16 encoded string.
/// @return A `twString` with trailing whitesapce removed. (Points to original data.)
twString twTrimRightUTF16(twString s);

/// @brief Removes all leading and trailing whitespace characters.
/// @param s A UTF-8 encoded string.
/// @return A `twString` with leading and trailing whitesapce removed. (Points to original data.)
twString twTrimUTF8(twString s);

/// @brief Removes all leading and trailing whitespace characters.
/// @param s A UTF-16 encoded string.
/// @return A `twString` with leading and trailing whitesapce removed. (Points to original data.)
twString twTrimUTF16(twString s);

//
// `twStringBuf` functions
//

/// @brief Creates a `twStringBuf` out of a static buffer.
/// @param S The buffer to wrap.
/// @return A `twStringBuf` whose memory is preallocated in a static buffer.
#define twStaticBuf(S) (twStringBuf){ \
    .bytes = S, \
    .length = 0, \
    .capacity = sizeof(S), \
    .max_capacity = sizeof(S) \
}

/// @brief Creates a `twString` from a `twStringBuf`.
twString twBufToString(twStringBuf buf);

/// @brief Extends the size of the buffer to be `new_size` unless `new_size`
///        exceeds `max_capacity`.
/// @return True if buffer was extended successfully. Otherwise, returns false.
bool twExtendBuf(twStringBuf *buf, size_t new_size);

/// @brief Adds a character to the end of a string buffer.
/// @param s A UTF-8 encdoded string buffer.
/// @return True if character was added succcessfully, otherwise returns false.
bool twPushUTF8(twStringBuf *buf, twChar c);

/// @brief Adds a character to the end of a string buffer.
/// @param buf A UTF-16 encdoded string buffer.
/// @return True if character was added succcessfully, otherwise returns false.
bool twPushUTF16(twStringBuf *buf, twChar c);

/// @brief Appends a string to the end of a string buffer.
/// @param buf A UTF-8 encoded string buffer.
/// @param s A UTF-8 encoded string.
/// @return True if string was added successfully. Otherwise, returns false.
bool twAppendUTF8(twStringBuf *buf, twString s);

/// @brief Appends a string to the end of a string buffer.
/// @param buf A UTF-16 encoded string buffer.
/// @param s A UTF-16 encoded string.
/// @return True if string was added successfully. Otherwise, returns false.
bool twAppendUTF16(twStringBuf *buf, twString s);

/// @brief Appends a formatted string to the end of a string buffer.
/// @param buf A UTF-8 encoded string buffer.
/// @return True if string was added sucessfully. Otherwise, returns false.
bool twAppendFmtUTF8(twStringBuf *buf, const char * restrict fmt, ...);

/// @brief Appends a formatted string to the end of a string buffer.
/// @param buf A UTF-16 encoded string buffer.
/// @return True if string was added sucessfully. Otherwise, returns false.
bool twAppendFmtUTF16(twStringBuf *buf, const char * restrict fmt, ...);

/// @brief Appends a string to the end of a string buffer and adds a newline.
/// @param buf A UTF-8 encoded string buffer.
/// @param s A UTF-8 encoded string.
/// @return True if string was added successfully. Otherwise, returns false.
bool twAppendLineUTF8(twStringBuf *buf, twString s);

/// @brief Appends a string to the end of a string buffer and adds a newline.
/// @param buf A UTF-16 encoded string buffer.
/// @param s A UTF-16 encoded string.
/// @return True if string was added successfully. Otherwise, returns false.
bool twAppendLineUTF16(twStringBuf *buf, twString s);

/// @brief Inserts a character into the buffer at a byte index.
/// @param buf The UTF-8 encoded buffer to insert the character into.
/// @param idx The byte position to insert the character.
/// @param c The character.
/// @return True if the character was inserted successfully. Otherwise, returns false.
bool twInsertUTF8(twStringBuf *buf, size_t idx, twChar c);

/// @brief Inserts a character into the buffer at a byte index.
/// @param buf The UTF-16 encoded buffer to insert the character into.
/// @param idx The byte position to insert the character.
/// @param c The character.
/// @return True if the character was inserted successfully. Otherwise, returns false.
bool twInsertUTF16(twStringBuf *buf, size_t idx, twChar c);

/// @brief Inserts a string into the buffer at a byte index.
/// @param buf The UTF-8 encoded buffer to insert the string into.
/// @param idx The byte position to insert the string.
/// @param s The UTF-8 encoded string.
/// @return True if the string was inserted successfully. Otherwise, returns false.
bool twInsertStrUTF8(twStringBuf *buf, size_t idx, twString s);

/// @brief Inserts a string into the buffer at a byte index.
/// @param buf The UTF-16 encoded buffer to insert the string into.
/// @param idx The byte position to insert the string.
/// @param s The UTF-16 encoded string.
/// @return True if the string was inserted successfully. Otherwise, returns false.
bool twInsertStrUTF16(twStringBuf *buf, size_t idx, twString s);

/// @brief Removes every character from a string buffer.
void twClear(twStringBuf *buf);

//
// `twChar` functions
//

/// @brief How many bytes required to encode a codepoint in UTF-8.
int twCodepointLengthUTF8(twChar c);

/// @brief How many bytes required to encode a codepoint in UTF-16.
int twCodepointLengthUTF16(twChar c);

/// @brief Number of bytes for an already encoded character.
/// @param byte1 The first byte of the UTF-8 encoded character.
int twEncodedCodepointLengthUTF8(char byte1);

/// @brief Number of bytes for an already encoded UTF-16 character.
/// @param byte1 The first byte of the UTF-16 encoded character.
int twEncodedCodepointLengthUTF16(char byte1);

/// @brief Is `c` a white space character.
/// @note Considers ASCII whitespace, and characters in the `Line_Separator`,
///       `Paragraph_Separator`, and `Space_Separator` character cateogies a
///        white space character.
bool twIsSpace(twChar c);

/// @brief Decodes a single UTF-8 character sequence into a codepoint.
/// @param bytes The UTF-8 encoded character to decode.
/// @param result [OUT] The decoded character.
/// @return Returns 0 if decoding was unsuccessful. Otherwise, returns length
///         of the encoded character.
int twDecodeUTF8(twString bytes, twChar *result);

/// @brief Decodes a single UTF-16 character sequence into a codepoint.
/// @param bytes The UTF-16 encoded character to decode.
/// @param result [OUT] The decoded character.
/// @return Returns 0 if decoding was unsuccessful. Otherwise, returns length
///         of the encoded character.
int twDecodeUTF16(twString bytes, twChar *result);

//
// Iteration functions
//

/// @brief Can be used to get the next ASCII character or to iterate through
///        many ASCII characters in a string.
/// @param iter The string is marched along its charcters as this function is used.
/// @param result [OUT, OPT] The next character in the string `iter`.
/// @return Returns the number of bytes iterated over. If there is no more
///         characters or an error occurred, 0 is returned.
int twNextASCII(twString *iter, char *result);

/// @brief Can be used to get the next UTF-8 character or to iterate through
///        many UTF-8 characters in a string.
/// @param iter The string is marched along its charcters as this function is used.
/// @param result [OUT, OPT] The next character in the string `iter`.
/// @return Returns the number of bytes iterated over. If there is no more
///         characters or an error occurred, 0 is returned.
int twNextUTF8(twString *iter, twChar *result);

/// @brief Can be used to get the next UTF-16 character or to iterate through
///        many UTF-16 characters in a string.
/// @param iter The string is marched along its charcters as this function is used.
/// @param result [OUT, OPT] The next character in the string `iter`.
/// @return Returns the number of bytes iterated over. If there is no more
///         characters or an error occurred, 0 is returned.
int twNextUTF16(twString *iter, twChar *result);

/// @brief Can be used to get the last ASCII character or to iterate backwards
///        through many ASCII characters in a string.
/// @param iter The string is marched backwards along its characters as this function is used.
/// @param result [OUT, OPT] The next character in the sequence. The last character in `iter`.
/// @return The number of bytes iterated over. If there is no more character or an error occurred,
///         0 is returned.
int twNextRevASCII(twString *iter, char *result);

/// @brief Can be used to get the last UTF-8 character or to iterate backwards through
///        many UTF-8 characters in a string.
/// @param iter The string is marched backwards along its charcters as this function is used.
/// @param result [OUT, OPT] The next character in the sequence. The last character in `iter`.
/// @return The number of bytes iterated over. If there is no more characters or an error occurred,
///         0 is returned.
int twNextRevUTF8(twString *iter, twChar *result);

/// @brief Can be used to get the last UTF-16 character or to iterate backwards through
///        many UTF-16 characters in a string.
/// @param iter The string is marched backwards along its charcters as this function is used.
/// @param result [OUT, OPT] The next character in the sequence. The last character in `iter`.
/// @return The number of bytes iterated over. If there is no more characters or an error occurred,
///         0 is returned.
int twNextRevUTF16(twString *iter, twChar *result);

//
// Printf Niceties
//

/// @brief Format specifer string for `twString` and `twStringBuf`.
#define twFmt "%.*s"

/// @brief Correctly passes `S` to a printf-like function.
/// @param S Either a `twString` or a `twStringBuf`.
#define twArg(S) (int)(S).length, (S).bytes

#endif // _TWINE_H_

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                              IMPLEMENTATION                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifdef TWINE_IMPLEMENTATION

#ifndef _TWINE_H_IMPLEMENTATION_
#define _TWINE_H_IMPLEMENTATION_

//
// C-String functions
//

int twEncodeUTF8(char *bytes, int nbytes, twChar c) {
    int c_len = twCodepointLengthUTF8(c);
    assert(c_len != 0 && c_len <= nbytes);

    switch (c_len) {
        case 1:
            bytes[0] = c & 0x7F;
            break;
        case 2:
            bytes[0] = 0xC0 | ((c >> 6) & 0x1F);
            bytes[1] = 0x80 | (c & 0x3F);
            break;
        case 3:
            bytes[0] = 0xE0 | ((c >> 12) & 0x0F);
            bytes[1] = 0x80 | ((c >> 6) & 0x3F);
            bytes[2] = 0x80 | (c & 0x3F);
            break;
        case 4:
            bytes[0] = 0xF0 | ((c >> 18) & 0x07);
            bytes[1] = 0x80 | ((c >> 12) & 0x3F);
            bytes[2] = 0x80 | ((c >> 6) & 0x3F);
            bytes[3] = 0x80 | (c & 0x3F);
            break;
    }

    return c_len;
}

int twEncodeUTF16(char *bytes, int nbytes, twChar c) {
    int codepoint_length = twEncodedCodepointLengthUTF16(bytes[0]);
    if (codepoint_length == 0) {
        return 0;
    }

    switch (codepoint_length) {
        case 2:
            bytes[0] = (c >> 8) & 0xFF;
            bytes[1] = c & 0xFF;
            break;
        case 4:
            c -= 0x10000;
            bytes[0] = 0xDB | ((c >> 18) & 0x03);
            bytes[1] = (c >> 10) & 0xFF;
            bytes[2] = 0xDC | ((c >> 8) & 0x03);
            bytes[3] = c & 0xFF;
            break;
    }

    return codepoint_length;
}

//
// `twString` functions
//

twString twStr(const char *s) {
    if (s) {
        return (twString){ .bytes = s, .length = strlen(s) };
    } else {
        return (twString){0};
    }
}

size_t twLenUTF8(twString s) {
    // TODO: If we're doing graphemes then we need to handle things like
    //       character modifiers.

    size_t len = 0;
    while (twNextUTF8(&s, NULL)) {
        len++;
    }
    return len;
}

size_t twLenUTF16(twString s) {
    // TODO: If we're doing graphemes then we need to handle things like
    //       character modifiers.

    size_t len = 0;
    while (twNextUTF16(&s, NULL)) {
        len++;
    }
    return len;
}

bool twIsValidUTF8(twString s) {
    while (s.length > 0) {
        int c_len = twEncodedCodepointLengthUTF8(s.bytes[0]);
        if (c_len == 0) {
            return false;
        }
        s = twDrop(s, c_len);
    }
    return true;
}

bool twIsValidUTF16(twString s) {
    while (s.length > 0) {
        int c_len = twEncodedCodepointLengthUTF16(s.bytes[0]);
        if (c_len == 0) {
            return false;
        }
        s = twDrop(s, c_len);
    }
    return true;
}

bool twEqual(twString a, twString b) {
    return a.length == b.length && (memcmp(a.bytes, b.bytes, a.length) == 0);
}

twString twSplitUTF8(twString s, twChar c, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF8(&s, &cur)) > 0 && cur != c) {
        result.length += curlen;
        if (s.length <= 0) {
            break;
        }
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitUTF16(twString s, twChar c, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF16(&s, &cur)) > 0 && cur != c) {
        result.length += curlen;
        if (s.length <= 0) {
            break;
        }
    }

    if (remainder) *remainder = s;
    return result;
}

twString twHeadUTF8(twString s) {
    int c_len = twEncodedCodepointLengthUTF8(s.bytes[0]);
    if (c_len == 0) {
        return (twString){0};
    }
    return (twString){ .bytes = s.bytes, .length = c_len };
}

twString twHeadUTF16(twString s) {
    int c_len = twEncodedCodepointLengthUTF16(s.bytes[0]);
    if (c_len == 0) {
        return (twString){0};
    }
    return (twString){ .bytes = s.bytes, .length = c_len };
}

twString twTailUTF8(twString s) {
    int c_len = twEncodedCodepointLengthUTF8(s.bytes[0]);
    if (c_len == 0) {
        return (twString){0};
    }
    return twDrop(s, c_len);
}

twString twTailUTF16(twString s) {
    int c_len = twEncodedCodepointLengthUTF16(s.bytes[0]);
    if (c_len == 0) {
        return (twString){0};
    }
    return twDrop(s, c_len);
}

twChar twLastASCII(twString s) {
    if (s.length == 0) {
        return 0;
    }

    return s.bytes[s.length - 1];
}

twChar twLastUTF8(twString s) {
    if (s.length == 0) {
        return 0;
    }

    for (int i = s.length - 1; i >= 0; i--) {
        int len = twEncodedCodepointLengthUTF8(s.bytes[i]);
        if (len != 0) {
            twChar c;
            int c_len = twDecodeUTF8((twString){s.bytes + i, len}, &c);
            if (c_len != len) {
                return 0;
            }

            return c;
        }
    }

    return 0;
}

twChar twLastUTF16(twString s) {
    if (s.length == 0) {
        return 0;
    }

    for (int i = s.length - 1; i >= 0; i--) {
        int len = twEncodedCodepointLengthUTF16(s.bytes[i]);
        if (len != 0) {
            twChar c;
            int c_len = twDecodeUTF16((twString){s.bytes + i, len}, &c);
            if (c_len != len) {
                return 0;
            }

            return c;
        }
    }

    return 0;
}

twString twDrop(twString s, size_t n) {
    int num_dropped = s.length < n ? s.length : n;
    return (twString){
        .bytes = s.bytes + num_dropped,
        .length = s.length - num_dropped
    };
}

twString twTrimLeftUTF8(twString s) {
    twString iter = s;

    size_t ndrop = 0;
    for (;;) {
        twChar c;
        int c_len = twNextUTF8(&iter, &c);
        if (c_len == 0) break;

        if (!twIsSpace(c)) {
            break;
        }
        ndrop += c_len;
    }

    return twDrop(s, ndrop);
}

twString twTrimLeftUTF16(twString s) {
    twString iter = s;

    size_t ndrop = 0;
    for (;;) {
        twChar c;
        int c_len = twNextUTF16(&iter, &c);
        if (c_len == 0) break;

        if (!twIsSpace(c)) {
            break;
        }
        ndrop += c_len;
    }

    return twDrop(s, ndrop);
}

twString twTrimRightUTF8(twString s) {
    twString iter = s;

    size_t ndrop = 0;
    for (;;) {
        twChar c;
        int c_len = twNextRevUTF8(&iter, &c);
        if (c_len == 0) break;

        if (!twIsSpace(c)) {
            break;
        }

        ndrop += c_len;
    }

    return (twString){
        .bytes = s.bytes,
        .length = s.length - ndrop
    };
}

twString twTrimRightUTF16(twString s) {
    twString iter = s;

    size_t ndrop = 0;
    for (;;) {
        twChar c;
        int c_len = twNextRevUTF16(&iter, &c);
        if (c_len == 0) break;

        if (!twIsSpace(c)) {
            break;
        }

        ndrop += c_len;
    }

    return (twString){
        .bytes = s.bytes,
        .length = s.length - ndrop
    };   
}

twString twTrimUTF8(twString s) {
    s = twTrimLeftUTF8(s);
    s = twTrimRightUTF8(s);
    return s;
}

twString twTrimUTF16(twString s) {
    s = twTrimLeftUTF16(s);
    s = twTrimRightUTF16(s);
    return s;
}

//
// `twStringBuilder` functions
//

twString twBufToString(twStringBuf buf) {
    return *(twString*)&buf;
}

bool twExtendBuf(twStringBuf *buf, size_t new_size) {
    if (buf->max_capacity != 0 && new_size > buf->max_capacity) {
        return false;
    }

    if (buf->capacity >= new_size) {
        return true;
    }

    buf->bytes = realloc(buf->bytes, new_size);
    if (!buf->bytes) {
        return false;
    }

    buf->capacity = new_size;
    return true;
}

bool twPushUTF8(twStringBuf *buf, twChar c) {
    int c_len = twCodepointLengthUTF8(c);
    if (c_len == 0) {
        return false;
    }

    if (buf->length + c_len > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + c_len)) {
            return false;
        }
    }

    int encode_result = twEncodeUTF8(buf->bytes + buf->length, buf->capacity - buf->length, c);
    if (encode_result == 0 || encode_result != c_len) {
        return false;
    }

    buf->length += c_len;
    return true;
}

bool twPushUTF16(twStringBuf *buf, twChar c) {
    int c_len = twCodepointLengthUTF16(c);
    if (c_len == 0) {
        return false;
    }

    if (buf->length + c_len > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + c_len)) {
            return false;
        }
    }

    int encode_result = twEncodeUTF16(buf->bytes + buf->length, buf->capacity - buf->length, c);
    if (encode_result == 0 || encode_result != c_len) {
        return false;
    }

    buf->length += c_len;
    return true;
}

bool twAppendUTF8(twStringBuf *buf, twString s) {
    if (!twIsValidUTF8(s)) {
        return false;
    }

    if (buf->length + s.length > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + s.length)) {
            return false;
        }
    }

    memcpy(buf->bytes + buf->length, s.bytes, s.length);
    buf->length += s.length;
    return true;
}

bool twAppendUTF16(twStringBuf *buf, twString s) {
    if (!twIsValidUTF16(s)) {
        return false;
    }

    if (buf->length + s.length > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + s.length)) {
            return false;
        }
    }

    memcpy(buf->bytes + buf->length, s.bytes, s.length);
    buf->length += s.length;
    return true;
}

bool twAppendFmtUTF8(twStringBuf *buf, const char * restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char temp[1024];
    int len = vsnprintf(temp, sizeof(temp), fmt, args);

    va_end(args);

    twString to_add = {temp, len};
    return twAppendUTF8(buf, to_add);
}

bool twAppendFmtUTF16(twStringBuf *buf, const char * restrict fmt, ...) {
    // va_list args;
    // va_start(args, fmt);

    // char temp[1024];
    // int len = vsnprintf(temp, sizeof(temp), fmt, args);

    // va_end(args);

    // twString to_add = {temp, len};
    // return twAppendUTF8(buf, to_add);
    return false;
}

bool twAppendLineUTF8(twStringBuf *buf, twString s) {
    bool ok = twAppendUTF8(buf, s);
    if (!ok) {
        return false;
    }

    return twPushUTF8(buf, '\n');
}

bool twAppendLineUTF16(twStringBuf *buf, twString s) {
    bool ok = twAppendUTF16(buf, s);
    if (!ok) {
        return false;
    }

    return twPushUTF16(buf, '\n');
}

bool twInsertUTF8(twStringBuf *buf, size_t idx, twChar c) {
    if (idx >= buf->length) {
        return false;
    }

    int c_len = twCodepointLengthUTF8(c);
    if (c_len == 0) {
        return false;
    }

    if (buf->length + c_len > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + c_len)) {
            return false;
        }
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i + c_len - 1] = buf->bytes[i - 1];
    }

    int encode_result = twEncodeUTF8(buf->bytes + idx, buf->capacity - idx, c);
    if (encode_result == 0 || encode_result != c_len) {
        return false;
    }

    buf->length += c_len;

    return true;
}

bool twInsertUTF16(twStringBuf *buf, size_t idx, twChar c) {
    if (idx >= buf->length) {
        return false;
    }

    int c_len = twCodepointLengthUTF16(c);
    if (c_len == 0) {
        return false;
    }

    if (buf->length + c_len > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + c_len)) {
            return false;
        }
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i + c_len - 1] = buf->bytes[i - 1];
    }

    int encode_result = twEncodeUTF16(buf->bytes + idx, buf->capacity - idx, c);
    if (encode_result == 0 || encode_result != c_len) {
        return false;
    }

    buf->length += c_len;

    return true;   return false;
}

bool twInsertStrUTF8(twStringBuf *buf, size_t idx, twString s) {
    if (idx >= buf->length) {
        return false;
    }

    if (!twIsValidUTF8(s)) {
        return false;
    }

    if (buf->length + s.length > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + s.length)) {
            return false;
        }
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i + s.length - 1] = buf->bytes[i - 1];
    }

    memcpy(buf->bytes + idx, s.bytes, s.length);

    buf->length += s.length;

    return true;
}

bool twInsertStrUTF16(twStringBuf *buf, size_t idx, twString s) {
    if (idx >= buf->length) {
        return false;
    }

    if (!twIsValidUTF16(s)) {
        return false;
    }

    if (buf->length + s.length > buf->capacity) {
        if (!twExtendBuf(buf, buf->length + s.length)) {
            return false;
        }
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i + s.length - 1] = buf->bytes[i - 1];
    }

    memcpy(buf->bytes + idx, s.bytes, s.length);

    buf->length += s.length;

    return true;   
}

void twClear(twStringBuf *buf) {
    buf->length = 0;
}

//
// `twChar` functions
//

int twCodepointLengthUTF8(twChar c) {
    if (c <= 0x7F) {
        return 1;
    } else if (c <= 0x7FF) {
        return 2;
    } else if (c <= 0xFFFF) {
        return 3;
    } else if (c <= 0x10FFFF) {
        return 4;
    } else {
        return 0;
    }
}

int twCodepointLengthUTF16(twChar c) {
    if (c < 0x10000) {
        return 2;
    } else if (c <= 0x10FFFF) {
        return 4;
    } else {
        return 0;
    }
}

int twEncodedCodepointLengthUTF8(char byte1) {
    if ((byte1 & 0x80) == 0) {
        return 1;
    } else if ((byte1 & 0xE0) == 0xC0) {
        return 2;
    } else if ((byte1 & 0xF0) == 0xE0) {
        return 3;
    } else if ((byte1 & 0xF8) == 0xF0) {
        return 4;
    } else {
        return 0;
    }
}

int twEncodedCodepointLengthUTF16(char byte1) {
    if ((byte1 & 0x80) == 0) {
        return 2;
    } else if ((byte1 & 0xE0) == 0xC0) {
        return 4;
    } else {
        return 0;
    }
}

bool twIsSpace(twChar c) {
    switch (c) {
        // ASCII Whitespace
        case 0x0009:
        case 0x000A:
        case 0x000B:
        case 0x000C:
        case 0x000D:
        // Unicode Category: Line Separator
        case 0x2028:
        // Unicode Category: Paragraph Separator
        case 0x2029:
        // Unicode Category: Space Separator
        case 0x0020:
        case 0x00A0:
        case 0x1608:
        case 0x2000:
        case 0x2001:
        case 0x2002:
        case 0x2003:
        case 0x2004:
        case 0x2005:
        case 0x2006:
        case 0x2007:
        case 0x2008:
        case 0x2009:
        case 0x200A:
        case 0x202F:
            return true;
        
        default:
            return false;
    }
}

int twDecodeUTF8(twString bytes, twChar *result) {
    int codepoint_length = twEncodedCodepointLengthUTF8(bytes.bytes[0]);

    switch (codepoint_length) {
        case 1:
            *result = bytes.bytes[0];
            break;
        case 2: {
            twChar byte1 = bytes.bytes[0];
            twChar byte2 = bytes.bytes[1];
            *result = (byte1 & 0x1F) << 6 | (byte2 & 0x3F);
            break;
        }
        case 3: {
            twChar byte1 = bytes.bytes[0];
            twChar byte2 = bytes.bytes[1];
            twChar byte3 = bytes.bytes[2];
            *result = (byte1 & 0x0F) << 12 | (byte2 & 0x3F) << 6 | (byte3 & 0x3F);
            break;
        }
        case 4: {
            twChar byte1 = bytes.bytes[0];
            twChar byte2 = bytes.bytes[1];
            twChar byte3 = bytes.bytes[2];
            twChar byte4 = bytes.bytes[3];
            *result = (byte1 & 0x07) << 18 | (byte2 & 0x3F) << 12 | (byte3 & 0x3F) << 6 | (byte4 & 0x3F);
            break;
        }
        default:
            *result = 0;
            return 0;
    }

    return codepoint_length;
}

int twDecodeUTF16(twString bytes, twChar *result) {
    int codepoint_length = twEncodedCodepointLengthUTF16(bytes.bytes[0]);

    switch (codepoint_length) {
        case 2: {
            *result = (bytes.bytes[0] << 8) | (bytes.bytes[1]);
            break;
        }
        case 4: {
            uint32_t fw = (bytes.bytes[0] << 8) | (bytes.bytes[1]);
            uint32_t sw = (bytes.bytes[2] << 8) | (bytes.bytes[3]);
            *result = ((fw - 0xD800) << 10) + (sw - 0xDC00) + 0x10000;
            break;
        }
        default: {
            *result = 0;
            break;
        }
    }

    return codepoint_length;
}

//
// Iteration functions
//

int twNextASCII(twString *iter, char *result) {
    if (iter->length == 0) {
        if (result) *result = 0;
        return 0;
    }

    if (result) *result = iter->bytes[0];
    iter->bytes++;
    iter->length--;
    return 1;
}

int twNextUTF8(twString *iter, twChar *result) {
    if (iter->length == 0) {
        goto FAIL;
    }

    int codepoint_length = result ? twDecodeUTF8(*iter, result)
                                  : twEncodedCodepointLengthUTF8(iter->bytes[0]);
    if (codepoint_length == 0) {
        goto FAIL;
    }

    *iter = twDrop(*iter, codepoint_length);

    return codepoint_length;

FAIL:
    if (result) *result = 0;
    return 0;
}

int twNextUTF16(twString *iter, twChar *result) {
    if (iter->length == 0) {
        goto FAIL;
    }

    int codepoint_length = result ? twDecodeUTF16(*iter, result)
                                  : twEncodedCodepointLengthUTF16(iter->bytes[0]);
    if (codepoint_length == 0) {
        goto FAIL;
    }

    *iter = twDrop(*iter, codepoint_length);

    return codepoint_length;

FAIL:
    if (result) *result = 0;
    return 0;
}

int twNextRevASCII(twString *iter, char *result) {
    if (iter->length == 0) {
        if (result) *result = 0;
        return 0;
    }

    if (result) *result = iter->bytes[--iter->length];
    return 1;
}

int twNextRevUTF8(twString *iter, twChar *result) {
    if (iter->length == 0) {
        if (result) *result = 0;
        return 0;
    }

    twChar last = twLastUTF8(*iter);
    if (result) *result = last;

    int last_len = twCodepointLengthUTF8(last);
    iter->length -= last_len;
    return last_len;
}

int twNextRevUTF16(twString *iter, twChar *result) {
    if (iter->length == 0) {
        if (result) *result = 0;
        return 0;
    }

    twChar last = twLastUTF16(*iter);
    if (result) *result = last;

    int last_len = twCodepointLengthUTF16(last);
    iter->length -= last_len;
    return last_len;
}

#endif // _TWINE_H_IMPLEMENTATION

#endif // TWINE_IMPLEMENTATION
