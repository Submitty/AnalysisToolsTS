import type { Tree } from 'tree-sitter';

import { convertTokenName } from './utils';

export enum Feature {
  token = 'token',
  node = 'node',
  call = 'call',
}

export function getCountableFeature(type: string): Feature {
  switch (type) {
    case 'token':
      return Feature.token;
    case 'node':
      return Feature.node;
    case 'call':
      return Feature.call;
    default:
      throw new Error(`Unknown countable feature: ${type}`);
  }
}

export function countToken(tree: Tree, feature: Feature, token: string): number {
  const tokenLower = token.toLowerCase();
  let count = 0;

  const cursor = tree.rootNode.walk();
  while (true) {
    if (cursor.gotoFirstChild() || cursor.gotoNextSibling()) {
      if (
        feature === Feature.token &&
        convertTokenName(cursor.nodeType).toLowerCase() === tokenLower
      ) {
        count++;
      }
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
    if (
      feature === Feature.token &&
      hadSibling &&
      convertTokenName(cursor.nodeType).toLowerCase() === tokenLower
    ) {
      count++;
    }
    if (cursor.currentNode === tree.rootNode) {
      break;
    }
  }
  return count;
}
