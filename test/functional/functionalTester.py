import os
import subprocess
import sys


class bcolors:
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
    print(bcolors.HEADER + bcolors.BOLD + "Begin functional tests" + bcolors.ENDC)
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
            print(bcolors.FAIL + bcolors.BOLD + "Test " + directory + " failed!" + bcolors.ENDC)
            exit(1)
        else:
            print(bcolors.OKGREEN + bcolors.BOLD + "Test " + directory + " passed!" + bcolors.ENDC)
    print()
    print(bcolors.OKGREEN + bcolors.BOLD + "All tests passed!" + bcolors.ENDC)
