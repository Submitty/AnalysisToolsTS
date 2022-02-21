import { CommanderError, InvalidArgumentError, program } from 'commander';

import { countToken, getCountableFeature } from './count';
import { diagnostics } from './diagnostics';
import { parseFile } from './parser';
import { Language } from './types';

interface CommanderOptions {
  language: Language;
}

function parseLanguageOption(language: string): Language {
  switch (language) {
    case 'py':
    case 'python':
      return Language.python;
    case 'c':
      return Language.c;
    case 'c++':
    case 'cpp':
      return Language.cpp;
    case 'java':
      return Language.java;
    case undefined:
      throw new InvalidArgumentError('No language specified');
    default:
      throw new InvalidArgumentError(`Unknown Language option: ${language}`);
  }
}

program.version('1.0.0');

program.requiredOption(
  '-l, --language <language>',
  'Language of student code',
  parseLanguageOption,
);

program
  .command('count')
  .argument('<feature>')
  .argument('<token>')
  .argument('<files...>')
  .action((feature: string, token: string, files: string[]) => {
    const options = program.opts<CommanderOptions>();
    const countedFeatures = files.reduce((acc, file) => {
      const tree = parseFile(options.language, file);
      try {
        acc += countToken(tree, getCountableFeature(feature), token);
      } catch (error) {
        throw new CommanderError(1, 'error.count', (error as Error).message);
      }
      return acc;
    }, 0);
    console.log(countedFeatures);
  });

program
  .command('diagnostics')
  .argument('<files...>')
  .action((files: string[]) => {
    const options = program.opts<CommanderOptions>();
    const filesDiagnostics = files.reduce((acc, file) => {
      const tree = parseFile(options.language, file);
      acc[file] = diagnostics(tree);
      return acc;
    }, {});
    console.log(JSON.stringify(filesDiagnostics, null, 2));
  });

program.parse(process.argv);
