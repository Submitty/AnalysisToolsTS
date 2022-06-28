#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>
#include "parser.h"
#include <vector>
extern "C" {
    #include <tree_sitter/api.h>
}

using namespace std;

enum Countable {
  token,
  node,
  call
};

class Counter {
    public:
        void count_feature();
        Counter(Parser* parser, Countable countable, string feature, vector<string>& files);

    private:
        Countable countable;
        string feature;
        vector<string> files;
        Parser *parser;
        bool is_parent_call;
        int count = 0;
};

#endif