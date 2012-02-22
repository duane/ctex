#ifndef __INCLUDE_OUTPUT_DVI_DVIWRITER_H__
#define __INCLUDE_OUTPUT_DVI_DVIWRITER_H__

#include <IO/BinaryOutputStream.h>
#include <Util/UniquePtr.h>

namespace tex {

class DVIWriter {
private:
  // Disallow copy/assign.
  DVIWriter(const DVIWriter&);
  DVIWriter &operator=(const DVIWriter&);
  DVIWriter(void) : output(), prev_page(-1), pages(0), stack_depth(0), post_offset(0), curr_depth(0) {}

private:
  UniquePtr<BinaryOutputStream> output;

  uint32_t prev_page;
  uint16_t pages, stack_depth;
  uint32_t post_offset;
  
  uint16_t curr_depth;
  
public:
  static Diag *init_with_file(const char *path, UniquePtr<DVIWriter> &result);

  void set_char(uint32_t c);
  
  void set_rule(int32_t a, int32_t b);
  
  void put_char(uint32_t c);
  
  void put_rule(int32_t a, int32_t b);
  
  void nop();
  
  void bop(int32_t *c);
  
  void eop();
  
  void push();
  
  void pop();
  
  void right(int32_t b);
  
  void w0();
  
  void w(int32_t b);
  
  void x0();
  
  void x(int32_t b);

  void down(int32_t a);
  
  void y0();
  
  void y(int32_t a);
  
  void z0();
  
  void z(int32_t a);
  
  void fnt(uint32_t f);
  
  void xxx(uint8_t *bytes, uint32_t len);

  void fnt_def(uint32_t f, uint32_t checksum, uint32_t scale, uint32_t design, const char *path);
  
  void pre(uint8_t id, uint32_t num, uint32_t den, uint32_t mag, const char *comment);
  
  void post(uint32_t num, uint32_t den, uint32_t mag, uint32_t l, uint32_t u);
  
  void post_post(uint8_t id);
  
};

}

#endif  // __INCLUDE_OUTPUT_DVI_DVIWRITER_H__