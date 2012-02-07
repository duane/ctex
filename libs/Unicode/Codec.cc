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
  size_t allocated = str->getAllocated();
  size_t length = str->getLength();
  size_t destIndex = 0;
  unichar *data = str->getRaw();
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
