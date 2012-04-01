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

#include <Config.h>
#include <State/State.h>
#include <Type/TFM.h>
#include <Render/Units.h>

using namespace tex;

State::State(void) {
  // first initialize ccode to all "CC_OTHER_CHAR" as the default code.
  for (unsigned i = 0; i < 128; i++) {
    ccode[i] = CC_OTHER_CHAR;
  }

    // now initialize letters.
  for (unsigned i = 0; i < 26; i++) {
    ccode['A' + i] = CC_LETTER;
    ccode['a' + i] = CC_LETTER;
  }
  
  ccode[0x00] = CC_IGNORE;
  ccode[0x20] = CC_SPACER; // ' ' 
  ccode[0x5C] = CC_ESCAPE; // '\\'
  ccode[0x25] = CC_COMMENT; // '%'
  ccode[0x7F] = CC_INVALID;
  ccode[0x0A] = CC_CAR_RET; // '\n'; technically deviates from tex.
  ccode[0x0D] = CC_CAR_RET; // '\r'
  ccode[0x7B] = CC_LBRACE; // '{'
  ccode[0x7D] = CC_RBRACE; // '}'

  // load the null font.
  Font null_font;
  fonts.append(null_font);

  // and load computer modern.
  the_eqtb[FONT_CODE].u64 = load_font("cmr10", "rm", -1000);
  load_font("cmti10", "it", -1000);
  load_font("cmbx10", "bf", -1000);
  load_font("cmtt10", "tt", -1000);

  // Internal variables
  the_eqtb[LEFT_SKIP_CODE].scaled = scaled(0);
  the_eqtb[RIGHT_SKIP_CODE].scaled = scaled(0);
  the_eqtb[HSIZE_CODE].scaled = scaled_from(0x67FFF, UNIT_IN); // 6.5in
  the_eqtb[VSIZE_CODE].scaled = scaled_from(9 << 16, UNIT_IN); // 9in
  the_eqtb[PARINDENT_CODE].scaled = scaled(18 << 16);
  the_eqtb[BASELINE_SKIP_CODE].scaled = scaled(12 << 16);
  the_eqtb[SPLIT_TOP_SKIP_CODE].scaled = scaled(12 << 16);
  the_eqtb[SPLIT_MAX_DEPTH_CODE].scaled = scaled(4 << 16);
  the_eqtb[ADJ_DEMERITS_CODE].i64 = 10000;
  the_eqtb[PRETOLERANCE_CODE].i64 = 100;
  the_eqtb[LINE_PENALTY_CODE].i64 = 10;

  // initialize box registers.
  for (unsigned i = 0; i < 256; i++) {
    the_eqtb[BOX_BEGIN_CODE + i].ptr = NULL;
  }

  // enter vmode.
  r_state.set_mode(VMODE);

  // initialize primitives.
  primitive("par", CC_PAR_END, (word){0});
  primitive("end", CC_STOP, (word){0});
  primitive("eject", EJECT_CODE, (word){0});
  primitive("noindent", PAR_BEG_CODE, (word){0});
  primitive("indent", PAR_BEG_CODE, (word){1});
  primitive("break", PENALTY_CODE, (word){-10000});
  glue_node *glue = new glue_node;
  *glue = (glue_node){scaled(0), scaled(1), scaled(0), GLUE_FIL, GLUE_NORMAL};
  word glue_word;
  glue_word.ptr = (void*)glue;
  primitive("hfil", SKIP_CODE, glue_word);
  primitive("hrule", RULE_CODE, (word){0});
}

void State::init(UniquePtr<State> &result) {
    result.reset(new State());
}

uint32_t State::load_font(const char *file, const char *name, int32_t at) {
  uint32_t f = fonts.entries();
  Font empty_font;
  fonts.append(empty_font);
  Font &font = fonts[f];
  Path path;
  path.set_area(FONT_AREA);
  path.set_file(file);
  path.set_ext("tfm");
  TFM::load_font(path.full_path().c_str(), font, at);
  primitive(name, CC_SET_FONT, (word){f});
  return f;
}
