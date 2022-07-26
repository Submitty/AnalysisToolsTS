import subprocess
import json

exe_path = "build/submitty_count_ts"
commands_json = "testSuite/commands.json"
failed_commands= []

def execute_command(command):
    args = [exe_path]
    args.extend(command.split(" "))
    result = subprocess.run(args, capture_output=True)
    output = result.stdout.decode().strip("\n")
    if (not output.isnumeric()):
        raise Exception("Result is not numeric")
    return int(output)

json_file = open(commands_json)
files_commands = json.load(json_file)

for file in files_commands:
    tests = files_commands[file]
    for test in tests:
        command = list(test.keys())[0]
        expected_output = int(test[command])
        output = execute_command(command)
        if (output != expected_output):
            failed_commands.append("{} @expected_output = {}, @output={}.".format(command, expected_output, output))

if (len(failed_commands) > 0):
    exit("Failed commands:\n" +"\n".join(failed_commands))
