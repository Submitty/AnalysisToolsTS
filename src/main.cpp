#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <regex>
#include <vector>
#include "parser.h"
#include "counter.h"
#include "utils.h"

using namespace std;

int main(int argc, char *argv[]) {
    Language lang;
    Countable countable;
    string feature;
    vector<string> files;
    parse_args(argc, argv, lang, countable, feature, files);
    Parser parser(lang);
    Counter counter(&parser, countable, feature, files);
    counter.count_feature();
    exit(EXIT_SUCCESS);
}
