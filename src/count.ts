import type { Tree } from 'tree-sitter';

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
      if (cursor.nodeType.toLowerCase() === tokenLower) {
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
      break;
    }
  }
  return count;
}
