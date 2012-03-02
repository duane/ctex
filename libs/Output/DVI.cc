#include <Output/DVI.h>

using namespace tex;

void DVI::init_with_file(const char *path, UniquePtr<DVI> &result) {
  UniquePtr<DVI> dvi(new DVI());
  DVIWriter::init_with_file(path, dvi->writer);
  result.reset(dvi.take());
}

static void write_header(UniquePtr<DVIWriter> &writer) {
  writer->pre(2, 25400000u, 473628672u, 1000, " TeX output 2010.11.12:1134");
  int32_t c[10] = {1};
  writer->bop(c);
}

static void write_post(UniquePtr<DVIWriter> &writer) {
  writer->eop();
  writer->post(25400000u, 473628672u, 1000, 43725786, 30785863);
  writer->fnt_def(0, 0, 655360, 655360, "cmr10");
  writer->post_post(2);
}

void DVI::render(UniquePtr<State> &state) {
  write_header(writer);
  writer->fnt_def(0, 0, 655360, 655360, "cmr10");
  writer->fnt(0);
  RenderNode *hlist = state->render().head();
  while (hlist) {
    switch(hlist->type) {
      case CHAR_NODE: {
        char_node node = hlist->ch;
        writer->set_char(node.ch);
        break;
      }
      case GLUE_NODE: {
        glue_node glue = hlist->glue;
        writer->right(glue.width.i64);
        break;
      }
      default: {
        throw new GenericDiag("Unable to render render node type.",
                              DIAG_RENDER_ERR, BLAME_HERE);
      }
    }
    hlist = hlist->link;
  }
  write_post(writer);
}