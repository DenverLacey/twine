#ifndef _TWINE_H_
#define _TWINE_H_

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
    #define TWLIT(T) T
    #define TWDEFAULT(T) T{}
#else
    #define TWLIT(T) (T)
    #define TWDEFAULT(T) (T){0}
#endif

//
// Types
//

/// Simple pointer-and-length string.
typedef struct twString {
    const char *bytes;
    size_t      length;
} twString;

/// A growable and mutable string buffer capable of dynamic string construction.
typedef struct twStringBuf {
    char  *bytes;
    size_t length;
    size_t capacity;
    size_t max_capacity;
} twStringBuf;

/// An (at least) 32-bit integer used to represent characters as Unicode codepoints.
typedef uint_least32_t twChar;

typedef bool (*twSplitByPredicate)(twChar);

#ifdef __cplusplus
extern "C" {
#endif

//
// C-String functions
//

/// Encodes a unicode codepoint as UTF-8 into a given buffer.
///
/// Paramters:
/// - `bytes` [OUT]: Destination bytes of the encoded character.
/// - `nbytes`: Number of bytes available for encoding.
/// - `c`: The character to encode.
///
/// Returns:
/// The codepoint length of `c`.
int twEncodeUTF8(char *bytes, int nbytes, twChar c);

/// Encodes a unicode codepoint as UTF-16 into a given buffer.
///
/// Parameters:
/// - `bytes` [OUT]: Destination bytes of the encoded character.
/// - `nbytes`: Number of bytes available for encoding.
/// - `c`: The character to encode.
///
/// Returns:
/// The codepoint length of `c`.
int twEncodeUTF16(char *bytes, int nbytes, twChar c);

/// Decodes the first `n` characters in `s`.
///
/// Parameters:
/// - `s`: The UTF-8 encoded string to decode.
/// - `cs` [OUT]: The destination for the decoded characters.
/// - `n`: How many characters to decode. (`cs` assumed to have enough space.)
///
/// Returns:
/// How many characters were decoded. This can be fewer than `n` if
/// something goes wrong while decoding or `s` doesn't have that many
/// characters.
size_t twDecodeUTF8(const char *s, twChar *cs, size_t n);

/// Decodes the first `n` characters in `s`.
///
/// Parameters:
/// - `s`: The UTF-16 encoded string to decode.
/// - `cs` [OUT]: The destination for the decoded characters.
/// - `n`: How many characters to decode. (`cs` assumed to have enough space.)
///
/// Returns:
/// How many characters were decoded. This can be fewer than `n` if
/// something goes wrong while decoding or `s` doesn't have that many
/// characters.
size_t twDecodeUTF16(const char *s, twChar *cs, size_t n);

/// Duplicates and null terminates a string.
char *twDupToC(twString s);

//
// `twString` functions
//

/// Creates a `twString` from a C string.
///
/// Note:
/// Points to the original string and therefore doesn't allocate.
twString twStr(const char *s);

/// Creates a `twString` from a static C string without walking the
///        string to find it's length.
#define twStatic(S) TWLIT(twString){ .bytes = S, .length = sizeof(S) - 1 }

/// Converts a `twString` or `twStringBuf` to a C string.
///
/// Parameters:
/// - `S`: either a `twString` or a `twStringBuf`.
///
/// Note:
/// Doesn't allocate or guarantee null terminator.
#define twToC(S) ((const char *)(S).bytes)

/// Duplicate the contents of a `twString`.
///
/// Parameters:
/// - `s`: The string to duplicate.
///
/// Returns:
/// A newly allocated `twString` with identical contents to `s`.
twString twDup(twString s);

/// Free's the contents of a `twString`.
///
/// Parameters:
/// - `s`: The string to deallocate.
void twFree(twString s);

/// Calculates the number of graphemes in `s`.
///
/// Parameters:
/// - `s`: A UTF-8 encoded string.
size_t twLenUTF8(twString s);

/// Calculates the number of graphemes in `s`.
///
/// Parameters:
/// - `s`: A UTF-16 encoded string.
size_t twLenUTF16(twString s);

/// Checks that `s` is a valid sequence of UTF-16.
bool twIsValidUTF8(twString s);

/// Checks that `s` is a valid sequence of UTF-16.
bool twIsValidUTF16(twString s);

/// Checks if `s` is null.
bool twIsNull(twString s);

/// Checks if `s` is null or has a length of 0.
bool twIsEmpty(twString s);

/// Checks if `s` is null or is all whitespace.
bool twIsAllSpaceASCII(twString s);

/// Checks if `s` is null or is all whitespace.
bool twIsAllSpaceUTF8(twString s);

/// Checks if `s` is null or is all whitespace.
bool twIsAllSpaceUTF16(twString s);

/// Comapres two strings.
///
/// Returns:
/// `true` if the strings are equal, and `false` if not.
bool twEqual(twString a, twString b);

/// Checks if `s` starts with a given `prefix`.
///
/// Returns:
/// `true` if `s` begins with `prefix` and `false` if otherwise.
///
/// Note:
/// It doesn't matter what encoding `s` and `prefix` are so long as they are the
/// same.
bool twStartsWith(twString s, twString prefix);

/// Checks if `s` ends with a given `suffix`.
///
/// Returns:
/// `true` if `s` begins with `suffix` and `false` if otherwise.
///
/// Note:
/// It doesn't matter what encoding `s` and `suffix` are so long as they are the
/// same.
bool twEndsWith(twString s, twString suffix);

/// Checks if `s` contains `needle` as a substring.
///
/// Returns:
/// Offset of `needle` into `s`. Returns `-1` if `needle` does not occur in `s`.
ssize_t twContains(twString s, twString needle);

/// Index of first occurence of `c` in `s`.
///
/// Parameters:
/// - `s`: An ASCII enoded string.
/// - `c`: Character we're looking for.
///
/// Returns:
/// If `c` is found, the index of its first occurence in `s`. Otherwise, returns `-1`.
ssize_t twIndexASCII(twString s, twChar c);

/// Index of first occurence of `c` in `s`.
///
/// Parameters:
/// - `s`: A UTF-8 enoded string.
/// - `c`: Character we're looking for.
///
/// Returns:
/// If `c` is found, the index of its first occurence in `s`. Otherwise, returns `-1`.
ssize_t twIndexUTF8(twString s, twChar c);

/// Index of first occurence of `c` in `s`.
///
/// Parameters:
/// - `s`: A UTF-16 enoded string.
/// - `c`: Character we're looking for.
///
/// Returns:
/// If `c` is found, the index of its first occurence in `s`. Otherwise, returns `-1`.
ssize_t twIndexUTF16(twString s, twChar c);

/// Byte offset of first occurence of `c` in `s`.
///
/// Parameters:
/// - `s`: An ASCII enoded string.
/// - `c`: Character we're looking for.
///
/// Returns:
/// If `c` is found, the byte offset of its first occurence in `s`. Otherwise, returns `-1`.
ssize_t twOffsetASCII(twString s, twChar c);

/// Byte offset of first occurence of `c` in `s`.
///
/// Parameters:
/// - `s`: A UTF-8 enoded string.
/// - `c`: Character we're looking for.
///
/// Returns:
/// If `c` is found, the byte offset of its first occurence in `s`. Otherwise, returns `-1`.
ssize_t twOffsetUTF8(twString s, twChar c);

/// Byte offset of first occurence of `c` in `s`.
///
/// Parameters:
/// - `s`: A UTF-16 enoded string.
/// - `c`: Character we're looking for.
///
/// Returns:
/// If `c` is found, the byte offset of its first occurence in `s`. Otherwise, returns `-1`.
ssize_t twOffsetUTF16(twString s, twChar c);

/// Splits a string by a character. (The split character is not included.)
///
/// Parameters:
/// - `s`: The ASCII encoded string to split.
/// - `c`: The charcter to search for.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitASCII(twString s, char c, twString *remainder);

/// Splits a string by a character. (The split character is not included.)
///
/// Parameters:
/// - `s`: The UTF-8 encoded string to split.
/// - `c`: The charcter to search for.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitUTF8(twString s, twChar c, twString *remainder);

/// Splits a string by a character. (The split character is not included.)
///
/// Parameters:
/// - `s`: The UTF-16 encoded string to split.
/// - `c` The charcter to search for.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitUTF16(twString s, twChar c, twString *remainder);

/// Splits a string by a predicate. (The split character is not included.)
/// 
/// Parameters:
/// - `s`: The ASCII encoded string to split.
/// - `pred`: The predicate function used to split the string.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitByASCII(twString s, twSplitByPredicate pred, twString *remainder);

/// Splits a string by a predicate. (The split character is not included.)
/// 
/// Parameters:
/// - `s`: The UTF-8 encoded string to split.
/// - `pred`: The predicate function used to split the string.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitByUTF8(twString s, twSplitByPredicate pred, twString *remainder);

/// Splits a string by a predicate. (The split character is not included.)
///
/// Parameters:
/// - `s`: The UTF-16 encoded string to split.
/// - `pred`: The predicate function used to split the string.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitByUTF16(twString s, twSplitByPredicate pred, twString *remainder);

/// Splits a string until `pred` returns false. (The split character is not included.)
///
/// Parameters:
/// - `s`: The ASCII encoded string to split.
/// - `pred`: The predicate function used to split the string.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitWhileASCII(twString s, twSplitByPredicate pred, twString *remainder);

/// Splits a string until `pred` returns false. (The split character is not included.)
///
/// Parameters:
/// - `s`: The UTF-8 encoded string to split.
/// - `pred`: The predicate function used to split the string.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitWhileUTF8(twString s, twSplitByPredicate pred, twString *remainder);

/// Splits a string by a predicate. (The split character is not included.)
///
/// Paramters:
/// - `s`: The UTF-16 encoded string to split.
/// - `pred`: The predicate function used to split the string.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the first instance of `c`.
twString twSplitWhileUTF16(twString s, twSplitByPredicate pred, twString *remainder);

/// Splits a string at the first occurence of any character in `cs`.
///
/// Parameters:
/// - `s`: The ASCII encoded string to split.
/// - `cs`: List of split characters.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the split point.
///
/// Note:
/// The split character is not included in the return value *and* `remainder`.
twString twSplitAnyASCII(twString s, const char *restrict cs, twString *remainder);

/// Splits a string at the first occurence of any character in `cs`.
///
/// Parameters:
/// - `s`: The UTF-8 encoded string to split.
/// - `cs`: List of split characters.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the split point.
///
/// Note:
/// The split character is not included in the return value *and* `remainder`.
twString twSplitAnyUTF8(twString s, const char *restrict cs, twString *remainder);

/// Splits a string at the first occurence of any character in `cs`.
///
/// Parameters:
/// - `s`: The UTF-16 encoded string to split.
/// - `cs`: List of split characters.
/// - `remainder` [OUT, OPT]: The rest of the string after the split character.
///
/// Returns:
/// A string slice of the contents of `s` before the split point.
///
/// Note:
/// The split character is not included in the return value *and* `remainder`.
twString twSplitAnyUTF16(twString s, const char *restrict cs, twString *remainder);

/// The first character of `s` as a `twString`.
///
/// Paramters:
/// - `s`: A ASCII encoded string.
///
/// Returns:
/// The in-place bytes of the first character.
twString twHeadASCII(twString s);

/// The first character of `s` as a `twString`.
///
/// Paramters:
/// - `s`: A UTF-8 encoded string.
///
/// Returns:
/// The in-place bytes of the first character.
twString twHeadUTF8(twString s);

/// The first character of `s` as a `twString`.
///
/// Paramters:
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// The in-place bytes of the first character.
twString twHeadUTF16(twString s);

/// Paramters:
/// - `s`: A ASCII encoded string.
///
/// Returns:
/// The tail of `s` (All except the first character.)
twString twTailASCII(twString s);

/// Paramters:
/// - `s`: A UTF-8 encoded string.
///
/// Returns:
/// The tail of `s` (All except the first character.)
twString twTailUTF8(twString s);

/// Paramters:
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// The tail of `s` (All except the first character.)
twString twTailUTF16(twString s);

/// The first character in `s`.
///
/// Paramters:
/// - `s`: An ASCII encoded string.
///
/// Returns:
/// If `s` isn't empty, the first character in `s`. Otherwise, returns 0.
/// Check codepoint length to determine if the first character was
/// actually 0 or if there is no first character.
twChar twFirstASCII(twString s);

/// The first character in `s`.
///
/// Paramters:
/// - `s`: An UTF-8 encoded string.
///
/// Returns:
/// If `s` isn't empty, the first character in `s`. Otherwise, returns 0.
/// Check codepoint length to determine if the first character was
/// actually 0 or if there is no first character.
twChar twFirstUTF8(twString s);

/// The first character in `s`.
///
/// Paramters:
/// - `s`: An UTF-16 encoded string.
///
/// Returns:
/// If `s` isn't empty, the first character in `s`. Otherwise, returns 0.
/// Check codepoint length to determine if the first character was
/// actually 0 or if there is no first character.
twChar twFirstUTF16(twString s);

/// The last character in `s`.
///
/// Paramters:
/// - `s`: An ASCII string.
///
/// Returns:
/// If `s` isn't empty, the last character in `s`. Otherwise, it returns 0.
/// Check length to determine if the last character was actually 0 or if
/// there is no last character.
twChar twLastASCII(twString s);

/// The last character in `s`.
///
/// Paramters:
/// - `s`: A UTF-8 string.
///
/// Returns:
/// If `s` isn't empty, the last character in `s`. Otherwise, it returns 0.
/// Check length to determine if the last character was actually 0 or if
/// there is no last character.
twChar twLastUTF8(twString s);

/// The last character in `s`.
///
/// Paramters:
/// - `s`: A UTF-16 string.
///
/// Returns:
/// If `s` isn't empty, the last character in `s`. Otherwise, it returns 0.
/// Check length to determine if the last character was actually 0 or if
/// there is no last character.
twChar twLastUTF16(twString s);

/// Drop the first `n` bytes of `s`.
///
/// Parameters:
/// - `n`: Number of bytes to drop.
///
/// Returns:
/// New `twString` with first `n` bytes of `s` removed.
twString twDrop(twString s, size_t n);

/// Truncates `s` to a length of `n`. (If `s` is shorter than `n`, the
/// empty string is returned.)
twString twTrunc(twString s, size_t n);

/// Removes all leading whitespace characters.
///
/// Parameters:
/// - `s`: A ASCII encoded string.
///
/// Returns:
/// A `twString` with leading whitesapce removed. (Points to original data.)
twString twTrimLeftASCII(twString s);

/// Removes all leading whitespace characters.
///
/// Parameters:
/// - `s`: A UTF-8 encoded string.
///
/// Returns:
/// A `twString` with leading whitesapce removed. (Points to original data.)
twString twTrimLeftUTF8(twString s);

/// Removes all leading whitespace characters.
///
/// Paramters:
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// A `twString` with leading whitesapce removed. (Points to original data.)
twString twTrimLeftUTF16(twString s);

/// Removes all trailing whitespace characters.
///
/// Parameters:
/// - `s`: A ASCII encoded string.
///
/// Returns:
/// A `twString` with trailing whitesapce removed. (Points to original data.)
twString twTrimRightASCII(twString s);

/// Removes all trailing whitespace characters.
///
/// Parameters:
/// - `s`: A UTF-8 encoded string.
///
/// Returns:
/// A `twString` with trailing whitesapce removed. (Points to original data.)
twString twTrimRightUTF8(twString s);

/// Removes all trailing whitespace characters.
///
/// Parameters:
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// A `twString` with trailing whitesapce removed. (Points to original data.)
twString twTrimRightUTF16(twString s);

/// Removes all leading and trailing whitespace characters.
///
/// Parameters:
/// - `s`: A ASCII encoded string.
///
/// Returns:
/// A `twString` with leading and trailing whitesapce removed. (Points to original data.)
twString twTrimASCII(twString s);

/// Removes all leading and trailing whitespace characters.
///
/// Parameters:
/// - `s`: A UTF-8 encoded string.
///
/// Returns:
/// A `twString` with leading and trailing whitesapce removed. (Points to original data.)
twString twTrimUTF8(twString s);

/// Removes all leading and trailing whitespace characters.
///
/// Parameters:
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// A `twString` with leading and trailing whitesapce removed. (Points to original data.)
twString twTrimUTF16(twString s);

//
// `twStringBuf` functions
//

/// Creates a `twStringBuf` out of a static buffer.
///
/// Parameters:
/// - `S`: The buffer to wrap.
///
/// Returns:
/// A `twStringBuf` whose memory is preallocated in a static buffer.
#define twStaticBuf(S) TWLIT(twStringBuf){ \
    .bytes = S, \
    .length = 0, \
    .capacity = sizeof(S), \
    .max_capacity = sizeof(S) \
}

