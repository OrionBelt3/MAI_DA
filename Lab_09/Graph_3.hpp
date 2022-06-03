#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

//задается как значение inf, т.к. больше этого числа нет ничего
const long INF = 1000000001;

// структура связи между узлами
struct Edge
{
    // задаю начальную и конечную вершину ребра
    int startVertex, endVertex;
    //длина ребра
    long weight;
};

class Graph
{
    //количество вершин и ребер
    int numberOfVertices, numberOfEdges;
    
    vector<Edge> link; //список связей (н.вершина, к.вершина, длина)
    vector<long> shortestPaths; // для Дейкстры, сюда записывает ответ
    vector<long> potentialFunctionValues; //ответ алгоритма Ф-Б
    vector<bool> usedNodes; // использованные вершины

    map<pair<long, long>, long> edges;

public:
    Graph();
    ~Graph();

    void buildGraph(); // чтение из потока ввода и заполнение данных начального графа

    void dijkstra(int startVertex);
    bool fordBelman(long startVertex);
    void johnson();

    long getWeight(int startVertex, int endVertex);
    long getMinimumValueNumber(vector<long> D, vector<bool> usedVertex);
};

Graph::Graph()
{
    buildGraph();
}

Graph::~Graph(){}

// O(E+V)
void Graph::buildGraph()
{
    Edge edge;
    cin >> numberOfVertices >> numberOfEdges;
    //запишем всю информацию о ребрах
    for(int i = 0; i < numberOfEdges; ++i){
        cin >> edge.startVertex  >> edge.endVertex  >> edge.weight;
        link.push_back(edge);
        /* edges.insert(pair<pair<long, long>, long>(make_pair(edge.startVertex, edge.endVertex ), edge.weight)); */
    }

    //инициализируем массив использованных вершин
    for(int i = 0; i < numberOfVertices; ++i){
        usedNodes.push_back(false);
    }


}

// O(E)
/* long Graph::getWeight(int startVertex, int endVertex)
{
    long result = INF;
    for(int i = 0; i < numberOfEdges; ++i){
        if(link[i].startVertex == startVertex && link[i].endVertex == endVertex){
            result = link[i].weight;
            break;
        }
    } 
    cout << "sV:" << startVertex << " eV:" << endVertex << " Res: " << result << endl;
    return result;
} */


// работает исправно!!!!!! ошибки нет тут
long Graph::getWeight(int startVertex, int endVertex)
{
    edges.clear();
    for (int i = 0; i < numberOfEdges; ++i) {
        edges.insert(pair<pair<long, long>, long>(make_pair(link[i].startVertex, link[i].endVertex ), link[i].weight));
    }
    long result = INF;
    if (edges.find(make_pair(startVertex, endVertex)) != edges.end()) {
        result = edges.find(make_pair(startVertex, endVertex))->second;
    }
    
    /* cout << "sV:" << startVertex << " eV:" << endVertex << " Res: " << result << endl; */
    return result;
}

// O(V)
long Graph::getMinimumValueNumber(vector<long> D, vector<bool> usedNodes)
{
    int minimumWeight = INF;
    int k = 0;
    for(int i = 0; i < numberOfVertices; ++i){
        if(usedNodes[i]){
            continue;
        }
        if(D[i] < minimumWeight){
            minimumWeight = D[i];
            k = i;
        }
    }
    return k;
}

//O(V*V*E)
void Graph::dijkstra(int startVertex)
{
    shortestPaths.clear();
    for(int i = 0; i < numberOfVertices; ++i){
        shortestPaths.push_back(INF);
    }

    shortestPaths[startVertex-1] = 0;
    for(int i = 0; i < numberOfEdges; ++i){
        if(link[i].startVertex == startVertex){
            shortestPaths[link[i].endVertex - 1] = link[i].weight;
        }
    }

    for(int i = 0; i < numberOfVertices; ++i){
        usedNodes[i] = false;
    }
    usedNodes[startVertex-1] = true;

    int k;
    for(int i = 1; i < numberOfVertices; ++i){
        k = getMinimumValueNumber(shortestPaths, usedNodes);
        usedNodes[k] = true;
        for(int j = 0; j < numberOfVertices; ++j){
            if(usedNodes[j]){
                continue;
            }
            shortestPaths[j] = min(shortestPaths[j], shortestPaths[k] + getWeight(k+1, j+1)); // ...убрал тут(k+1, j+1)...     здесь жрет сволочь много
        }
    }
}

//O(V*E)
bool Graph::fordBelman(long startVertex)
{
    for(int i = 0; i < numberOfVertices; ++i){
        potentialFunctionValues.push_back(1);
    }
    potentialFunctionValues[startVertex-1] = 0;


    for(int k = 0; k < numberOfVertices; ++k){
        for(int i = 0; i < numberOfEdges; ++i){
            if(potentialFunctionValues[link[i].endVertex-1] > potentialFunctionValues[link[i].startVertex-1] + link[i].weight){
                potentialFunctionValues[link[i].endVertex-1] = potentialFunctionValues[link[i].startVertex-1] + link[i].weight;
            }
        }
    } 

    /* for (int i = 0; i < numberOfEdges; ++i) {
        cout << potentialFunctionValues[i] << " ";
    }
    cout << endl; */

    for(int i = 0; i < numberOfEdges; ++i){
        if(potentialFunctionValues[link[i].endVertex-1] > potentialFunctionValues[link[i].startVertex-1] + link[i].weight){
            return false;
        }
    }
    return true;
}

//O(V*2)
void Graph::johnson()
{   
    Edge edge;
    for(int i = 0; i < numberOfVertices; ++i){
        edge.startVertex = numberOfVertices+1;
        edge.endVertex = i+1;
        edge.weight = 0;
        link.push_back(edge);
        /* edges.insert(pair<pair<long, long>, long>(make_pair(edge.startVertex, edge.endVertex ), edge.weight)); */
    }
    numberOfEdges += numberOfVertices;
    numberOfVertices += 1;

    if(fordBelman(numberOfVertices) == false){
        cout << "Negative cycle" << endl;
    } else {
        numberOfVertices -= 1;
        numberOfEdges -= numberOfVertices;
        for(int i = 0; i < numberOfVertices - 1; ++i){
            link.pop_back();

        }
        /* edges.clear(); */

        for(int i = 0; i < numberOfEdges; ++i){
            link[i].weight += (potentialFunctionValues[link[i].startVertex-1] - potentialFunctionValues[link[i].endVertex-1]);
        }

        /* map<pair<long, long>, long> :: iterator it;
        for (it = edges.begin(); it != edges.end(); ++it) {
            it->second += (potentialFunctionValues[it->first.first - 1] - potentialFunctionValues[it->first.second - 1]);
        } */



        for(int i = 0; i < numberOfVertices; ++i){
            dijkstra(i+1);
            /* cout << "K: " ;
            for (int m = 0; m < numberOfVertices; ++m) {
                cout  << shortestPaths[m] << " ";
            }
            cout << endl; */

            for(int k = 0; k < numberOfVertices; ++k){
                if(shortestPaths[k] < INF){
                    shortestPaths[k] += (potentialFunctionValues[k] - potentialFunctionValues[i]);
                }
            }


            for(int i = 0; i < numberOfVertices; ++i){
                if(shortestPaths[i] >= INF){
                    cout << "inf ";
                } else {
                    cout << shortestPaths[i] << " ";
                }
            }
            cout << endl;            
        }  
    }
}