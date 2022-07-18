#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <fstream>
#include <iterator>
#include "parser.h"
#include "utils.h"

struct NodeDetails
{
    std::string end_col;
    std::string node;
    std::string start_line;
    std::string start_col;
    std::string end_line;
};

std::string escape_sequence(std::string type) {
    std::string escape_chars[] = {"\"", "\'"};
    if (type.find("\'") != std::string::npos || type.find("\"") != std::string::npos) {
        std::string filtered = "";
        for (size_t i = 0; i < type.length(); i++)
        {
            std::string target = type.substr(i, i + 1);
            if (std::find(std::begin(escape_chars), std::end(escape_chars), target) != std::end(escape_chars)) {
                filtered += "\\";
            }
            filtered += type[i];
        }
        return filtered;
    }
    return type;
}

void write_node(std::string file, TSNode cur, std::map<std::string, std::vector<NodeDetails>>& nodes)
{
    NodeDetails details;
    TSPoint start_point = ts_node_start_point(cur);
    TSPoint end_point = ts_node_end_point(cur);
    details.start_col = std::to_string(start_point.column + 1);
    details.start_line = std::to_string(start_point.row + 1);
    details.end_col = std::to_string(end_point.column + 1);
    details.end_line = std::to_string(end_point.row + 1);
    details.node = escape_sequence(ts_node_type(cur));
    if (nodes.count(file) > 0)
    {
        NodeDetails last = nodes[file].back();
        if (last.start_col == details.start_col && last.start_line == details.start_line && last.end_col == details.end_col && last.end_line == details.end_line) {
            nodes[file].pop_back();
        }
        nodes[file].push_back(details);
    }
    else
    {
        std::vector<NodeDetails> nodes_init(1, details);
        nodes[file] = nodes_init;
    }
}

void write_to_json_file(const std::map<std::string, std::vector<NodeDetails>>& file_nodes_map) {
    std::ofstream output_file("out.json");
    output_file << "{"
                << "\n";
    for (const std::pair<std::string, std::vector<NodeDetails>> &file_nodes : file_nodes_map)
    {
        output_file << "\t\"" + file_nodes.first + "\": [ \n";
        std::vector<NodeDetails> nodes = file_nodes.second;
        for (NodeDetails& node : nodes)
        {
            std::string obj = "\t\t{\n"
                              "\t\t\t\"start_col\": " + node.start_col + ",\n"
                              "\t\t\t\"start_line\": " + node.start_line + ",\n"
                              "\t\t\t\"end_col\": " + node.end_col + ",\n"
                              "\t\t\t\"end_line\": " + node.end_line + ",\n"
                              "\t\t\t\"node\": \"" + node.node + "\"\n"
                                  "\t\t}";
            if (&node == &nodes.back())
            {
                output_file << obj << "\n";
            }
            else
            {
                output_file << obj << ",\n";
            }
        }
        output_file << "\t]"
                    << "\n";
    }
    output_file << "}"
                << "\n";
}

void diagnose(Parser *parser, const std::vector<std::string> &files)
{
    std::map<std::string, std::vector<NodeDetails>> file_nodes_map;
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
                write_node(file, cur, file_nodes_map);
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
            if (had_sibling) {
                write_node(file, cur, file_nodes_map);
            }
            if (ts_node_is_null(ts_node_parent(cur)))
            {
                break;
            }
        }
    }
    write_to_json_file(file_nodes_map);
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
