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

#include <stdlib.h>

#include <Type/TFM.h>
#include <Type/Font.h>
#include <Util/SmallVector.h>

using namespace tex;

char *TFM::create_fix_word_string(int32_t fix_word) {
  char left_str[5] = {'\0'};
  bool sign = false;
  if (fix_word < 0) {
    sign = true;
    fix_word = -fix_word;
  }
  
  uint32_t left = fix_word >> 20;
  uint32_t right = fix_word & ((1 << 20) - 1);
  snprintf(left_str, sizeof(left_str), "%d", left);
  
  // now for the calculation of the decimals.
  // <part to right of decimal> = right / 2^20
  const unsigned right_size = 20; // we never need more than 20 decimal places.
  char right_str[right_size + 1] = "00000000000000000000";
  for (unsigned r_idx = 0; r_idx < right_size; r_idx++) {
    right *= 10;
    unsigned digit = right / (1 << 20);
    right_str[r_idx] = digit + '0';
    right -= (digit * 1<<20);
  }
  
  // now allocate the string.
  unsigned string_size = 1 // sign
                       + 4 // left
                       + 1 // decimal
                       + right_size // right
                       + 1; // nul terminator
  char *str = (char*)malloc(string_size);
  if (sign)
    snprintf(str, string_size, "-%s.%s", left_str, right_str);
  else
    snprintf(str, string_size, "%s.%s", left_str, right_str);
  return str;
}

// This is one ugly piece of code.
// However, it is written in a straightforward manner.
void TFM::read_header(UniquePtr<BinaryInputStream> &stream, uint16_t lh, UniquePtr<TFM> &tfm) {
  if (!lh)
    return;
  
  if (stream->read_uint32(tfm->file_checksum))
    assert(false && "Unexpected EOF! Internal state error.");
  
  if (lh == 1)
    return;
  
  if (stream->read_uint32((uint32_t&)tfm->design_size))
    assert(false && "Unexpected EOF! Internal state error.");
  
  if (lh > 2) {
    stream->seek(stream->offset() + (lh - 2) * 4);
  }

}

void TFM::read_char_info(UniquePtr<BinaryInputStream> &stream, UniquePtr<TFM> &tfm) {
  unsigned chars = tfm->char_upper - tfm->char_lower + 1;
  for (unsigned c = 0; c < chars; c++) {
    // read a single char_info
    char_info_word info;
    if (stream->read_uint8(info.width_index))
      assert(false && "Unexpected EOF! Internal state error.");
    uint8_t height_depth;
    if (stream->read_uint8(height_depth))
      assert(false && "Unexpected EOF! Internal state error.");
    info.height_index = (height_depth >> 4);
    info.depth_index = height_depth & 0x0F;
    
    uint8_t italic_tag;
    if (stream->read_uint8(italic_tag))
      assert(false && "Unexpected EOF! Internal state error.");
    
    info.italic_index = italic_tag >> 2;
    info.tag = italic_tag & 0x3;
    
    if (stream->read_uint8(info.remainder))
      assert(false && "Unexpected EOF! Internal state error.");
    
    tfm->char_info[c] = info;
  }
}

static void read_fix_word_table(UniquePtr<BinaryInputStream> &stream, TFM::fix_word *table, uint16_t size) {
  for (uint16_t f = 0; f < size; f++) {
    uint32_t word;
    if (stream->read_uint32(word))
      assert(false && "Unexpected EOF! Internal state error.");
    table[f] = (TFM::fix_word)word;
  }
}


