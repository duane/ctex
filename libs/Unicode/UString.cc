#include <cstring>

#include <Unicode/UString.h>

using namespace tex;

UString::UString(const char *cstr) {
  assert(cstr && "Attempted to initialize UString with NULL C-string.");
  allocated = length = strlen(cstr);
  raw = new unichar[allocated];
  for (size_t i = 0; i < length; i++) {
    raw[i] = (unichar)cstr[i];
  }
  dirty_hash = true;
}
