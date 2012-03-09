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

#include <cassert>

#include <Diag/Diag.h>


using namespace tex;

GenericDiag::GenericDiag(const char *msg, int errno, BlameSource source) {
  message = msg;
  err = errno;
  diag_source = source;
  
  assert(message && "Attempted to create a GenericDiag with NULL message.");
  
  std::string string = std::string(diag_source.string());
  string += std::string(": ");
  string += std::string(message, 2048);
  size_t str_len = string.length() + 1;
  str = new char[str_len];
  memcpy(str, string.c_str(), str_len);
}

void GenericDiag::print(void) {
  puts(str);
}


BlameSourceDiag::BlameSourceDiag(const char *msg, int error, BlameSource generationSource, BlameSource errorSource) {
  assert(msg && "Got passed null diagnostic message.");

  message = msg;
  err = error;
  diag_source = generationSource;
  text_source = errorSource;
  
  std::string string = std::string("Diagnostic generated from ");
  string += std::string(diag_source.string());
  string += std::string(":\n");
  string += std::string(text_source.string());
  string += std::string(": ");
  string += std::string(msg, 2048);
  
  size_t str_len = string.length() + 1;
  str = new char[str_len];
  memcpy(str, string.c_str(), str_len);
}

void BlameSourceDiag::print() {
  puts(str);
}