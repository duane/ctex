#ifndef __INCLUDE_TYPE_TFM_H__
#define __INCLUDE_TYPE_TFM_H__

#include <cassert>
#include <stdint.h>

#include <Diag/Diag.h>
#include <IO/BinaryInputStream.h>
#include <IO/Path.h>
#include <Render/sp.h>
#include <Util/UniquePtr.h>


namespace tex {

class Font;

/**
 *  TFM is a class that corresponds directly to a TFM file.
 *  It follows that the internal state mirrors the file.
 *  Width/height/depth/etc are stored as arrays, and the character
 *  information index into these arrays.
 *  @see http://www-users.math.umd.edu/~asnowden/comp-cont/tfm.html
 */
class TFM {
public:
  typedef int32_t fix_word;
  
  static char *create_fix_word_string(int32_t fix_word);
  
private:
  // Disallow copy/assign.
  // There's no real reason to: after creation, TFM is immutable.
  // But on the other hand, there's no real reason to copy/assign.
  // So I'll at least make it difficult to do so.
  TFM(const TFM &);
  TFM &operator=(const TFM &);
  
private:
  struct char_info_word {
    uint8_t width_index;
    uint8_t height_index;
    uint8_t depth_index;
    uint8_t italic_index;
    uint8_t tag;
    uint8_t remainder;
  };
  
  // sizes of respective tables
  uint16_t width_size, height_size, depth_size, italic_size;
  
  // The bounds of the character information in the TFM.
  uint16_t char_lower, char_upper;
  
  uint32_t file_checksum;
  
  // Encoding information.
  char char_code_conv[40];
  char font_id[20];
  
  fix_word design_size;

  uint8_t seven_bit_safe_flag;
  uint8_t font_face;
    
  // The actual information.
  char_info_word *char_info;
  fix_word *width_table;
  fix_word *height_table;
  fix_word *depth_table;
  fix_word *italic_table;

  fix_word f_slant, f_space, f_space_stretch, f_space_shrink;
  fix_word f_x_height, f_quad, f_extra_space;

  Path tfm_path;
private:
  // The default private constructor used in init_from_file
  TFM(void) : width_size(0), height_size(0), depth_size(0), italic_size(0),
              char_lower(1), char_upper(0), file_checksum(0), design_size(1<<20),
              seven_bit_safe_flag(0), font_face(0), char_info(NULL),
              width_table(NULL), height_table(NULL), depth_table(NULL), italic_table(NULL) {}
              
  static void read_header(UniquePtr<BinaryInputStream> &stream, uint16_t lh, UniquePtr<TFM> &tfm);
  static void read_char_info(UniquePtr<BinaryInputStream> &stream, UniquePtr<TFM> &tfm);
public:
  /** Returns the lower bound of the characters encoded in the TFM. */
  uint16_t lower_char(void) const {
    return char_lower;
  }
  
  /** Returns the upper bound of the characters encoded in the TFM. */
  uint16_t upper_char(void) const {
    return char_upper;
  }
  
  /** Returns the checksum of the read file. */
  uint32_t checksum(void) const {
    return file_checksum;
  }
  
  /** Retuns the design size of the TFM file. */
  fix_word design(void) const {
    return design_size;
  }
  
  /** Returns the character convention for the TFM file. */
  char *character_convention(void) const {
    return (char*)char_code_conv;
  }
  
  /** Returns the font identifier for the TFM file. */
  char *font_identifier(void) const {
    return (char*)font_id;
  }
  
  /**
   *  Returns the face information for the font.
   *  Face information is encoded as thus:
   *  1. Add 1 for an italic font, 0 for roman font.
   *  2. Add 0 for medium, 2 for bold, 4 for light.
   *  3. Add 0 for regular, 6 for condensed, or 12 for extended.
   */
  uint8_t face(void) const {
    return font_face;
  }
  
  /**
   *  Fetches the width of the character.
   *  @param c The character code to be looked up.
   *  @return The binary fraction describing the width.
   */
  fix_word width(uint16_t c) const {
    assert((c >= char_lower || c <= char_upper) && "Requested character outside the bounds of the TFM file.");
    char_info_word info = char_info[c - char_lower];
    return width_table[info.width_index];
  }
  
  /**
   *  Fetches the height of the character.
   *  @param c The character code to be looked up.
   *  @return The binary fraction describing the height.
   */
  fix_word height(uint16_t c) const {
    assert((c >= char_lower || c <= char_upper) && "Requested character outside the bounds of the TFM file.");
    char_info_word info = char_info[c - char_lower];
    return height_table[info.height_index];
  }
  
  /**
   *  Fetches the depth of the character.
   *  @param c The character code to be looked up.
   *  @return The binary fraction describing the depth.
   */
  fix_word depth(uint16_t c) const {
    assert((c >= char_lower || c <= char_upper) && "Requested character outside the bounds of the TFM file.");
    char_info_word info = char_info[c - char_lower];
    return depth_table[info.depth_index];
  }
  
  /**
   *  Fetches the italic correction of the character.
   *  @param c The character code to be looked up.
   *  @return The binary fraction describing the italic.
   */
  fix_word italic(uint16_t c) const {
    assert((c >= char_lower || c <= char_upper) && "Requested character outside the bounds of the TFM file.");
    char_info_word info = char_info[c - char_lower];
    return italic_table[info.italic_index];
  }
  
  fix_word space(void) const {
    return f_space;
  }

  fix_word space_stretch(void) const {
    return f_space_stretch;
  }

  fix_word space_shrink(void) const {
    return f_space_shrink;
  }

  fix_word x_height(void) const {
    return f_x_height;
  }

  fix_word quad(void) const {
    return f_quad;
  }

  fix_word extra_space(void) const {
    return f_extra_space;
  }

  Path path(void) const {
    return tfm_path;
  }

  /**
   *  Initializes a new TFM file from the contents of file at path.
   *  @param path Where to find the TFM file.
   *  @param result Where the resulting TFM is stored.
   *  @return NULL on success, non-NULL Diag* on failure.
   */
  static void init_from_file(const char *path, UniquePtr<TFM> &result);
  
  void populate_font(Font &font, int32_t at) const;

  static void load_font(const char *path, Font &font, int32_t at);

  static sp sp_from_fixed(fix_word f) {
    return scaled(f >> 4);
  }

  /** Deconstructor frees allocated memory. */
  ~TFM(void);
};

}

#endif  // __INCLUDE_TYPE_TFM_H__