void TFM::init_from_file(const char *path, UniquePtr<TFM> &result) {
  UniquePtr<BinaryInputStream> stream;
  BinaryInputStream::init_from_file(path, stream);

    // TFM files are big endian
  stream->set_endian(ENDIAN_BIG);
  
  // The first 24 bytes are guaranteed.
  if (stream->size() < 24)
    throw new GenericDiag("TFM file not large enough.", DIAG_TFM_PARSE_ERR, BLAME_HERE);
  uint16_t lf, lh, bc, ec, nw, nh, nd, ni, nl, nk, ne, np;
  if (stream->read_uint16(lf))
    throw new GenericDiag("TFM file empty.", DIAG_TFM_PARSE_ERR, BLAME_HERE);
  
  if ((lf < 6))
    throw new GenericDiag("TFM file not large enough to hold header.", DIAG_TFM_PARSE_ERR, BLAME_HERE);

  if ((lf * 4) != stream->size())
    throw new GenericDiag("TFM file incorrect size.", DIAG_TFM_PARSE_ERR, BLAME_HERE);
  
  if    (stream->read_uint16(lh)
      || stream->read_uint16(bc)
      || stream->read_uint16(ec)
      || stream->read_uint16(nw)
      || stream->read_uint16(nh)
      || stream->read_uint16(nd)
      || stream->read_uint16(ni)
      || stream->read_uint16(nl)
      || stream->read_uint16(nk)
      || stream->read_uint16(ne)
      || stream->read_uint16(np)) {
        // The size of the binary stream is at least 6 words, or 24 bytes, so reading these
        // halfwords should not be an issue. If there is, someone lied somewhere about their
        // size.
        assert(false && "Expected to read header without EOF occurring; internal error.");
      }

  if ((bc > 0 && (bc - 1) > ec) || ec > 255)
    throw new GenericDiag("Bad character range in TFM file.", DIAG_TFM_PARSE_ERR, BLAME_HERE);
  
  if (ne > 256)
    throw new GenericDiag("Extensible character table larger than valid size.", DIAG_TFM_PARSE_ERR, BLAME_HERE);
  
  uint16_t check_size = 6 + lh + (ec - bc + 1) + nw + nh + nd + ni + nl + nk + ne + np;
  if (check_size != lf)
    throw new GenericDiag("Size of file does not equal expected size of file.", DIAG_TFM_PARSE_ERR, BLAME_HERE);
  
  // Ok, so now we're ready to actually commit to an object.
  UniquePtr<TFM> tfm;
  tfm.reset(new TFM());

  tfm->width_size = nw;
  tfm->height_size = nh;
  tfm->depth_size = nd;
  tfm->italic_size = ni;
  tfm->ligkern_size = nl;
  tfm->kern_size = nk;
  
  tfm->char_lower = bc;
  tfm->char_upper = ec;
  
  tfm->char_code_conv[0] = '\0'; // empty character encoding convention by default.
  tfm->font_id[0] = '\0';

  tfm->width_table = new fix_word[tfm->width_size];
  tfm->height_table = new fix_word[tfm->height_size];
  tfm->depth_table = new fix_word[tfm->depth_size];
  tfm->italic_table = new fix_word[tfm->italic_size];
  tfm->ligkern_table = new ligkern_step[tfm->ligkern_size];
  tfm->kern_table = new fix_word[tfm->kern_size];
  
  tfm->char_info = new char_info_word[tfm->char_upper - tfm->char_lower + 1];
  
  tfm->tfm_path = Path(path);

  // let's read the remainder of the TFM file.
  read_header(stream, lh, tfm);
  read_char_info(stream, tfm);
  read_fix_word_table(stream, tfm->width_table, tfm->width_size);
  read_fix_word_table(stream, tfm->height_table, tfm->height_size);
  read_fix_word_table(stream, tfm->depth_table, tfm->depth_size);
  read_fix_word_table(stream, tfm->italic_table, tfm->italic_size);

  // read ligkern
  for (unsigned i = 0; i < nl; i++) {
    uint32_t step;
    if (stream->read_uint32(step))
      assert(false && "Unexpected EOF! Internal state error.");
    tfm->ligkern_table[i] = ligkern_step::from_be_word(step);
  }

  read_fix_word_table(stream, tfm->kern_table, tfm->kern_size);

  // skip extensible; not implemented.
  stream->seek(stream->offset() + ne * 4);

  // now read parameters.
  if (np < 7)
    throw new GenericDiag("TFM file must have at least seven parameters.",
                          DIAG_TFM_PARSE_ERR, BLAME_HERE);

  if (stream->read_uint32((uint32_t&)tfm->f_slant)
      || stream->read_uint32((uint32_t&)tfm->f_space)
      || stream->read_uint32((uint32_t&)tfm->f_space_stretch)
      || stream->read_uint32((uint32_t&)tfm->f_space_shrink)
      || stream->read_uint32((uint32_t&)tfm->f_x_height)
      || stream->read_uint32((uint32_t&)tfm->f_quad)
      || stream->read_uint32((uint32_t&)tfm->f_extra_space))
    assert(false && "Internal error. input stream ended before promised.");
  // skip the rest of np bytes.
  assert(stream->offset() == stream->size() && "More to file than expected.");

  result.reset(tfm.take());
}

