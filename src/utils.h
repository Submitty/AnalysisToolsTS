#ifndef UTILS_H
#define UTILS_H

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <regex>
#include <vector>
#include "parser.h"
#include "counter.h"

using namespace std;
using namespace boost::filesystem;

void find_files(string file_pattern, vector<string>& files);

Language get_language(string arg);

Countable get_countable(string arg);

void parse_args(int argc, char *argv[], Language& lang, Countable& countable, string& feature, vector<string>& files);

#endif
