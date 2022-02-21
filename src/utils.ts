const ALIASES: Record<string, string> = {
  ':': 'colon',

  '=': 'Equal',
  '<': 'LessThan',
  '>': 'GreaterThan',
  '+': 'Plus',
  '-': 'Minus',
  '+=': 'AddAssign',
  '-=': 'SubAssign',

  '==': 'EqOp',
  '<=': 'LeOp',
  '>=': 'GeOp',

  integer: 'IntegerLiteral',
};

export function convertTokenName(tokenName: string): string {
  return upperFirstChar(ALIASES[tokenName] || tokenName);
}

export function upperFirstChar(str: string): string {
  return str.charAt(0).toUpperCase() + str.slice(1);
}
