#include <iostream>
#include "parser.h"
#include <vector>
extern "C" {
    #include <tree_sitter/api.h>
}

using namespace std;

#ifndef COUNTER_H
#define COUNTER_H

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
        int count = 0;
};

#endif