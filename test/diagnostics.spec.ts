import { expect } from 'chai';
import { describe, test } from 'mocha';
import path from 'path';

import { diagnostics } from '../src/diagnostics';
import { parseFile } from '../src/parser';
import { Language } from '../src/types';

import { TEST_DATA } from './constants';

describe('diagnostics', () => {
  describe('python', () => {
    test('simple python program', () => {
      expect(
        diagnostics(
          parseFile(Language.python, path.join(TEST_DATA, 'python', 'simple.py')),
        ),
      ).to.deep.equal({
        tokens: [
          {
            token: 'While',
            start_col: 1,
            end_col: 6,
            start_line: 1,
            end_line: 1,
          },
          {
            token: 'True',
            start_col: 7,
            end_col: 11,
            start_line: 1,
            end_line: 1,
          },
          {
            token: 'Colon',
            start_col: 11,
            end_col: 12,
            start_line: 1,
            end_line: 1,
          },
          {
            token: 'IntegerLiteral',
            start_col: 3,
            end_col: 4,
            start_line: 2,
            end_line: 2,
          },
          {
            token: 'Plus',
            start_col: 5,
            end_col: 6,
            start_line: 2,
            end_line: 2,
          },
          {
            token: 'IntegerLiteral',
            start_col: 7,
            end_col: 8,
            start_line: 2,
            end_line: 2,
          },
        ],
      });
    });

    test('find all colon tokens', () => {
      const { tokens } = diagnostics(
        parseFile(Language.python, path.join(TEST_DATA, 'python', 'operators.py')),
      );
      expect(
        tokens.reduce((acc, token) => {
          acc += token.token === 'Colon' ? 1 : 0;
          return acc;
        }, 0),
      ).to.deep.equal(7);
    });
  });
});