void TFM::populate_font(Font &font, int32_t at) const {
  sp z = sp_from_fixed(design_size);
  if (at != -1000) {
    if (at > 0)
      z = scaled(at);
    else
      z = z.xn_over_d(at, -1000);
  }
  for (unsigned c = char_lower; c <= char_upper; c++) {
    CharInfo info;
    info.width = sp_from_fixed(width(c)) * z;
    info.height = sp_from_fixed(height(c)) * z;
    info.depth = sp_from_fixed(depth(c)) * z;
    info.italic = sp_from_fixed(italic(c)) * z;
    font.set(c, info);
  }

  font.set_space(sp_from_fixed(f_space) * z);
  font.set_space_stretch(sp_from_fixed(f_space_stretch) * z);
  font.set_space_shrink(sp_from_fixed(f_space_shrink) * z);
  font.set_x_height(sp_from_fixed(f_x_height) * z);
  font.set_quad(sp_from_fixed(f_quad) * z);
  font.set_extra_space(sp_from_fixed(f_extra_space) * z);
  font.set_at(z);
}

void TFM::load_font(const char *path, Font &font, int32_t at) {
  UniquePtr<TFM> tfm;
  init_from_file(path, tfm);
  tfm->populate_font(font, at);
  font.set_tfm(tfm);
}

TFM::~TFM(void) {
  delete[] width_table;
  delete[] height_table;
  delete[] depth_table;
  delete[] italic_table;
  delete[] ligkern_table;
  delete[] kern_table;
  delete[] char_info;
}


struct character {
  uint32_t code;
  unsigned idx, extent;
};

std::list<set_op> *TFM::set_string(UString &string, sp at) const {
  std::list<set_op> *ops = new std::list<set_op>();

  SmallVector<character, 16> lig_stack;

  for (unsigned i = 0; i < string.get_length(); i++) {
    unsigned idx = string.get_length() - i - 1;
    character c;
    c.code = string[idx];
    c.idx = idx;
    c.extent = 1;
    lig_stack.push(c);
  }

  while (lig_stack.entries() >= 2) { // while there are characters to process.
    character lhs = lig_stack.pop();
    character rhs = lig_stack.pop();
    for (TFM::ligkern_iterator iter = lk_begin(lhs.code);
                               iter != lk_end();
                               iter++) {
      TFM::ligkern_step step = iter.step();
      if (rhs.code == step.next_char) {
        if (step.op >= 128) {
          if (step.remainder >= kern_size)
            throw new GenericDiag("kerning program referenced kerning value "
                                  "outside the bounds of the kerning table.",
                                  DIAG_TFM_PARSE_ERR, BLAME_HERE);
          // kerning step.
          ops->push_back(set_op::set(lhs.code));
          sp kern = sp_from_fixed(kerning(step.remainder)) * at;
          ops->push_back(set_op::adjust(kern, scaled(0)));
          lig_stack.push(rhs);
        } else {
          // ligature step.
          character ch;
          ch.code = step.remainder;
          unsigned c = step.op & 0x1;
          unsigned b = (step.op >> 1) & 0x1;
          unsigned a = step.op >> 2;
          if (a > (b + c))
            throw new GenericDiag("Invalid ligature op found.",
                                  DIAG_TFM_PARSE_ERR, BLAME_HERE);
          if (c)
            lig_stack.push(rhs);
          if (a - b) {
            ops->push_back(set_op::set(ch.code));
          } else {
            lig_stack.push(ch);
          }
          if (b) {
            if (a) {
              ops->push_back(set_op::set(lhs.code));
            } else {
              lig_stack.push(lhs);
            }
          }
        }
        break; // end the program.
      }
    }
  }

  if (lig_stack.entries()) {
    character last_ch = lig_stack.pop();
    ops->push_back(set_op::set(last_ch.code));
  }
  return ops;
}