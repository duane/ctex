/*****************************************************************************
*  Copyright (c) 2012 Duane Ryan Bailey                                      *
*                                                                            *
*  Licensed under the Apache License, Version 2.0 (the "License");           *
*  you may not use this file except in compliance with the License.          *
*  You may obtain a copy of the License at                                   *
*                                                                            *
*      http://www.apache.org/licenses/LICENSE-2.0                            *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*****************************************************************************/

#ifndef __INCLUDE_UNICODE_CODECS_UTF8_H__
#define __INCLUDE_UNICODE_CODECS_UTF8_H__

#include <Unicode/Codec.h>

namespace tex {
static uint8_t utf_8_bl[256] = {
/*    0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f */  
/* 0 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 1 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 2 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 4 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 6 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* a */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* b */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* c */ 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/* d */ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/* e */ 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
/* f */ 4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
  
class UTF8Codec : public Codec {
public:
  UTF8Codec() {}
  ~UTF8Codec() {}
  
  EncodedCharacter encode(unichar uchar) const {
    EncodedCharacter ec;
    if (uchar < 0x80) {
      ec.encoded[0] = (uint8_t)uchar;
      ec.length = 1;
    } else if (uchar < 0x800) {
      ec.encoded[0] = (uint8_t)((uchar >> 6) & 0x1f) | 0xc0;
      ec.encoded[1] = (uint8_t)(uchar & 0x3f) | 0x80;
      ec.length = 2;
    } else if (uchar < 0x10000) {
      ec.encoded[0] = (uint8_t)((uchar >> 12) & 0x0f) | 0xe0;
      ec.encoded[1] = (uint8_t)((uchar >> 6) & 0x3f) | 0x80;
      ec.encoded[2] = (uint8_t)(uchar & 0x3f) | 0x80;
      ec.length = 3;
    } else if (uchar < 0x110000) {
      ec.encoded[0] = (uint8_t)((uchar >> 18) & 0x07) | 0xf0;
      ec.encoded[1] = (uint8_t)((uchar >> 12) & 0x3f) | 0x80;
      ec.encoded[2] = (uint8_t)((uchar >> 6) & 0x3f) | 0x80;
      ec.encoded[3] = (uint8_t)(uchar & 0x3f) | 0x80;
      ec.length = 4;
    } else {
      return encode(kBadUnichar);  // awkward.
    }
    return ec;
  }
  
  // does not handle any special cases.
  DecodedCharacter decode(uint8_t *str, size_t offset, size_t length) const {
    DecodedCharacter dc;
    uint8_t *source = (uint8_t*)&str[offset];
    uint8_t c = source[0];
    dc.length = utf_8_bl[c];
    
    if (offset + dc.length > length) {
      dc.uchar = kBadUnichar;
      dc.length = 2;
      return dc;
    }
    
    if (c < 0x80) { // single-byte, ascii char
      source += 1;
      dc.uchar = (unichar)c;
      return dc;
    } else { // the beginning of the multi-byte mark or an invalid code point
      switch (dc.length) {
        default: 
        case 1: // should theoretically never be reached;
        assert(false && "Unreachable code point! Something went very wrong.");
        case 0:  // an invalid code point
          dc.uchar = kBadUnichar;
          dc.length = 2;
          return dc;
        case 2:
          dc.uchar = ((source[0] & 0x1f) << 6) | (source[1] & 0x3f);
          return dc;
        case 3:
          dc.uchar = ((source[0] & 0x0f) << 12) | ((source[1] & 0x3f) << 6) | (source[2] & 0x3f);
          return dc;
        case 4:
          dc.uchar = ((source[0] & 0x07) << 18) | ((source[1] & 0x3f) << 12) | ((source[2] & 0x3f) << 6) | (source[3] & 0x3f);
          return dc;
      }
    }
  }
  
  // Also expose a nifty cstring -> UString initializer.
  UString *DecodeCString(const char *cstr) {
    EncodedBuffer eb;
    eb.length = strlen(cstr);
    eb.buffer = (uint8_t*)cstr;
    return DecodeBuffer(eb);
  }
  
  std::string name() const {
    return std::string("UTF8");
  }
};
}  // namespace tex

#endif  // __INCLUDE_UNICODE_CODECS_UTF8_H__
