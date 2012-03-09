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

#include <Unicode/Codecs/ASCII.h>
#include <Unicode/Codec.h>

using namespace tex;

Codec::Codec() {
  
}
  
UString *Codec::DecodeBuffer(EncodedBuffer buf) {
  uint8_t *str = buf.buffer;
  size_t length = buf.length;
  MutableUString *out = new MutableUString(length);
  size_t offset = 0;
  for (size_t i = 0; i < length; i++) {
    DecodedCharacter dc = decode(str, offset, length);
    offset += dc.length;
    out->add(dc.uchar);
  }
  return out;
}

EncodedBuffer Codec::GetEncodedBuffer(UString *str) {
  EncodedBuffer eb;
  size_t allocated = str->get_allocated();
  size_t length = str->get_length();
  size_t destIndex = 0;
  unichar *data = str->get_raw();
  uint8_t *dest = new uint8_t[allocated];
  for (size_t index = 0; index < length; index++) {
    EncodedCharacter ec = encode(data[index]);
    if (destIndex + ec.length >= allocated) { // if our buffer has run out of room
      uint8_t *temp = new uint8_t[allocated * 2];
      memcpy(temp, dest, destIndex);
      delete dest;
      dest = temp;
      allocated = allocated * 2;
    }
    memcpy(dest + destIndex, ec.encoded, ec.length);
    destIndex += ec.length;
  }
  eb.length = destIndex;
  eb.buffer = dest;
  return eb;
}
