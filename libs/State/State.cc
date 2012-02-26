#include <State/State.h>
#include <Type/TFM.h>

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

  // load the null font.
  Font f;
  fonts.append(f);

  // and load CMR10;
  curr_font = load_font("cmr10.tfm", sp(-1000));
}

void State::init(UniquePtr<State> &result) {
  result.reset(new State());
}

uint32_t State::load_font(const char *path, sp at) {
  UniquePtr<TFM> tfm;
  TFM::init_from_file(path, tfm);
  uint32_t f = fonts.entries();
  Font empty_font;
  fonts.append(empty_font);
  Font &font = fonts.get(f);
  tfm->populate_font(font, at);
  return f;
}
