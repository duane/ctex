#include <IO/BlameSource.h>
#include <string>

using namespace tex;

BlameSource::BlameSource(const char *input, uint32_t lineStart, uint32_t lineEnd, uint32_t colStart, uint32_t colEnd)
{
  line_start = lineStart + 1;
  line_end = lineEnd + 1;
  col_start = colStart + 1;
  col_end = colEnd + 1;
  input_name = input;
  
  assert(line_start <= line_end && "Start line exceeds end line.");
  assert(col_start <= col_end && "Start col exceeds end col.");
  assert(input && "Got passed null input string.");
  
  std::string string = std::string(input_name); // copy up to 256 chars from input name to string.

  string += std::string(":");

  char lstart[11];
  snprintf(lstart, sizeof(lstart), "%u", line_start);
  string += std::string(lstart);
  if (line_end > line_start) {
    char lend[11];
    snprintf(lend, sizeof(lend), "%u", line_end);
    string += std::string("-");
    string += std::string(lend);
  }

  string += std::string(":");
  
  char cstart[11];
  snprintf(cstart, sizeof(cstart), "%u", col_start);
  string += std::string(cstart);
  if (col_end > col_start) {
    char cend[11];
    snprintf(cend, sizeof(cend), "%u", col_end);
    string += "-";
    string += std::string(cend);
  }
  
  str = new char[string.length() + 1];
  memcpy(str, string.c_str(), string.length() + 1);
}