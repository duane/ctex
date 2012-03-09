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

#ifndef __INCLUDE_UNICODE_CODEC_H__
#define __INCLUDE_UNICODE_CODEC_H__

#include <string>

#include <Unicode/Unicode.h>

namespace tex {

class UString;

/** A simple structure to store encoded characters. */
struct EncodedCharacter {
  // TODO: Hardwired for UTF-8's 6-byte theoretical maximum. Change to a statically-allocated vector for unusual codecs.
  /** Six bytes dedicated to the encoded character. */
  uint8_t encoded[6];
  /** The number of bytes used; cannot exceed six. */
  uint8_t length;
};

/** A struct storing the decoded character and the number of bytes of the encoded character. */
struct DecodedCharacter {
  /** The Unicode code point. */
  unichar uchar;
  /** The number of bytes used to decode the character. */
  uint8_t length; 
};

/** A simple buffer used for encoding and decoding multiple characters at a time. */
struct EncodedBuffer {
  /** The buffer used to store the characters. */
  uint8_t *buffer;
  /** The size of the buffer in bytes, not characters. */
  size_t length;
};


/**
 *  The base Codec class, used to convert between Unicode codepoints and various character encodings.
 *  
 *  The children classes derived from Codec should not have any state but the vtable implimenting the encoding, decoding, and name identification.
 */
class Codec {
public:
  /** This initializer does nothing, because there is no state. */
  Codec();
  
  /**
   *  encode() translates the given Unicode codepoint into a series of encoded bytes.  
   *  @param u The codepoint to encode.
   *  @return The returned EncodedCharacter is always a valid encoded character; problems with translation are indicated by returning the encoded 0xFFF4.
   */
  virtual EncodedCharacter encode(unichar u) const = 0;
  
  /**
   *  decode() translates bytes in memory to a unicode codepoint.
   *  @param str The buffer in memory to be decoded.
   *  @param offset The index into the buffer; validity depends on each codec, but it cannot equal or exceed length.
   *  @param length The size of the buffer.
   *  @return The decoded character is always a valid unicode point. Errors are indicated by returning 0xFFF4.
   */
  virtual DecodedCharacter decode(uint8_t *str, size_t offset, size_t length) const = 0;
  
  /** Returns a string identifying the codec; e.g. "UTF-8", "ASCII", etc. */
  virtual std::string name() const = 0;
  
  /**
   *  Decodes an entire buffer into a UString.
   *  @param buf The EncodedBuffer to be decoded into a string of codepoints.
   *  @return If an error is encountered, NULL is returned. Otherwise, a UString is allocated and returned with the decoded information.
   */
  UString *DecodeBuffer(EncodedBuffer buf);
  
  /**
   *  Encodes a string of Unicode codepoints into a buffer of encoded characters.
   *  @param str The string to be encoded.
   *  @return On an error, an EncodedBuffer is returned with a NULL buffer and a length of 0.
   */
  EncodedBuffer GetEncodedBuffer(UString *str);
};
}  // namespace tex

#endif  // __INCLUDE_UNICODE_CODEC_H__
