#include <vector>
#include "parser.h"
#include "utils.h"
#include <glob.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>

void find_files(const std::string &file_pattern, std::vector<std::string> &files)
{
  glob_t glob_result = {0};
  int return_value = glob(file_pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
  if (return_value != 0)
  {
    std::cerr << "Error: Invalid file pattern" << std::endl;
    std::cerr << "Usage: Use the exact file name or a valid glob" << std::endl;
    exit(EXIT_FAILURE);
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
  else if (arg == "cpp" || arg == "c++")
  {
    return CPP;
  }
  std::cerr << "Error: Invalid language" << std::endl;
  std::cerr << "Usage: Use either py/python, c, cpp/c++" << std::endl;
  exit(EXIT_FAILURE);
}

Countable get_countable(const std::string &arg)
{
  if (arg == "node")
  {
    return NODE;
  }
  else if (arg == "call")
  {
    return CALL;
  }
  else if (arg == "function")
  {
    return FUNCTION;
  }
  else if (arg == "identifier")
  {
    return IDENTIFIER;
  }
  std::cerr << "Error: Invalid countable" << std::endl;
  std::cerr << "Usage: Pass either node, call, identifier or function" << std::endl;
  exit(EXIT_FAILURE);
}

void parse_args_counter(int argc, char *argv[], Language &lang, Countable &countable, std::string &feature, std::vector<std::string> &files)
{
  std::string usage_format = "Usage: submitty_count_ts -l <language> <countable> <feature> <files>";
  if (argc != 6)
  {
    std::cerr << "Error: require more arguments" << std::endl;
    std::cerr  << usage_format << std::endl;
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
    std::cerr << "Error: Invalid argument at index " << i << " " << argv[i] << std::endl;
    std::cerr << usage_format << std::endl;
    exit(EXIT_FAILURE);
  }
}

void parse_args_diagnoser(int argc, char *argv[], Language &lang, std::vector<std::string> &files)
{
  std::string usage_format = "Usage: submitty_diagnostics_ts -l <language> <files>";
  if (argc != 4)
  {
    std::cerr << "Error: require more arguments" << std::endl;
    std::cerr  << usage_format << std::endl;
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
      find_files(argv[i], files);
      continue;
    }
    std::cerr << "Error: Invalid argument at index " << i << " " << argv[i] << std::endl;
    std::cerr << usage_format << std::endl;
    exit(EXIT_FAILURE);
  }
}
