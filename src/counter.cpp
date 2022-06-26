#include "counter.h"
#include <cstring>
extern "C" {
    #include <tree_sitter/api.h>
}

using namespace std;

Counter::Counter(Parser* parser, Countable _countable, string _feature, vector<string>& files)
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
          bool same_depth = false;
          if (ts_tree_cursor_goto_first_child(&cursor)) {
            same_depth = true;
          }
          else if (ts_tree_cursor_goto_next_sibling(&cursor)) {
            same_depth = true;
          }
          if (same_depth) {
            TSNode cur = ts_tree_cursor_current_node(&cursor);
            if (!strcmp("=", ts_node_type(cur))) {
              count += 1;
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
          if(had_sibling && !strcmp("=", ts_node_type(cur))) {
            count += 1;
          }
          if (ts_node_is_null(ts_node_parent(cur))) {
            break;
          }
      }
    }
    std::cout << count << "\n";
}