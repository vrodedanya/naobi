import os
import subprocess


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

    for directory in directories:
        args.append(directory + "/main.naobi")
        result = subprocess.run(args, stdout=subprocess.PIPE)
        if result.returncode != 0:
            print(Colors.FAIL + Colors.BOLD + "Test " + directory + " failed!" + Colors.ENDC)
            print("stdout:")
            print(result.stdout)
            exit(1)
        else:
            print(Colors.OKGREEN + Colors.BOLD + "Test " + directory + " passed!" + Colors.ENDC)
    print()
    print(Colors.OKGREEN + Colors.BOLD + "All tests passed!" + Colors.ENDC)
