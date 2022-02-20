import { expect } from 'chai';
import { describe, test } from 'mocha';

import { parseFile } from '../src/parser';

describe('parseFile', () => {
  test('exception is thrown on invalid language', () => {
    expect(() => parseFile('invalid', 'file')).to.throw(
      "Language 'invalid' is not supported",
    );
  });
});