/// A new zero-ed `twStringBuf`.
twStringBuf twNewBuf(void);

/// A new `twStringBuf` with a max capacity.
///
/// Parameters:
/// - `max_capacity`: The maximum capacity of the new `twStringBuf`.
///
/// Note:
/// This function doesn't allocate.
twStringBuf twNewBufWithMaxCapacity(size_t max_capacity);

/// A new `twStringBuf` initialized with a newly allocated buffer of
/// `capacity` bytes.
///
/// Parameters:
/// - `capacity`: Number of bytes to initalize the buffer with.
twStringBuf twNewBufWithCapacity(size_t capacity);

/// Deallocates the contents of a `twStringBuf`.
///
/// Paramters:
/// - `buf`: The buffer to deallocate.
void twFreeBuf(twStringBuf buf);

/// Creates a `twString` from a `twStringBuf`.
twString twBufToString(twStringBuf buf);

/// Resizes the buffer except when that would exceed `max_capacity`.
///
/// Parameters:
/// - `buf`: The string buffer to resize.
/// - `new_size`: The new desired size of the buffer.
///
/// Returns:
/// `true` if buffer was resized successfully. Otherwise, returns `false`.
///
/// Note:
/// If `new_size` exceeds the buffer's max capacity, `buf` remains the same size.
bool twResizeBuf(twStringBuf *buf, size_t new_size);

