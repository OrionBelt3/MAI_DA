import random as rd

from asyncore import write
from random import random


k = int(input("кол-во тестов: "))

operation = ["+", "-", "*", "/", ">", "<", "=", "^"]

file = open("test.txt", "w")
for i in range(k):
    op = operation[rd.randint(0, 6)]
    if op == "^":
        first = rd.randint(0, 10000000000)
        second = rd.randint(0, 1000)
    else:
        first = rd.randint(0, 10000000000000000000000000000000000)
        second = rd.randint(0, 10000000000000000000000000000000000)
    st = str(first) + "\n" +  str(second) + "\n" + op + "\n"
    file.write(st)
file.close()