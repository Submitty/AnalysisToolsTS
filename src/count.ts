import type { Tree } from 'tree-sitter';

export enum Counter {
  token = 'token',
  node = 'node',
  call = 'call',
  func = 'func',
  depth = 'depth',
}

export function getCounter(type: string): Counter {
  switch (type) {
    case 'token':
      return Counter.token;
    case 'node':
      return Counter.node;
    case 'call':
      return Counter.call;
    case 'func':
      return Counter.func;
    case 'depth':
      return Counter.depth;
    default:
      throw new Error(`Unknown counter: ${type}`);
  }
}

export function countToken(tree: Tree, counter: Counter, token: string): number {
  const tokenLower = token.toLowerCase();
  let count = 0;

  const cursor = tree.rootNode.walk();
  let continueLoop = true;
  while (continueLoop) {
    if (cursor.gotoFirstChild()) {
      if (cursor.nodeType.toLowerCase() === tokenLower) {
        count++;
      }
      continue;
    }
    if (cursor.gotoNextSibling()) {
      if (cursor.nodeType === tokenLower) {
        count++;
      }
      continue;
    }
    cursor.gotoParent();
    while (!cursor.gotoNextSibling()) {
      if (!cursor.gotoParent()) {
        break;
      }
    }
    if (cursor.currentNode === tree.rootNode) {
      continueLoop = false;
    }
  }
  return count;
}
