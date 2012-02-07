/*
  Copyright 2010 Duane R. Bailey
  
  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  
    http://www.apache.org/licenses/LICENSE-2.0
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#ifndef __INCLUDE_UNICODE_CODEC_H__
#define __INCLUDE_UNICODE_CODEC_H__

#include <string>

#include <Unicode/Unicode.h>

namespace tex {

class UString;


struct EncodedCharacter {
  // TODO: Hardwired for UTF-8's 6-byte theoretical maximum. Change to a statically-allocated vector for unusual codecs.
  uint8_t encoded[6];
  uint8_t length;
};

struct DecodedCharacter {
  unichar uchar;
  uint8_t length; 
};

struct EncodedBuffer {
  uint8_t *buffer;
  size_t length; // in bytes
};

class Codec {
public:
  Codec();
  
  virtual EncodedCharacter encode(unichar u) const = 0;
  virtual DecodedCharacter decode(uint8_t *str, size_t offset, size_t length) const = 0;
  virtual std::string name() const = 0;
  
  UString *DecodeBuffer(EncodedBuffer buf);
  EncodedBuffer GetEncodedBuffer(UString *str);
};
}  // namespace tex

#endif  // __INCLUDE_UNICODE_CODEC_H__
