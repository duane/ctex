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

#ifndef __INCLUDE_OUTPUT_DVI_H__
#define __INCLUDE_OUTPUT_DVI_H__

#include <State/State.h>
#include <Output/DVIWriter.h>
#include <Util/SmallIntMap.h>

namespace tex {

class DVI {
private:
  DVI(const DVI&);
  DVI &operator=(const DVI&);

  DVI(void) : cur_font(0xFFFFFFFF), max_font(0), mode(VERT) {}

private:

  enum dvi_mode {
    HORIZ,
    VERT
  };

  UniquePtr<DVIWriter> writer;

  SmallIntMap<uint32_t, 64> tex_dvi_font_map;

  void write_header(UniquePtr<State> &state);
  void write_post(UniquePtr<State> &state);
  void write_char(UniquePtr<State> &state, RenderNode *node);
  void write_hbox(UniquePtr<State> &state, RenderNode *node);
  void write_vbox(UniquePtr<State> &state, RenderNode *node);
  void write_glue(UniquePtr<State> &state, RenderNode *node);
  void write_node(UniquePtr<State> &state, RenderNode *node);

  uint32_t define_font(UniquePtr<State> &state, uint32_t tex_font);
  void switch_font(UniquePtr<State> &state, uint32_t tex_font);

  uint32_t cur_font, max_font;
  dvi_mode mode;

public:
  static void init_with_file(const char *path, UniquePtr<DVI> &result);

  void render(UniquePtr<State> &state);
};

}

#endif  // __INCLUDE_OUTPUT_DVI_H__