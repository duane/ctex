#ifndef __INCLUDE_IO_BLAMESOURCE_H__
#define __INCLUDE_IO_BLAMESOURCE_H__

#include <cassert>

#include <string>

#define BLAME_HERE \
  BlameSource(__FILE__, __LINE__, __LINE__, 0, 0)

namespace tex {

class BlameSource {
private:
  const char *inputName;
  uint32_t line_start, line_end;
  uint32_t col_start, col_end;
  char *str;
public:
  BlameSource() : inputName(NULL), line_start(0), line_end(0), col_start(0), col_end(0), str(NULL) {}
  BlameSource(const char *input, uint32_t lineStart, uint32_t lineEnd, uint32_t colStart, uint32_t colEnd);
  char *string() const {
    assert(str && "Attempted to return NULL string.");
    return str;
  }
};

}

#endif  // __INCLUDE_IO_BLAMESOURCE_H__