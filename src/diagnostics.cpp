#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <iterator>
#include "parser.h"
#include "utils.h"
#include <nlohmann/json.hpp>

void write_node(const std::string& file, const TSNode& cur, nlohmann::json& file_nodes_obj)
{
    nlohmann::json node = nlohmann::json::object({});
    TSPoint start_point = ts_node_start_point(cur);
    TSPoint end_point = ts_node_end_point(cur);
    node["start_col"] = start_point.column + 1;
    node["start_line"] = start_point.row + 1;
    node["end_col"] = end_point.column + 1;
    node["end_line"] = end_point.row + 1;
    node["node"] = ts_node_type(cur);
    if (file_nodes_obj.contains(file)) {
        nlohmann::json last = file_nodes_obj[file].back();
        nlohmann::json diff = nlohmann::json::diff(node, last);
        if (diff.size() == 1) {
            file_nodes_obj[file].erase(file_nodes_obj[file].size() - 1);
        }
        file_nodes_obj[file].push_back(node);
    }
    else {
        file_nodes_obj[file] = nlohmann::json::array();
        file_nodes_obj[file].push_back(node);
    }
}

void diagnose(Parser *parser, const std::vector<std::string> &files)
{
    nlohmann::json file_nodes_obj = nlohmann::json::object({});
    for (std::string file : files)
    {
        TSTree *tree = parser->parse_file(file);
        TSNode root_node = ts_tree_root_node(tree);
        TSTreeCursor cursor = ts_tree_cursor_new(root_node);
        while (true)
        {
            TSNode cur = ts_tree_cursor_current_node(&cursor);
            write_node(file, cur, file_nodes_obj);
            if (ts_tree_cursor_goto_first_child(&cursor) || ts_tree_cursor_goto_next_sibling(&cursor))
            {
                continue;
            }
            ts_tree_cursor_goto_parent(&cursor);
            while (!ts_tree_cursor_goto_next_sibling(&cursor))
            {
                if (!ts_tree_cursor_goto_parent(&cursor))
                {
                    break;
                }
            }
            cur = ts_tree_cursor_current_node(&cursor);
            if (ts_node_is_null(ts_node_parent(cur)))
            {
                break;
            }
        }
    }
    std::ofstream output_file("out.json");
    output_file << std::setw(4) << file_nodes_obj << std::endl;
}


int main(int argc, char *argv[])
{
    Language lang;
    std::vector<std::string> files;
    parse_args_diagnoser(argc, argv, lang, files);
    Parser parser(lang);
    diagnose(&parser, files);
    exit(EXIT_SUCCESS);
}
