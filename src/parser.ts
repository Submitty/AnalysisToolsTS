import fs from 'fs';
import Parser from 'tree-sitter';
import CLanguage from 'tree-sitter-c';
import CppLanguage from 'tree-sitter-cpp';
import JavaLanguage from 'tree-sitter-java';
import PythonLanguage from 'tree-sitter-python';

import { Language } from './types';

function getParseLanguage(language: Language): unknown {
  switch (language) {
    case Language.python:
      return PythonLanguage;
    case Language.c:
      return CLanguage;
    case Language.cpp:
      return CppLanguage;
    case Language.java:
      return JavaLanguage;
  }
}

export function parseFile(language: Language, file: string): Parser.Tree {
  const parser = new Parser();
  parser.setLanguage(getParseLanguage(language));
  return parser.parse(fs.readFileSync(file, 'utf8'));
}
