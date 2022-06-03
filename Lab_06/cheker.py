import re
import time
from pexpect import EOF
file = open("test.txt", "r")

tic = time.perf_counter_ns()

while  True:
    line = file.readline()
    if not line:
        break
    first = int(line)
    second = int(file.readline())
    op = file.readline()


    if op == "+\n":
        first + second
    elif op == "-\n":
        first - second
    elif op == "*\n":
        first * second
    elif op == "/\n":
        first // second
    elif op == "^\n":
        first**second
    elif op == "<\n":
        first < second
    elif op == ">\n":
        first > second
    elif op == "=\n":
        first == second

toc = time.perf_counter_ns()
print(toc - tic)

file.close()