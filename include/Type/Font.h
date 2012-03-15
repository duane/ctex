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

#ifndef __INCLUDE_TYPE_FONT_H__
#define __INCLUDE_TYPE_FONT_H__

#include <Render/sp.h>
#include <Util/SmallIntMap.h>
#include <Unicode/Unicode.h>
#include <Type/TFM.h>

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
  sp f_space, f_space_stretch, f_space_shrink;
  sp f_x_height, f_quad, f_extra_space;
  sp at_size;
  UniquePtr<TFM> tfm;

public:
  // Copy/assign allowed
  // Should only happen when creating the Font.
  Font(const Font &other) : char_map(other.map()) {
    Font &other_mut = const_cast<Font &>(other);
    set_tfm(other_mut.get_tfm());
  }

  Font operator=(Font other) {
    return Font(other);
  }

  Font(void) {}

  const SmallIntMap<CharInfo, 256> &map(void) const {
    return char_map;
  }

  void set_at(sp at) {
    at_size = at;
  }

  void set(unichar uc, const CharInfo &info) {
    char_map.set(uc, info);
  }

  void set_space(sp space) {
    f_space = space;
  }

  sp space(void) const {
    return f_space;
  }

  void set_space_stretch(sp space_stretch) {
    f_space_stretch = space_stretch;
  }

  sp space_stretch(void) const {
    return f_space_stretch;
  }

  void set_space_shrink(sp space_shrink) {
    f_space_shrink = space_shrink;
  }

  sp space_shrink(void) const {
    return f_space_shrink;
  }

  void set_x_height(sp x_height) {
    f_x_height = x_height;
  }

  sp x_height(void) const {
    return f_x_height;
  }

  void set_quad(sp quad) {
    f_quad = quad;
  }

  sp quad(void) const {
    return f_quad;
  }

  void set_extra_space(sp extra_space) {
    f_extra_space = extra_space;
  }

  sp extra_space(void) const {
    return f_extra_space;
  }

  const CharInfo &get(unichar uc) const {
    const CharInfo *info = char_map.get(uc);
    assert(info && "Attempted to get char which does not exist in font f.");
    return *info;
  }

  const CharInfo *get_maybe(unichar uc) const {
    return char_map.get(uc);
  }

  void set_tfm(UniquePtr<TFM> &new_tfm) {
    tfm.reset(new_tfm.take());
  }

  UniquePtr<TFM> &get_tfm(void) {
    return tfm;
  }

  std::list<set_op> *set_string(UString &string) {
    return tfm->set_string(string, at_size);
  }
};
//typedef SmallIntMap<CharInfo, 256> Font;

}  // namespace tex

#endif  // __INCLUDE_TYPE_FONT_H__