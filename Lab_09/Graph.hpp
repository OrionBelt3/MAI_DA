#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

const long INF = 1000000001;

struct Vertex
{
    int perentVertexNumber , connectedVertexNumber ;
    long w;
};

class Graph
{
    int v, e;
    vector<Vertex> conection;
    vector<int> phi;
    vector<long> D;

public:
    Graph();
    ~Graph();
    void buildGraph();
    long getWeight(int nodeFrom, int nodeTo);
    long getNumberMinWeight(vector<long> D, set<int> usedNode);
    void dijkstra(int vertex);
    void djonson();
    bool fordBelman(long s);
    void solve();
};

Graph::Graph()
{
    buildGraph();
}

Graph::~Graph()
{
}

void Graph::buildGraph()
{
    Vertex vertex;
    cin >> v >> e;
    for(int k = 0; k < e; ++k){
        cin >> vertex.perentVertexNumber  >> vertex.connectedVertexNumber  >> vertex.w;
        conection.push_back(vertex);
    }

    /* cout << endl;
    for(int i = 0; i < e; ++i){
        cout << conection[i].perentVertexNumber  << " " << conection[i].connectedVertexNumber  << " " << conection[i].w << endl;
    }
    cout << endl; */
}

long Graph::getWeight(int nodeFrom, int nodeTo)
{
    long weight;
    for(int i = 0; i < e; ++i){
        if(conection[i].perentVertexNumber == nodeFrom && conection[i].connectedVertexNumber == nodeTo){
            weight = conection[i].w;
            break;
        } else {
            weight = INF;
        }
    }
    
    return weight;
}

long Graph::getNumberMinWeight(vector<long> D, set<int> usedNodes)
{
    int minWeight = INF;
    int k = 0;
    for(int i = 0; i < v; ++i){
        if(usedNodes.count(i+1) != 0){
            continue;
        }
        if(D[i] < minWeight){
            minWeight = D[i];
            k = i;
        }
    }
    return k;
}

void Graph::dijkstra(int vertex)
{
    set<int> usedNodes;
    usedNodes.insert(vertex); // помещаем в множество стартовую вершину
     
    // заполнение связей n-ой бесконечностями
    for(int i = 0; i < v; ++i){
        D.push_back(INF);
    }
    D[vertex-1] = 0; // вершине, которую мы рассматриваем в качестве стартовой, присваиваем 0


    // расстоновка весов с вершины n
    for(int i = 0; i < e; ++i){
        if(conection[i].perentVertexNumber == vertex){
            D[conection[i].connectedVertexNumber - 1] = conection[i].w;
        }
    }

    int k;
    for(int i = 1; i < v; ++i){
        k = getNumberMinWeight(D, usedNodes);
        usedNodes.insert(k+1);
        for(int j = 0; j < v; ++j){
            if(usedNodes.count(j+1) != 0){
                continue;
            }
            D[j] = min(D[j], D[k] + getWeight(k+1, j+1));
        }
    }

    /* for(int i = 0; i < v; ++i){
        if(D[i] >= INF){
            cout << "inf ";
        } else {
            cout << D[i] << " ";
        }
    }
    cout << endl; */

    //D.clear();
}

void Graph::solve()
{
    for(int i = 0; i < v; ++i){
        djonson();
    }
    
}

void Graph::djonson()
{
    if(fordBelman(1) == false){
        cout << "FAIL" << endl;
    } else {
        for(int i = 0; i < e; ++i){
            conection[i].w += (phi[conection[i].perentVertexNumber] - phi[conection[i].connectedVertexNumber]);
        }
        for(int i = 0; i < v; ++i){
            dijkstra(i);
            for(int n = 0; n < v; ++n){
                D[n] += (phi[n] - phi[i]);
                cout << D[n] << " ";
            }
            cout << endl;
            D.clear();
        }    
    }
}

bool Graph::fordBelman(long s)
{
    for(int i = 0; i < v; ++i){
        phi.push_back(1);
    }
    phi[s-1] = 0;

    for(int i = 0; i < v; ++i){
        cout << phi[i] << " "; 
    }
    cout << endl;

    for(int k = 0; k < v; ++k){
        for(int i = 0; i < e; ++i){
            if(phi[conection[i].connectedVertexNumber-1] > phi[conection[i].perentVertexNumber-1] + conection[i].w){
                phi[conection[i].connectedVertexNumber-1] = phi[conection[i].perentVertexNumber-1] + conection[i].w;
            }
        }
    } 

    for(int i = 0; i < e; ++i){
        if(phi[conection[i].connectedVertexNumber-1] > phi[conection[i].perentVertexNumber-1] + conection[i].w){
            return false;
        }
    }
    return true;
}