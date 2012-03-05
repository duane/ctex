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