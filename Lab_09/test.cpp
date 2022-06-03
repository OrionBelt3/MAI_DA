#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

int main(){
    vector<int> v;

    v.push_back(10);
    v.push_back(3);
    v.push_back(7);
    vector<int>::iterator it = find(v.begin(), v.end(), 7);
    if(it != v.end()){
        cout << "Нашел!" << endl;
    } else {
        cout << "НЕ нашел!" << endl;
    }
}