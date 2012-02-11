/*
    This header provides types and functions for internal unicode represenations.
    Data is stored as UTF-32, which is probably horrendously wastful in terms of memory,
    but it's as complex as I'm willing to get right now. Delving into UTF-16, which is
    more or less the standard between Microsoft and Apple, discards all the advantages
    of a fixed data type. Upper regions of unicode are *usually* not touched, but sometimes
    they are.
*/

#ifndef __INCLUDE_UNICODE_H__
#define __INCLUDE_UNICODE_H__

#include <stdint.h>
#include <assert.h>
#include <string.h>

namespace tex {

/** A Unicode codepoint. */
typedef uint32_t unichar;

/** The character used to represent errors in encoding/decoding. */
const unichar kBadUnichar = 0xFFF4;  // replacement character, used to indicate errors in the bytestreams.

}  // namespace tex

#endif  // __INCLUDE_UNICODE_H__