/// Extend the buffer except when that would exceed `max_capacity`.
///
/// Parameters:
/// - `buf`: The string buffer to extend.
/// - `extension`: How many bytes to extend `buf` by.
///
/// Returns:
/// `true` if buffer was extended successfully. Otherwise, returns `false`.
///
/// Note:
/// If the new size exceeds the buffer's max capacity, `buf` remains the same size.
bool twExtendBuf(twStringBuf *buf, size_t extension);

/// Adds a character to the end of a string buffer.
///
/// Parameters:
/// - `s`: An ASCII encdoded string buffer.
///
/// Returns:
/// `true` if character was added succcessfully, otherwise returns `false`.
bool twPushASCII(twStringBuf *buf, char c);

/// Adds a character to the end of a string buffer.
///
/// Parameters:
/// - `s`: A UTF-8 encdoded string buffer.
///
/// Returns:
/// `true` if character was added succcessfully, otherwise returns `false`.
bool twPushUTF8(twStringBuf *buf, twChar c);

/// Adds a character to the end of a string buffer.
///
/// Parameters:
/// - `buf`: A UTF-16 encdoded string buffer.
///
/// Returns:
/// `true` if character was added succcessfully, otherwise returns `false`.
bool twPushUTF16(twStringBuf *buf, twChar c);

/// Appends a string to the end of a string buffer.
///
/// Parameters:
/// - `buf`: An ASCII encoded string buffer.
/// - `s`: An ASCII encoded string.
///
/// Returns:
/// `true` if string was added successfully. Otherwise, returns `false`.
bool twAppendASCII(twStringBuf *buf, twString s);

/// Appends a string to the end of a string buffer.
///
/// Parameters:
/// - `buf`: A UTF-8 encoded string buffer.
/// - `s`: A UTF-8 encoded string.
///
/// Returns:
/// `true` if string was added successfully. Otherwise, returns `false`.
bool twAppendUTF8(twStringBuf *buf, twString s);

/// Appends a string to the end of a string buffer.
///
/// Parameters:
/// - `buf`: A UTF-16 encoded string buffer.
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// `true` if string was added successfully. Otherwise, returns `false`.
bool twAppendUTF16(twStringBuf *buf, twString s);

/// Appends a formatted string to the end of a string buffer.
///
/// Parameters:
/// - `buf`: A ASCII encoded string buffer.
///
/// Returns:
/// `true` if string was added sucessfully. Otherwise, returns `false`.
bool twAppendFmtASCII(twStringBuf *buf, const char *__restrict fmt, ...);

/// Appends a formatted string to the end of a string buffer.
///
/// Parameters:
/// - `buf`: A UTF-8 encoded string buffer.
///
/// Returns:
/// `true` if string was added sucessfully. Otherwise, returns `false`.
bool twAppendFmtUTF8(twStringBuf *buf, const char *__restrict fmt, ...);

/// Appends a formatted string to the end of a string buffer.
/// buf A UTF-16 encoded string buffer.
/// True if string was added sucessfully. Otherwise, returns false.
bool twAppendFmtUTF16(twStringBuf *buf, const char *__restrict fmt, ...);

/// Appends a string to the end of a string buffer and adds a newline.
///
/// Parameters:
/// - `buf`: An ASCII encoded string buffer.
/// - `s` An ASCII encoded string.
///
/// Returns:
/// `true` if string was added successfully. Otherwise, returns `false`.
bool twAppendLineASCII(twStringBuf *buf, twString s);

/// Appends a string to the end of a string buffer and adds a newline.
///
/// Parameters:
/// - `buf`: A UTF-8 encoded string buffer.
/// - `s` A UTF-8 encoded string.
///
/// Returns:
/// `true` if string was added successfully. Otherwise, returns `false`.
bool twAppendLineUTF8(twStringBuf *buf, twString s);

