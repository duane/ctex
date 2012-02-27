#ifndef __INCLUDE_TYPE_FONT_H__
#define __INCLUDE_TYPE_FONT_H__

#include <Render/sp.h>
#include <Util/SmallIntMap.h>
#include <Unicode/Unicode.h>

namespace tex {

struct CharInfo {
  sp width;
  sp height;
  sp depth;
  sp italic;
};

class Font {
private:
  SmallIntMap<CharInfo, 256> char_map;

public:
  // Copy/assign allowed
  // Should only happen when creating the Font.
  Font(const Font &other) : char_map(other.map()) {}

  Font operator=(const Font &other) {
    return Font(other);
  }

  Font(void) : char_map() {}

  const SmallIntMap<CharInfo, 256> &map(void) const {
    return char_map;
  }

  void set(unichar uc, const CharInfo &info) {
    char_map.set(uc, info);
  }

  const CharInfo *get(unichar uc) const {
    return char_map.get(uc);
  }
};
//typedef SmallIntMap<CharInfo, 256> Font;

}  // namespace tex

#endif  // __INCLUDE_TYPE_FONT_H__