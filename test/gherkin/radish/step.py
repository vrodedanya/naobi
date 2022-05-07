import os
import subprocess
import time

from radish import given, then, when


# Support functions

def is_exist(path: str):
    return os.path.isfile(path)


def is_process_running(process):
    return process.poll() is None


def take_logs(byte_lines):
    utf8_lines = []
    for line in byte_lines:
        utf8_lines.append(line.decode("UTF-8"))
    return "".join(utf8_lines)


# Steps


@given("script:")
def take_script(step):
    args = ["--script", step.text]
    if is_exist("../../cmake-build-debug/naobi"):
        args.insert(0, "../../cmake-build-debug/./naobi")
    else:
        args.insert(0, "../../build/./naobi")

    step.context.process = subprocess.Popen(args, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
    # Waiting for compilation
    time.sleep(0.3)


@when("pass {number:d}")
def take_input(step, number):
    if not is_process_running(step.context.process):
        raise ChildProcessError("Naobi interpreter is already finished with code: "
                                f"{str(step.context.process.returncode)}\nLogs:\n"
                                f"{take_logs(step.context.process.stderr.readlines())}")
    step.context.process.stdin.write(bytes(str(number) + '\n', "UTF-8"))
    step.context.process.stdin.flush()


@then("got {number:d}")
def take_result(step, number):
    if not is_process_running(step.context.process):
        if step.context.process.returncode != 0:
            raise ChildProcessError(f"Naobi exit with code: "
                                    f"{str(step.context.process.returncode)} \n Logs:\n"
                                    f"{take_logs(step.context.process.stderr.readlines())}")
    out = step.context.process.stdout.readline().decode().replace('\n', '')
    if len(out) == 0:
        raise AssertionError("Empty output")
    integer = int(out)
    if integer != number:
        raise AssertionError(f"Expected {str(number)} got {str(integer)}")


@then("ends with the code {number:d}")
def finish_with(step, number):
    if is_process_running(step.context.process):
        step.context.process.wait()
    if step.context.process.returncode != number:
        raise AssertionError(f"Expected {number} got {step.context.process.returncode}")
