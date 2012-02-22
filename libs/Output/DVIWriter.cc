#include <Output/DVIWriter.h>

using namespace tex;

Diag *DVIWriter::init_with_file(const char *path, UniquePtr<DVIWriter> &result) {
  DVIWriter *writer = new DVIWriter();
  Diag *diag;
  if ((diag = BinaryOutputStream::init_with_file(path, writer->output))) {
    delete writer;
    return diag;
  }
  writer->output->set_endian(ENDIAN_BIG);
  result.reset(writer);
  return NULL;
}

void DVIWriter::set_char(uint32_t c) {
  if (c < 128)
    output->write_uint8(c);
  else {
    if (c < 256) {
      output->write_uint8(128);
      output->write_uint16(c);
    } else if (c < (1 << 16)) {
      output->write_uint8(129);
      output->write_uint16(c);
    } else if (c < (1 << 24)) {
      output->write_uint8(130);
      output->write_uint24(c);
    } else {
      output->write_uint8(131);
      output->write_uint32(c);
    }
  }
}

void DVIWriter::set_rule(int32_t a, int32_t b) {
  
}

void DVIWriter::put_char(uint32_t c) {

}

void DVIWriter::put_rule(int32_t a, int32_t b) {

}

void DVIWriter::nop() {

}

void DVIWriter::bop(int32_t *c) {
  uint32_t offset = (uint32_t)output->offset();
  output->write_uint8(139);
  for (unsigned i = 0; i < 10; i++) {
    output->write_uint32(c[i]);
  }
  output->write_uint32(prev_page);
  prev_page = offset;
  pages += 1;
}

void DVIWriter::eop() {
  output->write_uint8(140);
}

void DVIWriter::push() {
  output->write_uint8(141);
  curr_depth += 1;
  if (curr_depth > stack_depth)
    stack_depth = curr_depth;
}

void DVIWriter::pop() {
  output->write_uint8(142);
  curr_depth -= 1;
}

void DVIWriter::right(int32_t b) {
  if (b < 128 && b >= -128) {
    output->write_uint8(143);
    output->write_uint8((int8_t)b);
  } else if (b < (1 << 15) && (b >= -(1 << 15))) {
    output->write_uint8(144);
    output->write_uint16((int16_t)b);
  } else if (b < (1 << 23) && (b >= -(1 << 23))) {
    output->write_uint8(145);
    output->write_uint24(b);
  } else {
    output->write_uint8(146);
    output->write_uint32(b);
  }
}

void DVIWriter::w0() {

}

void DVIWriter::w(int32_t b) {

}

void DVIWriter::x0() {

}

void DVIWriter::x(int32_t b) {

}

void DVIWriter::down(int32_t a) {
  if (a < 128 && a >= -128) {
    output->write_uint8(157);
    output->write_uint8((int8_t)a);
  } else if (a < (1 << 15) && (a >= -(1 << 15))) {
    output->write_uint8(158);
    output->write_uint16((int16_t)a);
  } else if (a < (1 << 23) && (a >= -(1 << 23))) {
    output->write_uint8(159);
    output->write_uint24(a);
  } else {
    output->write_uint8(160);
    output->write_uint32(a);
  }
}

void DVIWriter::y0() {

}

void DVIWriter::y(int32_t a) {

}

void DVIWriter::z0() {

}

void DVIWriter::z(int32_t a) {

}

void DVIWriter::fnt(uint32_t f) {
  if (f < 64)
    output->write_uint8(f + 171);
  else if (f < 256) {
    output->write_uint8(235);
    output->write_uint8(f);
  } else if (f < (1 << 16)) {
    output->write_uint8(236);
    output->write_uint16(f);
  } else if (f < (1 << 24)) {
    output->write_uint8(237);
    output->write_uint24(f);
  } else {
    output->write_uint8(238);
    output->write_uint32(f);
  }
}

void DVIWriter::xxx(uint8_t *bytes, uint32_t len) {

}

void DVIWriter::fnt_def(uint32_t f, uint32_t checksum, uint32_t scale, uint32_t design, const char *path) {
  if (f < 256) {
    output->write_uint8(243);
    output->write_uint8(f);
  } else if (f < (1<<16)) {
    output->write_uint8(244);
    output->write_uint16(f);
  } else if (f < (1<<24)) {
    output->write_uint8(245);
    output->write_uint24(f);
  } else {
    output->write_uint8(246);
    output->write_uint32(f);
  }
  output->write_uint32(checksum);
  output->write_uint32(scale);
  output->write_uint32(design);
  output->write_uint8(0);
  uint8_t path_len = strnlen(path, 256);
  output->write_uint8(path_len);
  output->write_bytes((uint8_t*)path, path_len);
}

void DVIWriter::pre(uint8_t id, uint32_t num, uint32_t den, uint32_t mag, const char *comment) {
  output->write_uint8(247);
  output->write_uint8(id);
  output->write_uint32(num);
  output->write_uint32(den);
  output->write_uint32(mag);
  uint8_t comment_len = strnlen(comment, 256);
  output->write_uint8(comment_len);
  output->write_bytes((uint8_t*)comment, comment_len);
}

void DVIWriter::post(uint32_t num, uint32_t den, uint32_t mag, uint32_t l, uint32_t u) {
  post_offset = (uint32_t)output->offset();
  output->write_uint8(248);
  output->write_uint32(prev_page);
  output->write_uint32(num);
  output->write_uint32(den);
  output->write_uint32(mag);
  output->write_uint32(l);
  output->write_uint32(u);
  output->write_uint16(stack_depth);
  output->write_uint16(pages);
}

void DVIWriter::post_post(uint8_t id) {
  output->write_uint8(249);
  output->write_uint32(post_offset);
  output->write_uint8(id);
  output->write_uint8(223);
  output->write_uint8(223);
  output->write_uint8(223);
  output->write_uint8(223);
  while (output->offset() % 4) {
    output->write_uint8(223);
  }
}
