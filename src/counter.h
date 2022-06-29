#ifndef COUNTER_H
#define COUNTER_H

#include "parser.h"
#include <vector>

enum Countable {
  token,
  node,
  call
};

class Counter {
    public:
        void count_feature();
        Counter(Parser* parser, Countable countable, std::string feature, std::vector<std::string>& files);

    private:
        Countable countable;
        std::string feature;
        std::vector<std::string> files;
        Parser *parser;
        bool is_parent_call = false;
        int count = 0;
};

#endif