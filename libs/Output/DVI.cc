#include <Output/DVI.h>

#include <iostream>

using namespace tex;

static const bool FRAME_BOXES = false;
static const int64_t weight = 0x07fff;


void DVI::init_with_file(const char *path, UniquePtr<DVI> &result) {
  UniquePtr<DVI> dvi(new DVI());
  DVIWriter::init_with_file(path, dvi->writer);
  result.reset(dvi.take());
}

void DVI::write_header(UniquePtr<State> &state) {
  writer->pre(2, 25400000u, 473628672u, 1000, " TeX output 2010.11.12:1134");
  int32_t c[10] = {1};
  writer->bop(c);
}

void DVI::write_post(UniquePtr<State> &state) {
  writer->eop();
  writer->post(25400000u, 473628672u, 1000, 43725786, 30785863);
  for (SmallIntMap<uint32_t, 64>::iterator iter = tex_dvi_font_map.begin();
       iter != tex_dvi_font_map.end(); iter++) {
    uint32_t tex_font = iter.key();
    define_font(state, tex_font);
  }
  writer->post_post(2);
}

uint32_t DVI::define_font(UniquePtr<State> &state, uint32_t font) {
  const uint32_t *dvi = tex_dvi_font_map.get(font);
  uint32_t dvi_fnt;
  if (!dvi) {
    dvi_fnt = max_font;
    tex_dvi_font_map.set(font, dvi_fnt);
    max_font++;
  } else {
    dvi_fnt = *dvi;
  }
  UniquePtr<TFM> &tfm = state->metrics(font).get_tfm();
  sp design = TFM::sp_from_fixed(tfm->design());
  Path path = tfm->path();
  writer->fnt_def(dvi_fnt, 0, design.i64, design.i64, path.file().c_str());
  return dvi_fnt;
}

void DVI::switch_font(UniquePtr<State> &state, uint32_t font) {
  const uint32_t *dvi = tex_dvi_font_map.get(font);
  if (!dvi)
    writer->fnt(define_font(state, font));
  else if (*dvi != cur_font)
    writer->fnt(*dvi);
}

void DVI::write_char(UniquePtr<State> &state, RenderNode *node) {
  assert(node->type == CHAR_NODE && "Internal error; not char node.");
  switch_font(state, node->ch.font);
  writer->set_char(node->ch.ch);
}


void DVI::write_hbox(UniquePtr<State> &state, RenderNode *node) {
  assert(node->type == HBOX_NODE && "Internal error; not hbox.");
  assert(mode == VERT && "Attempted to write hbox in horizontal mode.");
  writer->down(node->height(state).i64);
  writer->push();
  mode = HORIZ;
  RenderNode *hlist = node->box.list;
  while (hlist) {
   write_node(state, hlist);
   hlist = hlist->link;
  }
  mode = VERT;
  writer->pop();
  if (FRAME_BOXES) {
    writer->push();
    int32_t width = node->width(state).i64;
    int32_t height = node->height(state).i64;
    std::cout << "width: " << scaled(width).string() << ", height: " << scaled(height).string() << std::endl;
    writer->put_rule(weight, width);
    writer->put_rule(height, weight);
    writer->down(-height);
    writer->put_rule(weight, width);
    writer->down(height);
    writer->right(width);
    writer->put_rule(height, weight);
    writer->pop();
  }
}

void DVI::write_vbox(UniquePtr<State> &state, RenderNode *node) {
  assert(node->type == VBOX_NODE && "Internal error; not vbox.");
  assert(mode == HORIZ && "Attempted to write vbox in vertical mode.");
  writer->push();
  mode = VERT;
  RenderNode *vlist = node->box.list;
  while (vlist) {
    write_node(state, vlist);
    vlist = vlist->link;
  }
  mode = HORIZ;
  writer->pop();
  if (FRAME_BOXES) {
    int32_t width = node->width(state).i64;
    int32_t height = node->height(state).i64;
    writer->put_rule(weight, width);
    writer->down(height);
    writer->put_rule(height, weight);
    writer->put_rule(weight, width);
    writer->right(width);
    writer->put_rule(height, weight);
    writer->down(-height);
  } else {
    writer->right(node->width(state).i64);
  }
}

void DVI::write_glue(UniquePtr<State> &state, RenderNode *node) {
  if (mode == VERT)
    writer->down(node->width(state).i64);
  else
    writer->right(node->width(state).i64);
}

void DVI::write_node(UniquePtr<State> &state, RenderNode *node) {
  switch (node->type){
    case CHAR_NODE: {
      write_char(state, node);
      break;
    }
    case HBOX_NODE: {
      write_hbox(state, node);
      break;
    }
    case VBOX_NODE: {
      write_vbox(state, node);
      break;
    }
    case GLUE_NODE: {
      write_glue(state, node);
      break;
    }
    default:
      throw new GenericDiag("Unable to render node type!",
                            DIAG_RENDER_ERR, BLAME_HERE);
  }
}

void DVI::render(UniquePtr<State> &state) {
  write_header(state);
  RenderNode *head = state->render().head();
  while (head) {
    write_node(state, head);
    head = head->link;
  }
  write_post(state);
}