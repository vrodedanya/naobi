import os
import subprocess

if __name__ == "__main__":
    print("Begin functional tests")
    directories = next(os.walk("."))[1]
    for directory in directories:
        print("Run " + directory)
        result = subprocess.run(["../../cmake-build-debug/./naobi", directory + "/main.naobi"])
        if result.returncode != 0:
            print("Test " + directory + " failed!")
            exit(1)
        else:
            print("Test " + directory + " passed!")
    print("All tests passed!")
