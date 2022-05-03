import os
import subprocess

from radish import given, then


def is_exist(path: str):
    return os.path.isfile(path)


@given("script:")
def take_script(step):
    args = ["--script", step.text]
    if is_exist("../../cmake-build-debug/naobi"):
        args.insert(0, "../../cmake-build-debug/./naobi")
    else:
        args.insert(0, "../../build/./naobi")

    step.context.process = subprocess.Popen(args, stdout=subprocess.PIPE, stdin=subprocess.PIPE)


@then("got {number:d}")
def take_result(step, number):
    out = step.context.process.stdout.readline().decode().replace('\n', '')
    integer = int(out)
    if integer != number:
        raise AssertionError("Expected " + str(number) + " got " + str(integer))
