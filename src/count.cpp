#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include "parser.h"
#include "utils.h"
extern "C"
{
#include <tree_sitter/api.h>
}

int count_feature(Parser *parser, const Countable &countable, const std::string &feature, const std::vector<std::string> &files)
{
    int count = 0;
    bool is_parent_call = false;
    bool is_parent_function = false;
    for (std::string file : files)
    {
        TSTree *tree = parser->parse_file(file);
        TSNode root_node = ts_tree_root_node(tree);
        TSTreeCursor cursor = ts_tree_cursor_new(root_node);
        while (true)
        {
            if (ts_tree_cursor_goto_first_child(&cursor) || ts_tree_cursor_goto_next_sibling(&cursor))
            {
                TSNode cur = ts_tree_cursor_current_node(&cursor);
                const char *type = ts_node_type(cur);
                if (!strcmp("identifier", type) && (is_parent_call || is_parent_function))
                {
                    int start = ts_node_start_byte(cur);
                    int end = ts_node_end_byte(cur);
                    if (parser->get_identifier(start, end) == feature)
                    {
                        count += 1;
                    }
                    if (is_parent_call)
                    {
                        is_parent_call = false;
                    }
                    if (is_parent_function)
                    {
                        is_parent_function = false;
                    }
                }
                if (!strncmp(feature.c_str(), type, strlen(type) > feature.length() ? strlen(type) : feature.length()))
                {
                    count += 1;
                }
                if ((!strcmp("call", type) || !strcmp("call_expression", type)) && countable == CALL)
                {
                    is_parent_call = true;
                }
                if ((!strcmp("function_definition", type)) && countable == FUNCTION)
                {
                    is_parent_function = true;
                }
                continue;
            }
            bool had_sibling = true;
            ts_tree_cursor_goto_parent(&cursor);
            while (!ts_tree_cursor_goto_next_sibling(&cursor))
            {
                if (!ts_tree_cursor_goto_parent(&cursor))
                {
                    had_sibling = false;
                    break;
                }
            }
            TSNode cur = ts_tree_cursor_current_node(&cursor);
            const char *type = ts_node_type(cur);
            if (had_sibling && !strncmp(feature.c_str(), type, strlen(type) > feature.length() ? strlen(type) : feature.length()))
            {
                count += 1;
            }
            if (ts_node_is_null(ts_node_parent(cur)))
            {
                break;
            }
        }
    }
    return count;
}

int main(int argc, char *argv[])
{
    Language lang;
    Countable countable;
    std::string feature;
    std::vector<std::string> files;
    parse_args_counter(argc, argv, lang, countable, feature, files);
    Parser parser(lang);
    int count = count_feature(&parser, countable, feature, files);
    std::cout << count << std::endl;
    exit(EXIT_SUCCESS);
}
