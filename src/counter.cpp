#include "counter.h"
#include <cstring>
extern "C" {
    #include <tree_sitter/api.h>
}

Counter::Counter(Parser* parser, const Countable& countable, const std::string& feature, const std::vector<std::string>& files)
  : parser(parser)
  , countable(countable)
  , feature(feature)
  , files(files)
{}

void Counter::count_feature() {
    for (size_t i = 0; i < files.size(); i++)
    {
      TSTree *tree = parser->parse_file(files[i]);
      TSNode root_node = ts_tree_root_node(tree);
      TSTreeCursor cursor = ts_tree_cursor_new(root_node);
      while (true)
      {
          if (ts_tree_cursor_goto_first_child(&cursor) || ts_tree_cursor_goto_next_sibling(&cursor)) {
            TSNode cur = ts_tree_cursor_current_node(&cursor);
            const char* type = ts_node_type(cur);
            if (!strcmp("identifier", type) && is_parent_call) {
              is_parent_call = false;
              int start = ts_node_start_byte(cur);
              int end = ts_node_end_byte(cur);
              if (parser->get_identifier(start, end) == feature) {
                count += 1;
              }
            }
            if (!strncmp(feature.c_str(), type, feature.length())) {
              count += 1;
            }
            if ((!strcmp("call", type) || !strcmp("call_expression", type)) && countable == call) {
              is_parent_call = true;
            }
            continue;
          }
          bool had_sibling = true;
          ts_tree_cursor_goto_parent(&cursor);
          while(!ts_tree_cursor_goto_next_sibling(&cursor)) {
            if (!ts_tree_cursor_goto_parent(&cursor)) {
              had_sibling = false;
              break;
            }
          }
          TSNode cur = ts_tree_cursor_current_node(&cursor);
          if(had_sibling && !strncmp(feature.c_str(), ts_node_type(cur), feature.length())) {
            count += 1;
          }
          if (ts_node_is_null(ts_node_parent(cur))) {
            break;
          }
      }
    }
    std::cout << count << std::endl;
}