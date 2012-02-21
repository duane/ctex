#include <cstdio>

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
  printf("\n\tCharacter convention: %s\n", tfm->character_convention());
  printf("\n\tFont face: ");
  print_face_info(tfm->face());
  printf("\n");
  for (unsigned c = tfm->lower_char(); c <= tfm->upper_char(); c++) {
    printf("\n\tCharacter code %2X", c);
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
  }
  printf("\n");
}

int main(int argc, char **argv) {
  UniquePtr<TFM> tfm;
  Diag *diag = TFM::init_from_file("cmr10.tfm", tfm);
  if (diag) {
    diag->print();
    return 1;
  }
  
  print_tfm(tfm);
  return 0;
}