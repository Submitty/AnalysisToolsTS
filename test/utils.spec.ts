import { expect } from 'chai';
import { describe, test } from 'mocha';

import { convertTokenName, upperFirstChar } from '../src/utils';

test('upperFirstChar', () => {
  expect(upperFirstChar('foo')).to.equal('Foo');
});

describe('convertTokenName', () => {
  [
    ['foo', 'Foo'],
    ['integer', 'IntegerLiteral'],
    [':', 'Colon'],
  ].forEach(([input, expected]) => {
    expect(convertTokenName(input)).to.equal(expected);
  });
});
