import { expect } from 'chai';
import { describe, test } from 'mocha';
import path from 'path';

import { countToken, Feature, getCountableFeature } from '../src/count';
import { parseFile } from '../src/parser';
import { Language } from '../src/types';

import { TEST_DATA } from './constants';

describe('getCounter', () => {
  test('throws error on unknown counter', () => {
    expect(() => getCountableFeature('invalid')).to.throw(
      'Unknown countable feature: invalid',
    );
  });

  const cases: [string, Feature][] = [
    ['token', Feature.token],
    ['node', Feature.node],
    ['call', Feature.call],
  ];
  cases.forEach(([input, expected]) => {
    test(`returns ${expected} for ${input}`, () => {
      expect(getCountableFeature(input)).to.equal(expected);
    });
  });
});

describe('countToken', () => {
  describe('token', () => {
    describe('python', () => {
      const testDir = path.join(TEST_DATA, 'python');
      const cases: [string, string, number][] = [
        ['loops.py', 'while', 2],
        ['loops.py', 'While', 2],
        ['loops.py', 'for', 3],
        ['loop_comprehensions.py', 'for', 4],
        ['class.py', 'class', 1],
        ['class.py', 'def', 4],
        ['operators.py', 'colon', 7],
      ];
      cases.forEach(([file, token, expected]) => {
        test(`counting token '${token}'`, () => {
          expect(
            countToken(
              parseFile(Language.python, path.join(testDir, file)),
              Feature.token,
              token,
            ),
          ).to.equal(expected);
        });
      });
    });
  });
});
