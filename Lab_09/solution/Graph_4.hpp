#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const long INF = 1e18;

struct Edge 
{
    // задаю начальную и конечную вершину ребра
    int startVertex, endVertex;
    //длина ребра
    long weight;
};

// пункт и путь который мы прошли до этого пункта
struct item
{
    int id;
    long weight;

    friend bool operator < (const item & first, const item second) {
        if (first.weight != second.weight) {
            return first.weight > second.weight;
        } else {
            return first.id < second.id;
        }
    }
};

class Graph
{
    //количество вершин и ребер
    int numberOfVertices, numberOfEdges;

    vector<Edge> edges; //список связей (н.вершина, к.вершина, длина)
    //для Дейкстры
    vector<vector< pair<int, long> > > g;
    // для Ф-Б
    vector<long> potentialFunctionValues; //ответ алгоритма Ф-Б


public:
    Graph();
    ~Graph();

    void build();
    bool fordBelman();
    void dijkstra(int startVertex);
    void johnson();


};

Graph::Graph()
{
    build();
}

Graph::~Graph() {} 

void Graph::build() 
{
    cin >> numberOfVertices >> numberOfEdges;
    g.resize(numberOfVertices);
    edges.resize(numberOfEdges);

    for (int i = 0; i < numberOfEdges; ++i) {
        int startVertex, endVertex;
        long weight;
        cin >> startVertex >> endVertex >> weight;
        edges[i] = {startVertex - 1, endVertex - 1, weight};
        g[startVertex - 1].push_back(make_pair(endVertex - 1, weight));
    }

    // строим доп. вершину с нулевыми весами
    for (int i = 0; i < numberOfVertices; ++i) {
        edges.push_back({numberOfVertices, i, 0});
    }

    // инициализация функции перехода
    potentialFunctionValues.resize(numberOfVertices+1);
    for (int i = 0; i < numberOfVertices + 1; ++i) {
        potentialFunctionValues[i] = INF;
    }
}


// под подозрением только дейкстра
void Graph::dijkstra(int startVertex) 
{
    vector<long> shortestPaths(numberOfVertices, INF); // для Дейкстры вектор длин
    vector<bool> usedVertex(numberOfVertices); //использованные вершины
    priority_queue<item> pq;

    shortestPaths[startVertex] = 0;
    pq.push({startVertex, 0});
    while (!pq.empty()) {
        item current = pq.top();
        pq.pop();
        int u = current.id;
        if (!usedVertex[u]) {
            for (long i = 0; i < g[u].size(); ++i) {
                int v = g[u][i].first;
                long w = g[u][i].second;
                if (shortestPaths[u] + w < shortestPaths[v]) {
                    shortestPaths[v] = shortestPaths[u] + w;
                    pq.push({v, shortestPaths[v]});
                }
            }
            usedVertex[u] = true;
        }
    }

    for (int k = 0; k < numberOfVertices; ++k) {
        if (shortestPaths[k] >= INF) {
            cout << "inf ";
        } else {
            cout << shortestPaths[k] + potentialFunctionValues[k] - potentialFunctionValues[startVertex] << " ";
    
        }
    }
    cout << endl;
}


// тут все работает.... где блин ошибка?
bool Graph::fordBelman() 
{
    potentialFunctionValues[numberOfVertices] = 0;
    bool changed = true;
    for (int i = 0; changed and i < numberOfVertices + 1; ++i) {
        changed = false;
        for (long j = 0; j < edges.size(); ++j) {
            int from = edges[j].startVertex;
            int to = edges[j].endVertex;
            long weight = edges[j].weight;
            if (potentialFunctionValues[from] + weight < potentialFunctionValues[to]) {
                changed = true;
                potentialFunctionValues[to] = potentialFunctionValues[from] + weight;
            }
        }
    }
    return changed;
}

void Graph::johnson()
{
    if (fordBelman()) {
        cout << "Negative cycle" << endl;
    } else {
        // подменочка весов бэнг-бэнг
        for (int i = 0; i < numberOfVertices; ++i) {
            for (long j = 0; j < g[i].size(); ++j) {
                int currentVertex = g[i][j].first;
                g[i][j].second = g[i][j].second + potentialFunctionValues[i] - potentialFunctionValues[currentVertex];
            }
        }

        // призываю Дейкстру
        for (int i = 0; i < numberOfVertices; ++i) {
            dijkstra(i);
        } 
    }
}
