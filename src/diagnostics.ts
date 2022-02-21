import { Tree, TreeCursor } from 'tree-sitter';

import { convertTokenName } from './utils';

interface Token {
  token: string;
  start_col: number;
  end_col: number;
  start_line: number;
  end_line: number;
}

export function diagnostics(tree: Tree): { tokens: Token[] } {
  const cursor = tree.rootNode.walk();
  const tokens: Token[] = [];
  const addToken = (cursor: TreeCursor): void => {
    if (cursor.currentNode.childCount === 0) {
      tokens.push({
        token: convertTokenName(cursor.nodeType),
        start_col: cursor.startPosition.column + 1,
        end_col: cursor.endPosition.column + 1,
        start_line: cursor.startPosition.row + 1,
        end_line: cursor.endPosition.row + 1,
      });
    }
  };

  while (true) {
    if (cursor.gotoFirstChild() || cursor.gotoNextSibling()) {
      addToken(cursor);
      continue;
    }
    cursor.gotoParent();
    let hadSibling = true;
    while (!cursor.gotoNextSibling()) {
      if (!cursor.gotoParent()) {
        hadSibling = false;
        break;
      }
    }
    if (hadSibling) {
      addToken(cursor);
    }
    if (cursor.currentNode === tree.rootNode) {
      break;
    }
  }
  return {
    tokens,
  };
}
