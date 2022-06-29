#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "parser.h"
#include "counter.h"

void find_files(const std::string& file_pattern, std::vector<std::string>& files);

Language get_language(const std::string& arg);

Countable get_countable(const std::string& arg);

void parse_args(int argc, char *argv[], Language& lang, Countable& countable, std::string& feature, std::vector<std::string>& files);

#endif
