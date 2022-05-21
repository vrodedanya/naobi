import os
import subprocess

from radish import given, then, when


# Support functions

def is_exist(path: str):
    return os.path.isfile(path)


def is_process_running(process):
    return process.poll() is None


logs = ""


def take_logs(byte_lines):
    utf8_lines = []
    for line in byte_lines:
        utf8_lines.append(line.decode("UTF-8"))
    temp = "".join(utf8_lines)
    if len(temp) == 0:
        global logs
        temp = logs
        logs = ""
    return temp


# Steps


@given("script:")
def take_script(step):
    args = ["--script", step.text, "--print-compile-end"]
    if is_exist("../../cmake-build-debug/naobi"):
        args.insert(0, "../../cmake-build-debug/./naobi")
    else:
        args.insert(0, "../../build/./naobi")

    step.context.process = subprocess.Popen(args, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
    while True:
        if is_process_running(step.context.process):
            out = step.context.process.stderr.readline().decode().replace('\n', '')
            if out == "compile_end":
                break
            else:
                global logs
                logs = logs + out
        else:
            break


@when("pass integer {number:d}")
def take_input(step, number):
    if not is_process_running(step.context.process):
        raise ChildProcessError("Naobi interpreter is already finished with code: "
                                f"{step.context.process.returncode}\nLogs:\n"
                                f"{take_logs(step.context.process.stderr.readlines())}")
    step.context.process.stdin.write(bytes(str(number) + '\n', "UTF-8"))
    step.context.process.stdin.flush()


@when("pass float {number:f}")
def take_input(step, number):
    if not is_process_running(step.context.process):
        raise ChildProcessError("Naobi interpreter is already finished with code: "
                                f"{step.context.process.returncode}\nLogs:\n"
                                f"{take_logs(step.context.process.stderr.readlines())}")
    step.context.process.stdin.write(bytes(str(number) + '\n', "UTF-8"))
    step.context.process.stdin.flush()


@when("pass boolean {var:Boolean}")
def take_input(step, var):
    if not is_process_running(step.context.process):
        raise ChildProcessError("Naobi interpreter is already finished with code: "
                                f"{step.context.process.returncode}\nLogs:\n"
                                f"{take_logs(step.context.process.stderr.readlines())}")
    step.context.process.stdin.write(bytes(str(var).lower() + '\n', "UTF-8"))
    step.context.process.stdin.flush()


@when("pass string {string:QuotedString}")
def take_input(step, string):
    if not is_process_running(step.context.process):
        raise ChildProcessError("Naobi interpreter is already finished with code: "
                                f"{step.context.process.returncode}\nLogs:\n"
                                f"{take_logs(step.context.process.stderr.readlines())}")
    step.context.process.stdin.write(bytes(string + '\n', "UTF-8"))
    step.context.process.stdin.flush()


@then("got integer {number:d}")
def take_result(step, number):
    if not is_process_running(step.context.process):
        if step.context.process.returncode != 0:
            raise ChildProcessError(f"Naobi exit with code: "
                                    f"{step.context.process.returncode} \n Logs:\n"
                                    f"{take_logs(step.context.process.stderr.readlines())}")
    out = step.context.process.stdout.readline().decode().replace('\n', '')
    if len(out) == 0:
        raise AssertionError("Empty output")
    integer = int(out)
    if integer != number:
        raise AssertionError(f"Expected {number} got {integer}")


@then("got float {number:f}")
def take_result(step, number):
    if not is_process_running(step.context.process):
        if step.context.process.returncode != 0:
            raise ChildProcessError(f"Naobi exit with code: "
                                    f"{step.context.process.returncode} \n Logs:\n"
                                    f"{take_logs(step.context.process.stderr.readlines())}")
    out = step.context.process.stdout.readline().decode().replace('\n', '')
    if len(out) == 0:
        raise AssertionError("Empty output")
    temp = float(out)
    if temp != number:
        raise AssertionError(f"Expected {number} got {temp}")


@then("got string {string:QuotedString}")
def take_result(step, string):
    if not is_process_running(step.context.process):
        if step.context.process.returncode != 0:
            raise ChildProcessError(f"Naobi exit with code: "
                                    f"{step.context.process.returncode} \n Logs:\n"
                                    f"{take_logs(step.context.process.stderr.readlines())}")
    out = step.context.process.stdout.readline().decode().replace('\n', '')
    if len(out) == 0:
        raise AssertionError("Empty output")
    temp = out
    if temp != string:
        raise AssertionError(f"Expected {string} got {temp}")


@then("got boolean {var:Boolean}")
def take_result(step, var):
    if not is_process_running(step.context.process):
        if step.context.process.returncode != 0:
            raise ChildProcessError(f"Naobi exit with code: "
                                    f"{str(step.context.process.returncode)} \n Logs:\n"
                                    f"{take_logs(step.context.process.stderr.readlines())}")
    out = step.context.process.stdout.readline().decode().replace('\n', '')
    if len(out) == 0:
        raise AssertionError("Empty output")
    temp = out
    if temp != str(var).lower():
        raise AssertionError(f"Expected {var} got {temp}")


@then("ends with the code {number:d}")
def finish_with(step, number):
    if is_process_running(step.context.process):
        step.context.process.wait()
    if step.context.process.returncode != number:
        raise AssertionError(f"Expected {number} got {step.context.process.returncode} with logs\n"
                             f"{take_logs(step.context.process.stderr.readlines())}")
    else:
        take_logs(step.context.process.stderr.readlines())
