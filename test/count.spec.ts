import { expect } from 'chai';
import { describe, test } from 'mocha';
import path from 'path';

import { Counter, countToken } from '../src/count';
import { parseFile } from '../src/parser';

import { TEST_DATA } from './constants';

describe('countToken', () => {
  describe('python', () => {
    const testDir = path.join(TEST_DATA, 'python');
    const cases: [string, number][] = [
      ['while', 2],
      ['for', 3],
    ];
    cases.forEach(([token, expected]) => {
      test(`counting ${token} loops`, () => {
        expect(
          countToken(
            parseFile('python', path.join(testDir, 'loops.py')),
            Counter.node,
            token,
          ),
        ).to.equal(expected);
      });
    });
  });
});
