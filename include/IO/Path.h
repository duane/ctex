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

#ifndef __INCLUDE_IO_PATH_H__
#define __INCLUDE_IO_PATH_H__

#include <string>

namespace tex {

class Path {
private:
  std::string path_area, path_file, path_ext;

public:
  Path() : path_area(), path_file(), path_ext() {}

  Path(const char *path) {
    std::string str = path;
    // march from right to left.
    signed ext_sep = -1;
    signed dir_sep = -1;
    for (unsigned i = 0; i < str.length(); i++){
      unsigned idx = str.length() - i - 1;
      char c = str.at(idx);
      if (c == '.' && ext_sep == -1)
        ext_sep = idx;
      else if (c == '/' && dir_sep == -1) {
        dir_sep = idx;
        break;
      }
    }
    // if the period is the first character in the name, then it's not an
    // extension.
    if (ext_sep == dir_sep + 1)
      ext_sep = -1;
    unsigned file_beg;
    unsigned file_end;
    if (ext_sep == -1)
      file_end = str.length() - 1;
    else {
      path_ext = std::string(str, ext_sep + 1, str.length() - ext_sep - 1);
      file_end = ext_sep - 1;
    }
    if (dir_sep == -1)
      file_beg = 0;
    else {
      path_area = std::string(str, 0, dir_sep + 1);
      file_beg = dir_sep + 1;
    }
    path_file = std::string(str, file_beg, (file_end - file_beg) + 1);
  }

  std::string area(void) const {
    return path_area;
  }

  void set_area(std::string area) {
    path_area = area;
  }

  std::string file(void) const {
    return path_file;
  }

  void set_file(std::string file) {
    path_file = file;
  }

  std::string ext(void) const {
    return path_ext;
  }

  void set_ext(std::string ext) {
    path_ext = ext;
  }

  std::string full_path(void) const {
    std::string result = path_area + path_file;
    if (path_ext.length())
      result += "." + path_ext;
    return result;
  }
};

}

#endif //  __INCLUDE_IO_PATH_H__