import { expect } from 'chai';
import { describe, test } from 'mocha';
import path from 'path';

import { Counter, countToken, getCounter } from '../src/count';
import { parseFile } from '../src/parser';

import { TEST_DATA } from './constants';

describe('getCounter', () => {
  test('throws error on unknown counter', () => {
    expect(() => getCounter('invalid')).to.throw('Unknown counter: invalid');
  });

  const cases: [string, Counter][] = [
    ['token', Counter.token],
    ['node', Counter.node],
    ['call', Counter.call],
    ['func', Counter.func],
    ['depth', Counter.depth],
  ];
  cases.forEach(([input, expected]) => {
    test(`returns ${expected} for ${input}`, () => {
      expect(getCounter(input)).to.equal(expected);
    });
  });
});

describe('countToken', () => {
  describe('token', () => {
    describe('python', () => {
      const testDir = path.join(TEST_DATA, 'python');
      const cases: [string, string, number][] = [
        ['loops.py', 'while', 2],
        ['loops.py', 'for', 3],
        ['class.py', 'class', 1],
        ['class.py', 'def', 4],
      ];
      cases.forEach(([file, token, expected]) => {
        test(`counting token '${token}'`, () => {
          expect(
            countToken(
              parseFile('python', path.join(testDir, file)),
              Counter.token,
              token,
            ),
          ).to.equal(expected);
        });
      });
    });
  });
});