/// Appends a string to the end of a string buffer and adds a newline.
///
/// Parameters:
/// - `buf`: A UTF-16 encoded string buffer.
/// - `s`: A UTF-16 encoded string.
///
/// Returns:
/// `true` if string was added successfully. Otherwise, returns `false`.
bool twAppendLineUTF16(twStringBuf *buf, twString s);

/// Inserts a character into the buffer at a byte index.
///
/// Parameters:
/// - `buf`: The ASCII encoded buffer to insert the character into.
/// - `idx`: The byte position to insert the character.
/// - `c`: The character.
///
/// Returns:
/// `true` if the character was inserted successfully. Otherwise, returns `false`.
bool twInsertASCII(twStringBuf *buf, size_t idx, char c);

/// Inserts a character into the buffer at a byte index.
///
/// Parameters:
/// - `buf`: The UTF-8 encoded buffer to insert the character into.
/// - `idx`: The byte position to insert the character.
/// - `c`: The character.
///
/// Returns:
/// `true` if the character was inserted successfully. Otherwise, returns `false`.
bool twInsertUTF8(twStringBuf *buf, size_t idx, twChar c);

/// Inserts a character into the buffer at a byte index.
///
/// Parameters:
/// - `buf`: The UTF-16 encoded buffer to insert the character into.
/// - `idx`: The byte position to insert the character.
/// - `c`: The character.
///
/// Returns:
/// `true` if the character was inserted successfully. Otherwise, returns `false`.
bool twInsertUTF16(twStringBuf *buf, size_t idx, twChar c);

/// Inserts a string into the buffer at a byte index.
///
/// Parameters:
/// - `buf`: The ASCII encoded buffer to insert the string into.
/// - `idx`: The position to insert the string.
/// - `s`: The ASCII encoded string.
///
/// Returns:
/// `true` if the string was inserted successfully. Otherwise, returns `false`.
bool twInsertStrASCII(twStringBuf *buf, size_t idx, twString s);

/// Inserts a string into the buffer at a byte index.
///
/// Parameters:
/// - `buf`: The UTF-8 encoded buffer to insert the string into.
/// - `idx`: The byte position to insert the string.
/// - `s`: The UTF-8 encoded string.
///
/// Returns:
/// `true` if the string was inserted successfully. Otherwise, returns `false`.
bool twInsertStrUTF8(twStringBuf *buf, size_t idx, twString s);

/// Inserts a string into the buffer at a byte index.
///
/// Parameters:
/// - `buf`: The UTF-16 encoded buffer to insert the string into.
/// - `idx`: The byte position to insert the string.
/// - `s`: The UTF-16 encoded string.
///
/// Returns:
/// `true` if the string was inserted successfully. Otherwise, returns `false`.
bool twInsertStrUTF16(twStringBuf *buf, size_t idx, twString s);

/// Concatenates several UTF-8 encoded strings together using a string buffer.
///
/// Parameters:
/// - `B`: A `twStringBuf`, Buffer used to concatenate the strings.
/// - `...`: Many `twString`s, UTF-8 encoded strings to concatenate.
///
/// Returns:
/// `true` if strings are successfully concatenated. Returns `false` on an error.
#define twConcatASCII(B, ...) __twConcatASCII(B, __VA_ARGS__, TWLIT(twString){(const char*)-1, (size_t)-1})
bool __twConcatASCII(twStringBuf *buf, ...);

/// Concatenates several UTF-8 encoded strings together using a string buffer.
///
/// Parameters:
/// - `B`: A `twStringBuf`, Buffer used to concatenate the strings.
/// - `...`: Many `twString`s, UTF-8 encoded strings to concatenate.
///
/// Returns:
/// `true` if strings are successfully concatenated. Returns `false` on an error.
#define twConcatUTF8(B, ...) __twConcatUTF8(B, __VA_ARGS__, TWLIT(twString){(const char*)-1, (size_t)-1})
bool __twConcatUTF8(twStringBuf *buf, ...);

/// Concatenates several UTF-16 encoded strings together using a string buffer.
///
/// Parameters:
/// - `B`: A `twStringBuf`, Buffer used to concatenate the strings.
/// - `...`: Many `twString`s, UTF-16 encoded strings to concatenate.
///
/// Returns:
/// `true` if strings are successfully concatenated. Returns `false` on an error.
///
bool __twConcatUTF16(twStringBuf *buf, ...);

/// Removes every character from a string buffer.
void twClear(twStringBuf *buf);

/// Does `buf` have a maximum capacity.
///
/// Parameters:
/// - `buf`: The buffer in question.
///
/// Returns:
/// `true` if `buf` has a maximum capacity that cannot be exceeded.
bool twHasMaxCapacity(twStringBuf buf);

//
// `twChar` functions
//

/// How many bytes required to encode a codepoint in UTF-8.
int twCodepointLengthUTF8(twChar c);

/// How many bytes required to encode a codepoint in UTF-16.
int twCodepointLengthUTF16(twChar c);

/// Number of bytes for an already encoded character.
///
/// Parameters:
/// - `byte1`: The first byte of the UTF-8 encoded character.
int twEncodedCodepointLengthUTF8(char byte1);

/// Number of bytes for an already encoded UTF-16 character.
///
/// Parameters:
/// - `byte1` The first byte of the UTF-16 encoded character.
int twEncodedCodepointLengthUTF16(char byte1);

/// Is `c` a white space character.
///
/// Note:
/// Considers ASCII whitespace, and characters in the `Line_Separator`,
/// `Paragraph_Separator`, and `Space_Separator` character cateogies a
/// white space character.
bool twIsSpace(twChar c);

//
// Iteration functions
//

/// Can be used to get the next ASCII character or to iterate through
/// many ASCII characters in a string.
///
/// Parameters:
/// - `iter`: The string is marched along its charcters as this function is used.
/// - `result` [OUT, OPT]: The next character in the string `iter`.
///
/// Returns:
/// The number of bytes iterated over. If there is no more characters or an error
/// occurred, 0 is returned.
int twNextASCII(twString *iter, char *result);

/// Can be used to get the next UTF-8 character or to iterate through
/// many UTF-8 characters in a string.
///
/// Parameters:
/// - `iter`: The string is marched along its charcters as this function is used.
/// - `result` [OUT, OPT]: The next character in the string `iter`.
///
/// Returns:
/// The number of bytes iterated over. If there is no more characters or an error
/// occurred, 0 is returned.
int twNextUTF8(twString *iter, twChar *result);

/// Can be used to get the next UTF-16 character or to iterate through
/// many UTF-16 characters in a string.
///
/// Parameters:
/// - `iter`: The string is marched along its charcters as this function is used.
/// - `result` [OUT, OPT]: The next character in the string `iter`.
///
/// Returns:
/// The number of bytes iterated over. If there is no more characters or an error
/// occurred, 0 is returned.
int twNextUTF16(twString *iter, twChar *result);

/// Can be used to get the last ASCII character or to iterate backwards
/// through many ASCII characters in a string.
///
/// Parameters:
/// - `iter`: The string is marched backwards along its characters as this function is used.
/// - `result` [OUT, OPT]: The next character in the sequence. The last character in `iter`.
///
/// Returns:
/// The number of bytes iterated over. If there is no more character or an error occurred,
/// 0 is returned.
int twNextRevASCII(twString *iter, char *result);

