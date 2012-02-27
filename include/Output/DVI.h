#ifndef __INCLUDE_OUTPUT_DVI_H__
#define __INCLUDE_OUTPUT_DVI_H__

#include <State/State.h>
#include <Output/DVIWriter.h>

namespace tex {

class DVI {
private:
  DVI(const DVI&);
  DVI &operator=(const DVI&);

  DVI(void) : writer() {}

private:
  UniquePtr<DVIWriter> writer;

public:
  static void init_with_file(const char *path, UniquePtr<DVI> &result);

  void render(UniquePtr<State> &state);
};

}

#endif  // __INCLUDE_OUTPUT_DVI_H__