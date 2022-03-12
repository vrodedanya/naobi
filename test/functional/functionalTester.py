import os
import subprocess
import json


class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


def is_exist(path: str):
    return os.path.isfile(path)


if __name__ == "__main__":
    print(Colors.HEADER + Colors.BOLD + "Begin functional tests" + Colors.ENDC)
    directories = next(os.walk("."))[1]
    directories.reverse()
    args = ["--enable-logger"]
    if is_exist("../../cmake-build-debug/naobi"):
        args.insert(0, "../../cmake-build-debug/./naobi")
    else:
        args.insert(0, "../../build/./naobi")

    for count, directory in enumerate(directories):
        args.append(directory + "/main.naobi")
        process = subprocess.Popen(args, stdout=subprocess.PIPE, stdin=subprocess.PIPE)

        if process.returncode != 1 and is_exist(directory + "/scenario.json"):
            f = open(directory + "/scenario.json")
            scenarios = json.loads(f.read())
            lineNumber = 0
            for scenario in scenarios:
                inputStr = ""
                if len(scenario["in"]) != 0:
                    inputStr = scenario["in"]
                var = process.communicate(inputStr)[0].decode().split(sep="\n")
                if var[lineNumber] != scenario["out"]:
                    print(Colors.FAIL + Colors.BOLD + "Test " + directory + " failed!\nExpected '" + scenario["out"] +
                          "' got '" + var[lineNumber] + "' in scenario " + str(count + 1))
                    exit(1)
                lineNumber += 1

        process.wait()
        if process.returncode != 0:
            print(Colors.FAIL + Colors.BOLD + "Test " + directory + " failed!" + Colors.ENDC)
            exit(1)
        else:
            print(Colors.OKGREEN + Colors.BOLD + "Test " + directory + " passed!" + Colors.ENDC)
    print()
    print(Colors.OKGREEN + Colors.BOLD + "All tests passed!" + Colors.ENDC)
