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

#ifndef __INCLUDE_UNICODE_CODECS_ASCII_H__
#define __INCLUDE_UNICODE_CODECS_ASCII_H__

#include <Unicode/Codec.h>
#include <Unicode/Ustring.h>

namespace tex {

class ASCIICodec : public Codec {
public:
  ASCIICodec() {}
  ~ASCIICodec() {}
  
  EncodedCharacter encode(unichar uchar) const {
    EncodedCharacter ec;
    if (uchar > 0x7f) {
      ec.encoded[0] = (uint8_t)(kBadUnichar >> 8);
      ec.encoded[1] = (uint8_t)(kBadUnichar & 0xff);
      ec.length = 2;
      return ec;
    }
    ec.encoded[0] = (uint8_t) uchar;
    ec.length = 1;
    return ec;
  }
  
  DecodedCharacter decode(uint8_t *str, size_t offset, size_t length) const {
    assert(offset < length);
    DecodedCharacter dc;
    dc.length = 1;
    dc.uchar = DecodeASCII(str, offset);
    return dc;
  }
  
  std::string name() const {
    return std::string("ASCII");
  }
  
  // since Decoding is literally impossible to get wrong,
  // and because fast ascii cstring decoding is so useful,
  // here's some shortcuts.
  
  static unichar DecodeASCII(uint8_t *str, size_t offset) {
    uint8_t c = str[offset];
    return (c & 0x80) ? kBadUnichar : (unichar)c;
  }
};
}  // namespace tex

#endif  //  __INCLUDE_UNICODE_CODECS_ASCII_H__
