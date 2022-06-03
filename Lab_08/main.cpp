#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

long getPositionSwap(long first, long currentPosition, vector<long> &digits)
{
    if (digits.empty() || currentPosition < 0 || currentPosition >= digits.size()){
        return 0;
    }

    while (digits[currentPosition] != first && currentPosition + 1 < digits.size()) {
        ++currentPosition;
    }
    return currentPosition;
}

long getPosisionSwapReverse(long first, long currentPosition, vector<long> &digits) {
    if (digits.empty() || currentPosition < 0 || currentPosition >= digits.size()){
        return 0;
    }

    while (digits[currentPosition] != first && currentPosition - 1 > -1) {
        --currentPosition;
    }
    return currentPosition;
}

int main() {
    const int COUNT_NUMBERS = 3;
    // тут храним количество ([единиц],[двоек],[троек])
    vector<long> count(COUNT_NUMBERS);

    long n, answer = 0;
    cin >> n;
    vector<long> digits(n);
    
    for (long i = 0; i < n; ++i) {
        cin >> digits[i];
        ++(count[digits[i]- 1]);
    }

    long position_1_in_2 = getPositionSwap(1,count[0], digits);
    long position_1_in_3 = getPosisionSwapReverse(1, n - 1, digits);
    long position_2_in_3 = getPositionSwap(2, count[0] + count[1], digits);

    long time_start = clock();
    for (long i = 0; i < n; ++i) {
        if (i < count[0] && digits[i] != 1){
            if (digits[i] == 2) {
                ++answer;
                swap(digits[i], digits[position_1_in_2]);
                position_1_in_2 = getPositionSwap(1,count[0], digits);
            } else {
                ++answer;
                swap(digits[i], digits[position_1_in_3]);
                position_1_in_3 = getPosisionSwapReverse(1, n - 1, digits);
            } 
        } else if (i >= count[0] && i < count[0] + count[1] && digits[i] != 2) {
            ++answer;
            swap(digits[i], digits[position_2_in_3]);
            position_2_in_3 = getPositionSwap(2, count[0] + count[1], digits);
        }
    }
    long time_finish = clock();

    cout << "Время работы: " << (time_finish - time_start) << endl;

    return 0;
}