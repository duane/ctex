#ifndef __INCLUDE_STATE_REGISTERFILE_H__
#define __INCLUDE_STATE_REGISTERFILE_H__

#include <State/Word.h>
#include <Util/SmallIntMap.h>

namespace tex {

template <uint32_t N>
class RegisterFile {
private:
  RegisterFile(const RegisterFile &);
  RegisterFile &operator=(const RegisterFile &);

private:
  SmallIntMap<word, N> file;

public:

  word &operator[](uint32_t code) {
    const word *res = file.get(code);
    if (!res) {
      file.set(code, (word){0});
      res = file.get(code);
    }
    // a dirty hack.
    word *mut = const_cast<word*>(res);
    return *mut;
  }

  RegisterFile(void) : file() {}

};

}

#endif  // __INCLUDE_STATE_REGISTERFILE_H__