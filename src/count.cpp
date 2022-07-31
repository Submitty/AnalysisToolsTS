#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "parser.h"
#include "utils.h"
extern "C" {
#include <tree_sitter/api.h>
}

// update a boolean if only the cur node is a function call node
void check_call_node(const char *type, bool &is_parent_call) {
	// types of function nodes: call, call_expression, binary_expression (eg:
	// std::cout << "hello")
	if (!strcmp("call", type) || !strcmp("call_expression", type) || !strcmp("binary_expression", type)) {
		is_parent_call = true;
	}
}

// check if the cur node is a function definition node
void check_func_def_node(const char *type, bool &is_parent_func_def) {
	if (!strcmp("function_definition", type)) {
		is_parent_func_def = true;
	}
}

// name of the function called is met in the node which is met before meeting
// the argument list node
void check_call_identifier_end(const std::string identifier, const std::string &feature, const char *type, int &count,
							   bool &is_parent_call) {
	if ((!strcmp("argument_list", type) || !strcmp("<<", type)) && is_parent_call) {
		is_parent_call = false;
		if (identifier == feature) {
			count += 1;
		}
	}
}

// increment count if type of the node matches the counting node type
bool is_node_type_match(const std::string &feature, const char *type) {
	if (!strncmp(feature.c_str(), type, std::max(strlen(type), feature.length()))) {
		return true;
	}
	return false;
}

int count_feature(Parser *parser, const Countable &countable, const std::string &feature,
				  const std::vector<std::string> &files) {
	int count = 0;
	bool is_parent_func_def = false;
	bool is_parent_call = false;
	for (std::string file : files) {
		TSTree *tree = parser->parse_file(file);
		TSNode root_node = ts_tree_root_node(tree);
		std::string identifier;
		TSTreeCursor cursor = ts_tree_cursor_new(root_node);
		while (true) {
			TSNode cur = ts_tree_cursor_current_node(&cursor);
			const char *type = ts_node_type(cur);
			// check if cur node is a identifier; used when counting
			// identifiers, functions or calls
			if (!strcmp("identifier", type) || !strcmp("field_identifier", type)) {
				int start = ts_node_start_byte(cur);
				int end = ts_node_end_byte(cur);
				// retrieve identifier name
				identifier = parser->get_identifier(start, end);
				if (identifier == feature) {
					if (is_parent_func_def && countable == FUNCTION) {
						count += 1;
					} else if (countable == IDENTIFIER) {
						count += 1;
					}
				}
				is_parent_func_def = false;
			}
			// node checks
			if (countable == NODE && is_node_type_match(feature, type)) {
				count += 1;
			} else if (countable == FUNCTION) {
				check_func_def_node(type, is_parent_func_def);
			} else if (countable == CALL) {
				check_call_node(type, is_parent_call);
				check_call_identifier_end(identifier, feature, type, count, is_parent_call);
			}
			if (ts_tree_cursor_goto_first_child(&cursor) || ts_tree_cursor_goto_next_sibling(&cursor)) {
				continue;
			}
			ts_tree_cursor_goto_parent(&cursor);
			while (!ts_tree_cursor_goto_next_sibling(&cursor)) {
				if (!ts_tree_cursor_goto_parent(&cursor)) {
					break;
				}
			}
			cur = ts_tree_cursor_current_node(&cursor);
			if (ts_node_is_null(ts_node_parent(cur))) {
				break;
			}
		}
	}
	return count;
}

int main(int argc, char *argv[]) {
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
