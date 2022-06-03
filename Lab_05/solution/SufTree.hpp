#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class SUFTree {
public:    
    int countSuffix = 0;
    int countList = 0;
    int position = 0;
    int currentEdge;

    struct Node;
    
    Node *root;
    Node *currentNode;
    Node *lastAdd;

    string inputString;

    void addNode(int pos);
    void createList(int pos, Node *node);
    void buildSufflink(Node *node);
    void walkSufflink();
    void breakNode(int pos);
    bool edgeFault();

 
    SUFTree(string &inputString);
    ~SUFTree();
    void solution(string &pattern, vector<int> &answer);
};

struct SUFTree::Node
{
    int numberList;
    int left, right;

    Node *Sufflink = nullptr;
    map<char, Node *> edges;
    Node (int left, int right, int numberList) {
        this->left = left;
        this->right = right;
        this->numberList = numberList;
    }
    ~Node();
    void listsNumbers(vector<int> &answer);
};

SUFTree::Node::~Node(){
    for (auto &i: edges){
        delete i.second;
    }
}

SUFTree::SUFTree (string &str) 
{
    this->inputString = str + "$";

    int size = this->inputString.size();
    this->root = new Node(-1, -1, -1);
    this->currentNode = root;
    for (int i = 0; i < size; ++i) {
        addNode(i);
    }
}

SUFTree::~SUFTree()
{
    delete this->root;
}

void SUFTree::addNode(int pos)
{
    this->lastAdd = nullptr;
    ++(this->countSuffix);
    while (countSuffix) {
        if (this->position == 0) {
            this->currentEdge = pos;
        }
        // проверим существует ли ребро
        if (currentNode->edges.find(this->inputString[currentEdge]) == currentNode->edges.end()) {
            // не нашлось такого
            //создадим новый лист
            createList(pos, currentNode);
            buildSufflink(currentNode);
        } else {
            // если позиция за пределами ребра
            // переходим далее и все проверяем
            if(edgeFault()){
                continue;
            }
            // если символ нашелся, переходим в него и создаем суф-ссылку
            Node *edge = currentNode->edges[this->inputString[currentEdge]];
            if (this->inputString[edge->left+this->position] == this->inputString[pos]) {
                buildSufflink(currentNode);
                this->position++;
                break;
            } else {
                // разрываем вершину
                //привязка суф-ссылки и создание листа
                breakNode(pos);
            }
        }
        --countSuffix;
        walkSufflink();
    }
}

void SUFTree::createList(int pos, Node *node){
    ++this->countList;
    Node *list = new Node(pos, this->inputString.size() - 1, countList);
    node->edges[this->inputString[pos]] = list;
}

void SUFTree::buildSufflink(Node *node) {
    if (lastAdd != nullptr) {
        lastAdd->Sufflink = node;
        lastAdd = nullptr;
    }
}

bool SUFTree::edgeFault(){
    Node *edge = currentNode->edges[this->inputString[currentEdge]];
    int lengthEdge = edge->right - edge->left + 1;
    if (this->position >=lengthEdge) {
        currentEdge += lengthEdge;
        this->position -= lengthEdge;
        currentNode = edge;
        return true;
    }
    return false;
}

void SUFTree::breakNode(int pos){
    Node *edge = currentNode->edges[this->inputString[currentEdge]];
    Node *splitNode = new Node(edge->left,edge->left + this->position - 1,-1);
    currentNode->edges[this->inputString[currentEdge]] = splitNode;
    edge->left += this->position;
    splitNode->edges[this->inputString[edge->left]] = edge;
    createList(pos,splitNode);
    buildSufflink(splitNode);
    lastAdd = splitNode;
}


void SUFTree::walkSufflink(){
    if(currentNode == root){
        if(this->position > 0){
            --this->position;
            ++currentEdge;
        }
    } else {
        //на всякий
        if(currentNode->Sufflink != nullptr){
            currentNode = currentNode->Sufflink;
        } else {
            currentNode = root;
        }
    }
}

void SUFTree::solution(string &pattern, vector<int> &answer){
    answer.clear();
    int size = pattern.size();
    Node *current = root;
    for(int i = 0;i < size;){
        //Если есть ребро начинающиеся с нужного символаж
        if(current->edges.find(pattern[i]) != current->edges.end()){
            //Переходим в нее и сверяем
            current = current->edges[pattern[i]];
            for(int j = current->left; j <= current->right && i < size; ++i,++j){
                if(this->inputString[j] != pattern[i]){
                    return;
                }
            }
        } else { 
            return;
        }
    }
    current->listsNumbers(answer);
    sort(answer.begin(),answer.end());
}

void SUFTree::Node::listsNumbers(vector<int> &answer){
    if(numberList == -1){
        for(auto &i:edges){
            i.second->listsNumbers(answer);
        }
    } else {
        answer.push_back(numberList);
    }
}