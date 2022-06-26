
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <regex>
#include <vector>
#include "parser.h"
#include "counter.h"

using namespace std;
using namespace boost::filesystem;

void find_files(string file_pattern, vector<string>& files) {
  regex pattern (file_pattern);
  for (auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(boost::filesystem::current_path()))) {
      stringstream ss;
      ss << entry.path().filename().string();
      string filename = ss.str();
      if (regex_match(filename, pattern) && is_regular_file(entry.path())) {
          files.push_back(filename);
      }
  }
}

Language get_language(string arg) {
  if (arg == "python" || arg == "py") {
    return PYTHON;
  }
  else if (arg == "c") {
    return C;
  }
  cout << "Invalid language" << endl;
  exit (EXIT_FAILURE);
}

Countable get_countable(string arg) {
  if (arg == "token") {
    return token;
  }
  else if (arg == "node") {
    return node;
  }
  else if (arg == "call") {
    return call;
  }
  cout << "Invalid language" << endl;
  exit (EXIT_FAILURE);
}

void parse_args(int argc, char *argv[], Language& lang, Countable& countable, string& feature, vector<string>& files) {
  if (argc != 6) {
    cout << "Require more arguements" << endl;
    exit(EXIT_FAILURE);
  }
  for (size_t i = 1; i < argc; i++) {
    if (i == 1 && (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--language"))) {
      continue;
    }
    else if (i == 2) {
      lang = get_language(argv[i]);
      continue;
    }
    else if (i == 3) {
      countable = get_countable(argv[i]);
      continue;
    }
    else if (i == 4) {
      feature = argv[i];
      continue;
    }
    else if (i == 5) {
      find_files(argv[i], files);
      continue;
    }
    cout << "Invalid arguement at index " << i  << " " << argv[i] << endl;
    exit(EXIT_FAILURE);
  }
}
