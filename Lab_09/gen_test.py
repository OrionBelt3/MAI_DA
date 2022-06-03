import random
vertex, edge = int(input("Кол-во вершин: ")), int(input("Количество ребер: "))

file = open("test", "w")

st = str(vertex) + " " + str(edge) + "\n"
for i in range(edge):
    vertexFrom = random.randint(1, vertex)
    vertexTo = random.randint(1, vertex)
    if vertexFrom == vertexTo:
        if vertexTo + 1 <=  vertex:
            vertexTo = vertexTo + 1
        else:
            vertexTo = vertexTo + 1
    weight = random.randint(-10000, 1000000000)

    st = st + str(vertexFrom) + " " + str(vertexTo) + " " + str(weight) + "\n"
file.write(st)
file.close()