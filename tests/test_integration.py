from pathlib import Path
import subprocess
import json
import pytest
from typing import Dict, List, Tuple

this_dir = Path(__file__).parent.resolve()
fixtures_dir = this_dir / 'fixtures'
exe_path = this_dir / '..' / 'build' / 'submitty_count_ts'

def execute_command(file: str, command: str) -> int:
    args = [exe_path, '-l', file.split('.'[-1]), command, str(this_dir / 'fixtures' / file)]
    result = subprocess.run(args, capture_output=True)
    output = result.stdout.decode().strip("\n")
    if not output.isnumeric():
        raise Exception("Result is not numeric")
    return int(output)


with (this_dir / 'fixtures' / 'commands.json').open() as file:
    commands: Dict[str, List[Tuple[str, int]]] = json.load(file)

testcases = []

file: str
for file in commands:
    for test in commands[file]:
        testcases.append([file, test[0], test[1]])

@pytest.mark.parametrize("file, command, expected_output", testcases)
def test_integration(file, command, expected_output):
    assert expected_output == execute_command(file, command)
