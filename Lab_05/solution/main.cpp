#include <iostream>
#include "SufTree.hpp"
#include <ctime>

using namespace std;

int main() {
    std::string inputString;
    string pattern;
    

    cin >> inputString;
    
    long start = clock();
    SUFTree tree(inputString);
    long end = clock();

    cout << "Время построения: " << end - start << endl;
    /* int iteration = 1;
    while (cin >> pattern) {
        vector<int> answer;
        tree.solution(pattern, answer);
        if (!answer.empty()) {
            cout << iteration << ": " << answer[0];
            for (int i = 1; i < answer.size(); ++i) {
                cout << ", " << answer[i];
            }
            cout << endl;
        }
        ++iteration;   
    } */
    return 0;
}