#ifndef COUNTER_H
#define COUNTER_H

#include "parser.h"
#include <vector>

enum Countable {
  token,
  node,
  call,
  function
};

class Counter {
    public:
        void count_feature();
        Counter(Parser* parser, const Countable& countable, const std::string& feature, const std::vector<std::string>& files);

    private:
        Countable countable;
        std::string feature;
        std::vector<std::string> files;
        Parser *parser;
        bool is_parent_call = false;
        bool is_parent_function = false;
        int count = 0;
};

#endif