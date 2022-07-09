#include <iostream>
#include <vector>
#include <string>
#include "parser.h"
#include "counter.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    Language lang;
    Countable countable;
    std::string feature;
    std::vector<std::string> files;
    parse_args(argc, argv, lang, countable, feature, files);
    Parser parser(lang);
    Counter counter(&parser, countable, feature, files);
    counter.count_feature();
    exit(EXIT_SUCCESS);
}
