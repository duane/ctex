#include <Unicode/Codecs/ASCII.h>
#include <Unicode/UString.h>

using namespace tex;

UString *UString::FromCString(const char *cstr) {
  return ASCIICodec::DecodeCString(cstr);
}