/// Can be used to get the last UTF-8 character or to iterate backwards through
/// many UTF-8 characters in a string.
///
/// Parameters:
/// - `iter`: The string is marched backwards along its characters as this function is used.
/// - `result` [OUT, OPT]: The next character in the sequence. The last character in `iter`.
///
/// Returns:
/// The number of bytes iterated over. If there is no more character or an error occurred,
/// 0 is returned.
int twNextRevUTF8(twString *iter, twChar *result);

/// Can be used to get the last UTF-16 character or to iterate backwards through
/// many UTF-16 characters in a string.
///
/// Parameters:
/// - `iter`: The string is marched backwards along its characters as this function is used.
/// - `result` [OUT, OPT]: The next character in the sequence. The last character in `iter`.
///
/// Returns:
/// The number of bytes iterated over. If there is no more character or an error occurred,
/// 0 is returned.
int twNextRevUTF16(twString *iter, twChar *result);

//
// Printf Niceties
//

/// Format specifer string for `twString` and `twStringBuf`.
#define twFmt "%.*s"

/// Correctly passes `S` to a printf-like function.
///
/// Parameters:
/// - `S`: Either a `twString` or a `twStringBuf`.
#define twArg(S) (int)(S).length, (S).bytes

#ifdef __cplusplus
}
#endif

/// Define this macro to provide non-prefixed versions of definitions
#ifdef TWINE_USING_NAMESPACE
typedef twString String;
typedef twStringBuf StringBuf;
typedef twChar Char;
typedef twSplitByPredicate SplitByPredicate;
#define EncodeUTF8 twEncodeUTF8
#define EncodeUTF16 twEncodeUTF16
#define DecodeUTF8 twDecodeUTF8
#define DecodeUTF16 twDecodeUTF16
#define DupToC twDupToC
#define Str twStr
#define Static twStatic
#define ToC twToC
#define Dup twDup
#define Free twFree
#define LenUTF8 twLenUTF8
#define LenUTF16 twLenUTF16
#define IsValidUTF8 twIsValidUTF8
#define IsValidUTF16 twIsValidUTF16
#define IsNull twIsNull
#define IsEmpty twIsEmpty
#define IsAllSpaceASCII twIsAllSapceASCII
#define IsAllSpaceUTF8 twIsAllSapceUTF8
#define IsAllSpaceUTF16 twIsAllSapceUTF16
#define Equal twEqaul
#define StartsWith twStartsWith
#define EndsWith twEndsWith
#define Contains twContains
#define IndexASCII twIndexASCII
#define IndexUTF8 twIndexUTF8
#define IndexUTF16 twIndexUTF16
#define OffsetUTF8 twOffsetUTF8
#define OffsetUTF16 twOffsetUTF16
#define SplitASCII twSplitASCII
#define SplitUTF8 twSplitUTF8
#define SplitUTF16 twSplitUTF16
#define SplitByASCII twSplitByASCII
#define SplitByUTF8 twSplitByUTF8
#define SplitByUTF16 twSplitByUTF16
#define SplitWhileASCII twSplitWhileASCII
#define SplitWhileUTF8 twSplitWhileUTF8
#define SplitWhileUTF16 twSplitWhileUTF16
#define SplitAnyASCII twSplitAnyASCII
#define SplitAnyUTF8 twSplitAnyUTF8
#define SplitAnyUTF16 twSplitAnyUTF16
#define HeadASCII twHeadASCII
#define HeadUTF8 twHeadUTF8
#define HeadUTF16 twHeadUTF16
#define TailASCII twTailASCII
#define TailUTF8 twTailUTF8
#define TailUTF16 twTailUTF16
#define FirstASCII twFirstASCII
#define FirstUTF8 twFirstUTF8
#define FirstUTF16 twFirstUTF16
#define LastASCII twLastASCII
#define LastUTF8 twLastUTF8
#define LastUTF16 twLastUTF16
#define Drop twDrop
#define Trunc twTrunc
#define TrimLeftASCII twTrimLeftASCII
#define TrimLeftUTF8 twTrimLeftUTF8
#define TrimLeftUTF16 twTrimLeftUTF16
#define TrimRightASCII twTrimRightASCII
#define TrimRightUTF8 twTrimRightUTF8
#define TrimRightUTF16 twTrimRightUTF16
#define TrimASCII twTrimASCII
#define TrimUTF8 twTrimUTF8
#define TrimUTF16 twTrimUTF16
#define StaticBuf twStaticBuf
#define NewBuf twNewBuf
#define NewBufWithMaxCapacity twNewBufWithMaxCapacity
#define NewBufWithCapacity twNewBufWithCapacity
#define FreeBuf twFreeBuf
#define BufToString twBufToString
#define ResizeBuf twResizeBuf
#define ExtendBuf twExtendBuf
#define PushASCII twPushASCII
#define PushUTF8 twPushUTF8
#define PushUTF16 twPushUTF16
#define AppendASCII twAppendASCII
#define AppendUTF8 twAppendUTF8
#define AppendUTF16 twAppendUTF16
#define AppendFmtASCII twAppendFmtASCII
#define AppendFmtUTF8 twAppendFmtUTF8
#define AppendFmtUTF16 twAppendFmtUTF16
#define AppendLineASCII twAppendLineASCII
#define AppendLineUTF8 twAppendLineUTF8
#define AppendLineUTF16 twAppendLineUTF16
#define InsertASCII twInsertASCII
#define InsertUTF8 twInsertUTF8
#define InsertUTF16 twInsertUTF16
#define InsertStrASCII twInsertStrASCII
#define InsertStrUTF8 twInsertStrUTF8
#define InsertStrUTF16 twInsertStrUTF16
#define ConcatASCII twConcatASCII
#define ConcatUTF8 twConcatUTF8
#define ConcatUTF16 twConcatUTF16
#define Clear twClear
#define HasMaxCapacity twHasMaxCapacity
#define CodepointLengthUTF8 twCodepointLengthUTF8
#define CodepointLengthUTF16 twCodepointLengthUTF16
#define EncodedCodepointLengthUTF8 twEncodedCodepointLengthUTF8
#define EncodedCodepointLengthUTF16 twEncodedCodepointLengthUTF16
#define IsSpace twIsSpace
#define NextASCII twNextASCII
#define NextUTF8 twNextUTF8
#define NextUTF16 twNextUTF16
#define NextRevASCII twNextRevASCII
#define NextRevUTF8 twNextRevUTF8
#define NextRevUTF16 twNextRevUTF16
#define Fmt twFmt
#define Arg twArg
#endif

#endif // _TWINE_H_

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                              IMPLEMENTATION                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifdef TWINE_IMPLEMENTATION

#ifndef _TWINE_H_IMPLEMENTATION_
#define _TWINE_H_IMPLEMENTATION_

#ifndef twAlloc
#define twAlloc(n) malloc(n)
#endif

#ifndef twDealloc
#define twDealloc(p) free(p)
#endif

#ifndef twRealloc
#define twRealloc(p, nsz) realloc(p, nsz)
#endif

