#include <Output/DVIWriter.h>

using namespace tex;

int main(int argc, char **argv) {
  UniquePtr<DVIWriter> writer;
  try {
    DVIWriter::init_with_file("hello.dvi", writer);
  } catch (Diag *diag) {
    diag->print();
    return 1;
  }
  writer->pre(2, 25400000u, 473628672u, 1000, " TeX output 2010.11.12:1134");
  int32_t c[10] = {1};
  writer->bop(c);
  writer->push();
  writer->down(-917504);
  writer->pop();
  writer->down(42152922);
  writer->push();
  writer->down(-41497562);
  writer->push();
  writer->right(1310720);
  writer->fnt_def(0, 1274110073, 655360, 655360, "cmr10");
  writer->fnt(0);
  writer->set_char('h');
  writer->set_char('e');
  writer->set_char('l');
  writer->set_char('l');
  writer->set_char('o');
  writer->pop();
  writer->pop();
  writer->down(1572864);
  writer->push();
  writer->right(15229091);
  writer->set_char(1);
  writer->pop();
  writer->eop();
  writer->post(25400000u, 473628672u, 1000, 43725786, 30785863);
  writer->fnt_def(0, 1274110073u, 655360, 655360, "cmr10");
  writer->post_post(2);

  return 0;
}
