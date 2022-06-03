#include <iostream>
#include "BigInt.hpp"
#include <ctime>

using namespace std;

int main(){

    string leftStr, rightStr;
    char operation;
    bool res;
    BigInt zero(0, true);

    int timeStart = clock();
    while (cin >> leftStr >> rightStr >> operation) {
        BigInt first(leftStr);
        BigInt second(rightStr);

        switch (operation)
        {
        case '+':
            first + second;
            break;
        
        case '-':
            if (first < second ) {
                //cout << "Error" << "\n";
            } else {
                first - second;
            }
            break;

        case '*':
            first * second;
            break;

        case '/':
            if (second == zero) {
                //cout << "Error" << "\n";
            } else if (first < second){
                //cout << "0" << "\n";
            } else {
                first / second ;
            }
            break;

        case '^':
            if (first == zero && second == zero) {
                cout << "Error" << "\n";
            } else {
                first ^ second;
            }
            break;

        case '<':
            if (first < second) {
                //cout << "true" << "\n";
            } else {
                //cout << "false" << "\n";
            }
            break;

        case '>':
            if (first > second) {
                //cout << "true" << "\n";
            } else {
                //cout << "false" << "\n";
            }
            break;

        case '=':
            if (first == second) {
                //cout << "true" << "\n";
            } else {
                //cout << "false" << "\n";
            }
            break;
        
        default:
            //cout << "Error" << "\n";
            break;
        }
    }
    int timeEnd = clock();

    cout << "Время работы: " << (timeEnd - timeStart)*100 << endl;
    
    return 0;
}