#ifdef __cplusplus
extern "C" {
#endif

//
// C-String functions
//

int twEncodeUTF8(char *bytes, int nbytes, twChar c) {
    int c_len = twCodepointLengthUTF8(c);
    if (c_len == 0 || c_len > nbytes) {
        return 0;
    }

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
    int c_len = twEncodedCodepointLengthUTF16(bytes[0]);
    if (c_len == 0 || c_len > nbytes) {
        return 0;
    }

    switch (c_len) {
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

    return c_len;
}

size_t twDecodeUTF8(const char *s, twChar *cs, size_t n) {
    size_t ndecoded = 0; 

    while (n > 0 && s[0] != 0) {
        int codepoint_length = twEncodedCodepointLengthUTF8(s[0]);

        switch (codepoint_length) {
            case 1:
                cs[ndecoded] = s[0];
                break;
            case 2: {
                twChar byte1 = s[0];
                twChar byte2 = s[1];
                cs[ndecoded] = (byte1 & 0x1F) << 6 | (byte2 & 0x3F);
                break;
            }
            case 3: {
                twChar byte1 = s[0];
                twChar byte2 = s[1];
                twChar byte3 = s[2];
                cs[ndecoded] = (byte1 & 0x0F) << 12 | (byte2 & 0x3F) << 6 | (byte3 & 0x3F);
                break;
            }
            case 4: {
                twChar byte1 = s[0];
                twChar byte2 = s[1];
                twChar byte3 = s[2];
                twChar byte4 = s[3];
                cs[ndecoded] = (byte1 & 0x07) << 18 | (byte2 & 0x3F) << 12 | (byte3 & 0x3F) << 6 | (byte4 & 0x3F);
                break;
            }
            default:
                goto RETURN;
        }

        n--;
        ndecoded++;
        s += codepoint_length;
    }

RETURN:
    return ndecoded;
}

size_t twDecodeUTF16(const char *s, twChar *cs, size_t n) {
    size_t ndecoded = 0; 

    while (n > 0 && s[0] != 0) {
        int codepoint_length = twEncodedCodepointLengthUTF16(s[0]);

        switch (codepoint_length) {
            case 2: {
                cs[ndecoded] = (s[0] << 8) | (s[1]);
                break;
            }
            case 4: {
                uint32_t fw = (s[0] << 8) | (s[1]);
                uint32_t sw = (s[2] << 8) | (s[3]);
                cs[ndecoded] = ((fw - 0xD800) << 10) + (sw - 0xDC00) + 0x10000;
                break;
            }
            default: {
                goto RETURN;
            }
        }

        n--;
        ndecoded++;
        s += codepoint_length;
    }

RETURN:
    return ndecoded;
}

char *twDupToC(twString s) {
    char *cstr = twAlloc(s.length + 1);
    if (cstr == NULL) {
        return NULL;
    }

    memcpy(cstr, s.bytes, s.length);
    cstr[s.length] = 0;

    return cstr;
}

//
// `twString` functions
//

twString twStr(const char *s) {
    if (s) {
        return TWLIT(twString){ .bytes = s, .length = strlen(s) };
    } else {
        return TWDEFAULT(twString);
    }
}

twString twDup(twString s) {
    char *new_bytes = twAlloc(s.length);
    if (new_bytes == NULL) {
        return TWDEFAULT(twString);
    }
    
    memcpy(new_bytes, s.bytes, s.length);

    return TWLIT(twString){
        .bytes = new_bytes,
        .length = s.length
    };
}

void twFree(twString s) {
    if (twIsNull(s)) return;
    twDealloc((void*)s.bytes);
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

bool twIsNull(twString s) {
    return s.bytes == NULL;
}

bool twIsEmpty(twString s) {
    return (s.length == 0) || twIsNull(s);
}

bool twIsAllSpaceASCII(twString s) {
    if (twIsEmpty(s)) {
        return true;
    }

    twString t = twTrimLeftASCII(s);
    return twIsEmpty(t);
}

bool twIsAllSpaceUTF8(twString s) {
    if (twIsEmpty(s)) {
        return true;
    }

    twString t = twTrimLeftUTF8(s);
    return twIsEmpty(t);
}

bool twIsAllSpaceUTF16(twString s) {
    if (twIsEmpty(s)) {
        return true;
    }

    twString t = twTrimLeftUTF16(s);
    return twIsEmpty(t);
}

bool twEqual(twString a, twString b) {
    return a.length == b.length && (memcmp(a.bytes, b.bytes, a.length) == 0);
}

bool twStartsWith(twString s, twString prefix) {
    twString _s = twTrunc(s, prefix.length);
    return twEqual(_s, prefix);
}

bool twEndsWith(twString s, twString suffix) {
    if (s.length < suffix.length) return false;
    twString _s = twDrop(s, s.length - suffix.length);
    return twEqual(_s, suffix);
}

ssize_t twContains(twString s, twString needle) {
    if (s.length < needle.length) {
        return -1;
    }

    for (ssize_t i = 0; i < (ssize_t)s.length - (ssize_t)needle.length; i++) {
        if (memcmp(&s.bytes[i], needle.bytes, needle.length) == 0) {
            return i;
        }
    }

    return -1;
}

ssize_t twIndexASCII(twString s, twChar c) {
    for (ssize_t i = 0; i < (ssize_t)s.length; i++) {
        if ((twChar)s.bytes[i] == c) {
            return i;
        }
    }
    return -1;
}

ssize_t twIndexUTF8(twString s, twChar c) {
    twChar si;
    ssize_t i = 0;
    while (twNextUTF8(&s, &si)) {
        if (si == c) {
            return i;
        }
        i++;
    }
    return -1;
}

ssize_t twIndexUTF16(twString s, twChar c) {
    twChar si;
    ssize_t i = 0;
    while (twNextUTF16(&s, &si)) {
        if (si == c) {
            return i;
        }
        i++;
    }
    return -1;
}

ssize_t twOffsetASCII(twString s, twChar c) {
    return twIndexASCII(s, c);
}

ssize_t twOffsetUTF8(twString s, twChar c) {
    twString t = twSplitUTF8(s, c, NULL);
    if (twEqual(t, s)) {
        return -1;
    }

    return (ssize_t)(t.bytes - s.bytes);
}

ssize_t twOffsetUTF16(twString s, twChar c) {
    twString t = twSplitUTF16(s, c, NULL);
    if (twEqual(t, s)) {
        return -1;
    }

    return (ssize_t)(t.bytes - s.bytes);
}

twString twSplitASCII(twString s, char c, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    char cur;
    while (twNextASCII(&s, &cur) != 0 && cur != c) {
        result.length++;
    }

    if (remainder) *remainder = s;
    return result;
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

twString twSplitByASCII(twString s, twSplitByPredicate pred, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    char cur;
    while (twNextASCII(&s, &cur) > 0 && !pred(cur)) {
        result.length++;
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitByUTF8(twString s, twSplitByPredicate pred, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF8(&s, &cur)) > 0 && !pred(cur)) {
        result.length += curlen;
        if (s.length <= 0) {
            break;
        }
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitByUTF16(twString s, twSplitByPredicate pred, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF16(&s, &cur)) > 0 && !pred(cur)) {
        result.length += curlen;
        if (s.length <= 0) {
            break;
        }
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitWhileASCII(twString s, twSplitByPredicate pred, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    while (twNextUTF8(&s, &cur) > 0 && pred(cur)) {
        result.length++;
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitWhileUTF8(twString s, twSplitByPredicate pred, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF8(&s, &cur)) > 0 && pred(cur)) {
        result.length += curlen;
        if (s.length <= 0) {
            break;
        }
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitWhileUTF16(twString s, twSplitByPredicate pred, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF16(&s, &cur)) > 0 && pred(cur)) {
        result.length += curlen;
        if (s.length <= 0) {
            break;
        }
    }

    if (remainder) *remainder = s;
    return result;
}

twString twSplitAnyASCII(twString s, const char *restrict cs, twString *remainder) {
    twString result = { .bytes = s.bytes, .length = 0 };

    char cur;
    while (twNextASCII(&s, &cur) > 0) {
        if (strchr(cs, cur)) {
            break;
        }
        result.length++;
    }

    if (remainder) *remainder = s;
    return result;
}

// TODO: Implement twSplitAnyUTF8()
twString twSplitAnyUTF8(twString s, const char *restrict cs, twString *remainder) {
    (void)s;
    (void)cs;
    (void)remainder;
    
    printf("%s is not implemeneted!\n", __func__);
    exit(-1);

#if 0
    twString result = { .bytes = s.bytes, .length = 0 };

    twChar cur;
    int curlen;
    while ((curlen = twNextUTF8(&s, &cur)) > 0) {

    }
#endif
}

// TODO: Implement twSplitAnyUTF16()
twString twSplitAnyUTF16(twString s, const char *restrict cs, twString *remainder) {
    (void)s;
    (void)cs;
    (void)remainder;
    
    printf("%s is not implemeneted!\n", __func__);
    printf("%s is not implemeneted!\n", __func__);
    exit(-1);
}

twString twHeadASCII(twString s) {
    if (twIsEmpty(s)) {
        return TWDEFAULT(twString);
    }
    return TWLIT(twString){ .bytes = s.bytes, .length = 1 };
}

twString twHeadUTF8(twString s) {
    int c_len = twEncodedCodepointLengthUTF8(s.bytes[0]);
    if (c_len == 0) {
        return TWDEFAULT(twString);
    }
    return TWLIT(twString){ .bytes = s.bytes, .length = c_len };
}

twString twHeadUTF16(twString s) {
    int c_len = twEncodedCodepointLengthUTF16(s.bytes[0]);
    if (c_len == 0) {
        return TWDEFAULT(twString);
    }
    return TWLIT(twString){ .bytes = s.bytes, .length = c_len };
}

twString twTailASCII(twString s) {
    return twDrop(s, 1);
}

twString twTailUTF8(twString s) {
    int c_len = twEncodedCodepointLengthUTF8(s.bytes[0]);
    if (c_len == 0) {
        return TWDEFAULT(twString);
    }
    return twDrop(s, c_len);
}

twString twTailUTF16(twString s) {
    int c_len = twEncodedCodepointLengthUTF16(s.bytes[0]);
    if (c_len == 0) {
        return TWDEFAULT(twString);
    }
    return twDrop(s, c_len);
}

twChar twFirstASCII(twString s) {
    if (twIsEmpty(s)) {
        return 0;
    }

    return s.bytes[0];
}

twChar twFirstUTF8(twString s) {
    if (twIsEmpty(s)) {
        return 0;
    }

    twChar c;
    int nchars = twDecodeUTF8(s.bytes, &c, 1);
    if (nchars != 1) {
        return 0;
    }

    return c;
}

twChar twFirstUTF16(twString s) {
    if (twIsEmpty(s)) {
        return 0;
    }

    twChar c;
    int nchars = twDecodeUTF16(s.bytes, &c, 1);
    if (nchars != 1) {
        return 0;
    }

    return c;
}

twChar twLastASCII(twString s) {
    if (twIsEmpty(s)) {
        return 0;
    }

    return s.bytes[s.length - 1];
}

twChar twLastUTF8(twString s) {
    if (twIsEmpty(s)) {
        return 0;
    }

    for (int i = s.length - 1; i >= 0; i--) {
        int len = twEncodedCodepointLengthUTF8(s.bytes[i]);
        if (len != 0) {
            twChar c;
            int nchars = twDecodeUTF8(&s.bytes[i], &c, 1);
            if (nchars != 1) {
                return 0;
            }

            return c;
        }
    }

    return 0;
}

twChar twLastUTF16(twString s) {
    if (twIsEmpty(s)) {
        return 0;
    }

    for (int i = s.length - 1; i >= 0; i--) {
        int len = twEncodedCodepointLengthUTF16(s.bytes[i]);
        if (len != 0) {
            twChar c;
            int nchars = twDecodeUTF16(&s.bytes[i], &c, 1);
            if (nchars != 1) {
                return 0;
            }

            return c;
        }
    }

    return 0;
}

twString twDrop(twString s, size_t n) {
    size_t num_dropped = s.length < n ? s.length : n;
    return TWLIT(twString){
        .bytes = s.bytes + num_dropped,
        .length = s.length - num_dropped
    };
}

twString twTrunc(twString s, size_t n) {
    size_t new_length = s.length < n ? s.length : n;
    s.length = new_length;
    return s;
}

twString twTrimLeftASCII(twString s) {
    twString iter = s;

    size_t ndrop = 0;
    for (;;) {
        char c;
        if (!twNextASCII(&iter, &c)) {
            break;
        }

        if (!twIsSpace(c)) {
            break;
        }

        ndrop++;
    }

    return twDrop(s, ndrop);
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

twString twTrimRightASCII(twString s) {
    twString iter = s;

    size_t ndrop = 0;
    for (;;) {
        char c;
        if (!twNextRevASCII(&iter, &c)) {
            break;
        }

        if (!twIsSpace(c)) {
            break;
        }

        ndrop++;
    }

    return TWLIT(twString){
        .bytes = s.bytes,
        .length = s.length - ndrop
    };
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

    return TWLIT(twString){
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

    return TWLIT(twString){
        .bytes = s.bytes,
        .length = s.length - ndrop
    };   
}

twString twTrimASCII(twString s) {
    s = twTrimLeftASCII(s);
    s = twTrimRightASCII(s);
    return s;
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

twStringBuf twNewBuf(void) {
    return TWDEFAULT(twStringBuf);
}

twStringBuf twNewBufWithMaxCapacity(size_t max_capacity) {
    return TWLIT(twStringBuf){
        .max_capacity = max_capacity
    };
}

twStringBuf twNewBufWithCapacity(size_t capacity) {
    char *bytes = twAlloc(capacity);
    if (bytes == NULL) {
        return TWDEFAULT(twStringBuf);
    }

    return TWLIT(twStringBuf){
        .bytes = bytes,
        .capacity = capacity
    };
}

void twFreeBuf(twStringBuf buf) {
    if (buf.bytes == NULL) return;
    twDealloc(buf.bytes);
}

twString twBufToString(twStringBuf buf) {
    return (twString) {
        .bytes = buf.bytes,
        .length = buf.length
    };
}

bool twResizeBuf(twStringBuf *buf, size_t new_size) {
    if (buf->max_capacity != 0 && new_size > buf->max_capacity) {
        return false;
    }

    if (buf->capacity >= new_size) {
        return true;
    }

    char *new_bytes = twRealloc(buf->bytes, new_size);
    if (!new_bytes) {
        return false;
    }

    buf->bytes = new_bytes;
    buf->capacity = new_size;
    return true;
}

bool twExtendBuf(twStringBuf *buf, size_t extension) {
    return twResizeBuf(buf, buf->length + extension);
}

bool twPushASCII(twStringBuf *buf, char c) {
    if (!twExtendBuf(buf, 1)) {
        return false;
    }

    buf->bytes[buf->length++] = c;
    return true;
}

bool twPushUTF8(twStringBuf *buf, twChar c) {
    int c_len = twCodepointLengthUTF8(c);
    if (c_len == 0) {
        return false;
    }

    if (!twExtendBuf(buf, c_len)) {
        return false;
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

    if (!twExtendBuf(buf, c_len)) {
        return false;
    }

    int encode_result = twEncodeUTF16(buf->bytes + buf->length, buf->capacity - buf->length, c);
    if (encode_result == 0 || encode_result != c_len) {
        return false;
    }

    buf->length += c_len;
    return true;
}

bool twAppendASCII(twStringBuf *buf, twString s) {
    if (!twExtendBuf(buf, s.length)) {
        return false;
    }

    memcpy(buf->bytes + buf->length, s.bytes, s.length);
    buf->length += s.length;
    return true;
}

bool twAppendUTF8(twStringBuf *buf, twString s) {
    if (!twIsValidUTF8(s)) {
        return false;
    }
    return twAppendASCII(buf, s);
}

bool twAppendUTF16(twStringBuf *buf, twString s) {
    if (!twIsValidUTF16(s)) {
        return false;
    }
    return twAppendASCII(buf, s);
}

bool twAppendFmtASCII(twStringBuf *buf, const char *__restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char *chars;

    char temp[1024];
    ssize_t len = vsnprintf(temp, sizeof(temp), fmt, args);
    if (len >= (ssize_t)sizeof(temp)) {
        chars = twAlloc((len + 1) * sizeof(*chars)); // Plus 1 for null terminator
        len = vsnprintf(chars, len, fmt, args);
    } else {
        chars = temp;
    }

    va_end(args);

    twString to_add = {chars, len};
    bool result = twAppendASCII(buf, to_add);

    if (chars != temp) {
        twDealloc(chars);
    }

    return result;
}

bool twAppendFmtUTF8(twStringBuf *buf, const char * restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char *chars;

    char temp[1024];
    ssize_t len = vsnprintf(temp, sizeof(temp), fmt, args);
    if (len >= (ssize_t)sizeof(temp)) {
        chars = twAlloc((len + 1) * sizeof(*chars)); // Plus 1 for null terminator
        len = vsnprintf(chars, len, fmt, args);
    } else {
        chars = temp;
    }

    va_end(args);

    twString to_add = {chars, len};
    bool result = twAppendUTF8(buf, to_add);

    if (chars != temp) {
        twDealloc(chars);
    }

    return result;
}

bool twAppendFmtUTF16(twStringBuf *buf, const char * restrict fmt, ...) {
    (void)buf;
    (void)fmt;
    // va_list args;
    // va_start(args, fmt);

    // char temp[1024];
    // int len = vsnprintf(temp, sizeof(temp), fmt, args);

    // va_end(args);

    // twString to_add = {temp, len};
    // return twAppendUTF8(buf, to_add);
    return false;
}

bool twAppendLineASCII(twStringBuf *buf, twString s) {
    bool ok = twAppendASCII(buf, s);
    if (!ok) {
        return false;
    }

    return twPushASCII(buf, '\n');
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

bool twInsertASCII(twStringBuf *buf, size_t idx, char c) {
    if (idx >= buf->length) {
        return false;
    }

    if (!twExtendBuf(buf, 1)) {
        return false;
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i] = buf->bytes[i - 1];
    }

    buf->bytes[idx] = c;
    buf->length++;

    return true;
}

bool twInsertUTF8(twStringBuf *buf, size_t idx, twChar c) {
    if (idx >= buf->length) {
        return false;
    }

    int c_len = twCodepointLengthUTF8(c);
    if (c_len == 0) {
        return false;
    }

    if (!twExtendBuf(buf, c_len)) {
        return false;
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

    if (!twExtendBuf(buf, c_len)) {
        return false;
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

bool twInsertStrASCII(twStringBuf *buf, size_t idx, twString s) {
    if (idx >= buf->length) {
        return false;
    }

    if (!twExtendBuf(buf, s.length)) {
        return false;
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i + s.length - 1] = buf->bytes[i - 1];
    }

    memcpy(buf->bytes + idx, s.bytes, s.length);

    buf->length += s.length;

    return true;
}

bool twInsertStrUTF8(twStringBuf *buf, size_t idx, twString s) {
    if (idx >= buf->length) {
        return false;
    }

    if (!twIsValidUTF8(s)) {
        return false;
    }

    if (!twExtendBuf(buf, s.length)) {
        return false;
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

    if (!twExtendBuf(buf, s.length)) {
        return false;
    }

    for (size_t i = buf->length; i > idx; i--) {
        buf->bytes[i + s.length - 1] = buf->bytes[i - 1];
    }

    memcpy(buf->bytes + idx, s.bytes, s.length);

    buf->length += s.length;

    return true;   
}

bool __twConcatASCII(twStringBuf *buf, ...) {
    va_list strs;
    va_start(strs, buf);

    bool result = true;

    for (;;) {
        twString str = va_arg(strs, twString);
        if (str.bytes == (const char*)-1 && str.length == (size_t)-1) {
            break;
        }

        if (!twAppendASCII(buf, str)) {
            result = false;
            break;
        }
    }

    va_end(strs);
    return result;
}

bool __twConcatUTF8(twStringBuf *buf, ...) {
    va_list strs;
    va_start(strs, buf);

    bool result = true;

    for (;;) {
        twString str = va_arg(strs, twString);
        if (str.bytes == (const char*)-1 && str.length == (size_t)-1) {
            break;
        }

        if (!twAppendUTF8(buf, str)) {
            result = false;
            break;
        }
    }

    va_end(strs);
    return result;
}

bool __twConcatUTF16(twStringBuf *buf, ...) {
    va_list strs;
    va_start(strs, buf);

    bool result = true;

    for (;;) {
        twString str = va_arg(strs, twString);
        if (str.bytes == (const char*)-1 && str.length == (size_t)-1) {
            break;
        }

        if (!twAppendUTF16(buf, str)) {
            result = false;
            break;
        }
    }

    va_end(strs);
    return result;
}

void twClear(twStringBuf *buf) {
    buf->length = 0;
}

bool twHasMaxCapacity(twStringBuf buf) {
    return buf.max_capacity != 0;
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
    twChar c = twFirstUTF8(*iter);
    if (c == 0) {
        if (result) *result = 0;
        return 0;
    }

    int codepoint_length = twCodepointLengthUTF8(c);
    *iter = twDrop(*iter, codepoint_length);
    if (result) *result = c;
    return codepoint_length;
}

int twNextUTF16(twString *iter, twChar *result) {
    twChar c = twFirstUTF16(*iter);
    if (c == 0) {
        if (result) *result = 0;
        return 0;
    }

    int codepoint_length = twCodepointLengthUTF16(c);
    *iter = twDrop(*iter, codepoint_length);
    if (result) *result = c;
    return codepoint_length;
}

int twNextRevASCII(twString *iter, char *result) {
    if (iter->length == 0) {
        if (result) *result = 0;
        return 0;
    }

    iter->length--;
    if (result) *result = iter->bytes[iter->length];
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

#ifdef __cplusplus
}
#endif

#endif // _TWINE_H_IMPLEMENTATION

#endif // TWINE_IMPLEMENTATION

