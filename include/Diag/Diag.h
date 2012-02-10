#ifndef __INCLUDE_DIAG_DIAG_H__
#define __INCLUDE_DIAG_DIAG_H__

#include <IO/BlameSource.h>

namespace tex {

enum {
  DIAG_OK = 0,
  DIAG_FILE_OPEN_ERR,
  DIAG_FILE_WRITE_ERR,
  DIAG_PARSE_ERR,
};

class Diag {
public:
  virtual char *string() const = 0;
  virtual int errno() const = 0;
  virtual void print() = 0;
  
  virtual operator bool() const {
    return errno() != DIAG_OK;
  }
  
  virtual BlameSource source() const = 0;
};

class GenericDiag : public Diag {
private:
  const char *message;
  int err;
  BlameSource diag_source;
  char *str;
public:
  GenericDiag(const char *msg, int errno, BlameSource source);
  char *string(void) const {
    assert(str && "Attempted to return NULL diag string.");
    return str;
  }
  
  int errno(void) const {
    return err;
  }
  
  BlameSource source(void) const {
    return diag_source;
  }
  
  void print(void);
};

class BlameSourceDiag : public Diag {
private:
  const char *message;
  int err;
  char *str;
  BlameSource text_source, diag_source;
public:
  char *string(void) const {
    return str;
  }
  
  int errno(void) const {
    return err;
  }
  
  BlameSource source(void) const {
    return diag_source;
  }
  
  BlameSource input_source(void) const {
    return text_source;
  }
  
  void print();
  BlameSourceDiag(const char *msg, int err, BlameSource diag_source, BlameSource input_source);
};

class OKDiag : public Diag {
public:
  OKDiag(void) {}
  
  char *string(void) const {
    return (char*)"";
  }
  
  int errno(void) const {
    return DIAG_OK;
  }
  
  BlameSource source(void) const {
    return BlameSource();
  }
  
  void print(void);
};

}  // namespace tex

#endif  // __INCLUDE_DIAG_DIAG_H__