import os
import subprocess


def is_exist(path : str):
    return os.path.isfile(path)


if __name__ == "__main__":
    print("Begin functional tests")
    directories = next(os.walk("."))[1]
    for directory in directories:
        print("Run " + directory)
        if is_exist("../../cmake-build-debug/naobi"):
            result = subprocess.run(["../../cmake-build-debug/./naobi", directory + "/main.naobi"])
        else:
            result = subprocess.run(["../../build/./naobi", directory + "/main.naobi"])

        if result.returncode != 0:
            print("Test " + directory + " failed!")
            exit(1)
        else:
            print("Test " + directory + " passed!")
    print("All tests passed!")
