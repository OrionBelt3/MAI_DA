from random import randint


k = int(input("Количество символов строки: "))

file = open("test", "w")
my_str = str(k) + "\n"
while (k != 0):
    n = randint(1, 3)
    my_str = my_str + str(n) + " "
    k = k - 1
file.write(my_str)

file.close()