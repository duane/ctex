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

#include <cstdio>

#include <Config.h>
#include <Type/TFM.h>

using namespace tex;

static inline void print_face_info(uint8_t face) {
  if (face % 2) {
    printf("Italic ");
    face -= 1;
  } else
    printf("Roman ");
  if (face % 6 == 4) {
    printf("Light ");
    face -= 4;
  } else if (face % 6 == 2) {
    printf("Bold ");
    face -= 2;
  } else
    printf("Medium ");
  if (face > 12)
    printf("Extended");
  else if (face > 6)
    printf("Condensed");
  else
    printf("Regular");
}

static inline void print_fix_word(TFM::fix_word f) {
  char *str = TFM::create_fix_word_string(f);
  printf("%s", str);
  free(str);
}

static inline void print_tfm(UniquePtr<TFM> &tfm) {
  assert(tfm);
  printf("TFM:\n");
  printf("\tIdentifier: %s\n", tfm->font_identifier());
  printf("\tFile checksum: %8X\n", tfm->checksum());
  printf("\tDesign size: ");
  print_fix_word(tfm->design());
  /*
  printf("\n\tCharacter convention: %s\n", tfm->character_convention());
  printf("\n\tFont face: ");
  print_face_info(tfm->face());
  printf("\n\tSpace: ");
  print_fix_word(tfm->space());
  printf("\n\tStretch: ");
  print_fix_word(tfm->space_stretch());
  printf("\n\tShrink: ");
  print_fix_word(tfm->space_shrink());
  printf("\n\tx-height: ");
  print_fix_word(tfm->x_height());
  printf("\n\tem: ");
  print_fix_word(tfm->quad());
  printf("\n\tExtra space: ");
  print_fix_word(tfm->extra_space());*/
  printf("\n");
  for (unsigned c = tfm->lower_char(); c <= tfm->upper_char(); c++) {
    printf("\n\tCharacter code %02X", c);
    if (c >= ' ' && c <= '~')
      printf(" ('%c')", c);
    printf(":\n");
    printf("\t\tWidth: ");
    print_fix_word(tfm->width(c));
    printf("\n\t\tHeight: ");
    print_fix_word(tfm->height(c));
    printf("\n\t\tDepth: ");
    print_fix_word(tfm->depth(c));
    printf("\n\t\tItalic: ");
    print_fix_word(tfm->italic(c));
    printf("\n");
    for (TFM::ligkern_iterator iter = tfm->lk_begin(c);
                               iter != tfm->lk_end();
                               iter++) {
      TFM::ligkern_step step = iter.step();
      printf("\t\t\tStep %d: ", iter.step_num);
      if (step.op < 128)
        printf("ligature: %02X+%02X => %02X\n", c, step.next_char,
               step.remainder);
      else {
        printf("kerning: ");
        print_fix_word(tfm->kerning(step.remainder));
        printf(" between %02X and %02X.\n", c, step.next_char);
      }
    }
  }
  printf("\n");
}

int main(int argc, char **argv) {
  UniquePtr<TFM> tfm;
  try {
    Path path;
    path.set_area(FONT_AREA);
    path.set_file("cmr10");
    path.set_ext("tfm");
    TFM::init_from_file(path.full_path().c_str(), tfm);
    print_tfm(tfm);
  } catch (Diag *diag) {
    diag->print();
    return 1;
  }
  return 0;
}