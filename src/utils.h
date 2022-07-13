#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "parser.h"

enum Countable
{
    TOKEN,
    NODE,
    CALL,
    FUNCTION
};

void find_files(const std::string &file_pattern, std::vector<std::string> &files);

Language get_language(const std::string &arg);

Countable get_countable(const std::string &arg);

void parse_args_counter(int argc, char *argv[], Language &lang, Countable &countable, std::string &feature, std::vector<std::string> &files);

void parse_args_diagnoser(int argc, char *argv[], Language &lang, std::vector<std::string> &files);

#endif
