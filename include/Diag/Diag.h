#ifndef __INCLUDE_DIAG_DIAG_H__
#define __INCLUDE_DIAG_DIAG_H__

#include <IO/BlameSource.h>

namespace tex {

/** Diagnostic errno constants. */
enum {
  DIAG_OK = 0,
  DIAG_FILE_OPEN_ERR,
  DIAG_FILE_WRITE_ERR,
  DIAG_PARSE_ERR,
};

/** Diag is the base Diagnostic class.
 * Diag is never intented to be initialized. Instead, use one of the children classes: GenericDiag for regular diagnostics, BlameSourceDiag for diagnostics which can be "blamed" on another text file. Usually used for parsing.
 */
class Diag {
public:
  /**
   * Returns a c-string describing the diagnostic which must be freed.
   */
  virtual char *string() const = 0;
  
  /**
   * Returns the error number for the diagnostic.
   */
  virtual int errno() const = 0;
  /**
   * Prints the diagnostic to standard out; May print a different string than the one returned by string().
   */
  virtual void print() = 0;
  
  /**
   * A convenience method which simple checks whether the error number is 0, aka DIAG_OK.
   */ 
  virtual operator bool() const {
    return errno() != DIAG_OK;
  }
  
  /**
   * Returns the source of the diagnostic in the source code; use with BLAME_HERE.
   */
  virtual BlameSource source() const = 0;
};


/** GenericDiag is for "normal" errors.
 * GenericDiag is a simple vector for a message, an error, and a BlameSource.
 */
class GenericDiag : public Diag {
private:
  const char *message;
  int err;
  BlameSource diag_source;
  char *str;
public:
  /**
   * The only public constructor for GenericDiag.
   * @param msg A constant c-string describing the error.
   * @param errno The error number of the diagnostic.
   * @param source A BlameSource referencing the original code in tex which generated the diagnostic.
   */
  GenericDiag(const char *msg, int errno, BlameSource source);
  
  /**
   *  @see Diag::string
   */
  char *string(void) const {
    assert(str && "Attempted to return NULL diag string.");
    return str;
  }
  
  /**
  *  @see Diag::errno
  */
  int errno(void) const {
    return err;
  }
  
  /**
   * @see Diag::source
   */
  BlameSource source(void) const {
    return diag_source;
  }
  
  /**
   * Prints the string returned by string().
   */ 
  void print(void);
};


/** BlameSourceDiag is for diagnostics which can be blamed on other text files. */
class BlameSourceDiag : public Diag {
private:
  const char *message;
  int err;
  char *str;
  BlameSource text_source, diag_source;
public:
  
  /** @see Diag::string() */
  char *string(void) const {
    return str;
  }
  
  /** @see Diag::errno() */
  int errno(void) const {
    return err;
  }
  
  /** @see Diag::source() */
  BlameSource source(void) const {
    return diag_source;
  }
  
  /**
   * Returns the BlameSource for the text file to blame for the diagnostic.
   */
  BlameSource input_source(void) const {
    return text_source;
  }
  
  /** @see Diag::print() */
  void print();
  
  /** The constructor for BlameSourceDiag
   * @param msg A constant C-string describing the diagnostic.
   * @param err The error code for the diagnostic.
   * @param diag_source The place in the source where the diagnostic was generated.
   * @param input_source The place in the input text file that caused the diagnostic to be generated.
   */
  BlameSourceDiag(const char *msg, int err, BlameSource diag_source, BlameSource input_source);
};

}  // namespace tex

#endif  // __INCLUDE_DIAG_DIAG_H__