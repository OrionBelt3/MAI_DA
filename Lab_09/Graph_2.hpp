#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

const long INF = 1000000001;

struct Edge
{
    int startVertex, endVertex;
    long weight;
};

class Graph
{
    int numberOfVertices, numberOfEdges;
    vector<Edge> link;
    vector<long> shortestPaths;
    vector<long> potentialFunctionValues;
    vector<int> usedNodes;

public:
    Graph();
    ~Graph();
    void buildGraph(); // чтение из потока ввода и заполнение данных начального графа

    void dijkstra(int startVertex);
    bool fordBelman(long startVertex);
    void johnson();

    long getWeight(int startVertex, int endVertex);
    long getMinimumValueNumber(vector<long> D, vector<int> usedVertex);
};

Graph::Graph()
{
    buildGraph();
}

Graph::~Graph(){}

// O(E)
void Graph::buildGraph()
{
    Edge edge;
    cin >> numberOfVertices >> numberOfEdges;
    for(int i = 0; i < numberOfEdges; ++i){
        cin >> edge.startVertex  >> edge.endVertex  >> edge.weight;
        link.push_back(edge);
    }
}

// O(E)
long Graph::getWeight(int startVertex, int endVertex)
{
    long result = INF;
    for(int i = 0; i < numberOfEdges; ++i){
        if(link[i].startVertex == startVertex && link[i].endVertex == endVertex){
            result = link[i].weight;
            break;
        }
    } 
    return result;
}

// O(V*V)
long Graph::getMinimumValueNumber(vector<long> D, vector<int> usedNodes)
{
    int minimumWeight = INF;
    int k = 0;
    for(int i = 0; i < numberOfVertices; ++i){
        vector<int>::iterator it = find(usedNodes.begin(), usedNodes.end(), i+1);
        if(it != usedNodes.end()){
            continue;
        }
        if(D[i] < minimumWeight){
            minimumWeight = D[i];
            k = i;
        }
    }
    return k;
}

//O(V^2)
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

    usedNodes.clear();
    usedNodes.push_back(startVertex);

    int k;
    for(int i = 1; i < numberOfVertices; ++i){
        k = getMinimumValueNumber(shortestPaths, usedNodes);
        usedNodes.push_back(k+1);
        for(int j = 0; j < numberOfVertices; ++j){
            vector<int>::iterator it = find(usedNodes.begin(), usedNodes.end(), j+1);
            if(it != usedNodes.end()){
                continue;
            }
            shortestPaths[j] = min(shortestPaths[j], shortestPaths[k] + getWeight(k+1, j+1));
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

        for(int i = 0; i < numberOfEdges; ++i){
            link[i].weight += (potentialFunctionValues[link[i].startVertex-1] - potentialFunctionValues[link[i].endVertex-1]);
        }


        for(int i = 0; i < numberOfVertices; ++i){
            dijkstra(i+1);

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