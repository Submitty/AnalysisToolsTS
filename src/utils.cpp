#include <vector>
#include "parser.h"
#include "counter.h"
#include <glob.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

void find_files(const std::string &file_pattern, std::vector<std::string> &files)
{
  glob_t glob_result = {0};
  int return_value = ::glob(file_pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
  if (return_value != 0)
  {
    return;
  }
  files.insert(files.begin(), glob_result.gl_pathv, glob_result.gl_pathv + glob_result.gl_pathc);
  globfree(&glob_result);
}

Language get_language(const std::string &arg)
{
  if (arg == "python" || arg == "py")
  {
    return PYTHON;
  }
  else if (arg == "c")
  {
    return C;
  }
  std::cout << "Invalid language" << std::endl;
  exit(EXIT_FAILURE);
}

Countable get_countable(const std::string &arg)
{
  if (arg == "token")
  {
    return token;
  }
  else if (arg == "node")
  {
    return node;
  }
  else if (arg == "call")
  {
    return call;
  }
  else if (arg == "function")
  {
    return function;
  }
  std::cout << "Invalid countable" << std::endl;
  exit(EXIT_FAILURE);
}

void parse_args(int argc, char *argv[], Language &lang, Countable &countable, std::string &feature, std::vector<std::string> &files)
{
  if (argc != 6)
  {
    std::cout << "Require more arguments" << std::endl;
    exit(EXIT_FAILURE);
  }
  for (size_t i = 1; i < argc; i++)
  {
    if (i == 1 && (!strncmp(argv[i], "-l", 2) || !strncmp(argv[i], "--language", 10)))
    {
      continue;
    }
    else if (i == 2)
    {
      lang = get_language(argv[i]);
      continue;
    }
    else if (i == 3)
    {
      countable = get_countable(argv[i]);
      continue;
    }
    else if (i == 4)
    {
      feature = argv[i];
      continue;
    }
    else if (i == 5)
    {
      find_files(argv[i], files);
      continue;
    }
    std::cout << "Invalid arguement at index " << i << " " << argv[i] << std::endl;
    exit(EXIT_FAILURE);
  }
}
