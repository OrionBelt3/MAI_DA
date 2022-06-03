#include <iostream>
#include "BigInt.hpp"

using namespace std;

int main(){

    string leftStr, rightStr;
    char operation;
    bool res;
    BigInt zero(0, true);

    while (cin >> leftStr >> rightStr >> operation) {
        BigInt first(leftStr);
        BigInt second(rightStr);

        switch (operation)
        {
        case '+':
            cout << first + second << "\n";
            break;
        
        case '-':
            if (first < second ) {
                cout << "Error" << "\n";
            } else {
                cout << first - second << "\n";
            }
            break;

        case '*':
            cout << first * second << "\n";
            break;

        case '/':
            if (second == zero) {
                cout << "Error" << "\n";
            } else if (first < second){
                cout << "0" << "\n";
            } else {
                cout << first / second << "\n";
            }
            break;

        case '^':
            if (first == zero && second == zero) {
                cout << "Error" << "\n";
            } else {
                cout << (first ^ second) << "\n";
            }
            break;

        case '<':
            if (first < second) {
                cout << "true" << "\n";
            } else {
                cout << "false" << "\n";
            }
            break;

        case '>':
            if (first > second) {
                cout << "true" << "\n";
            } else {
                cout << "false" << "\n";
            }
            break;

        case '=':
            if (first == second) {
                cout << "true" << "\n";
            } else {
                cout << "false" << "\n";
            }
            break;
        
        default:
            cout << "Error" << "\n";
            break;
        }
    }
    
    return 0;
}