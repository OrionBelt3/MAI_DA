#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

//задаем основание СС
const long long BASE = 1000000000;
//колличество цифр в нашей cc проецируемо на 10
const long long DIGIT_LENGTH = ceil(log10(BASE));

class BigInt
{
private:
    vector<long long> digits;
    void removeZeroes();
public:
    //конструктор задает размер равный size
    BigInt(long long size);
    //конструктор преобразовывыет переданное на вход число в нашу СС
    BigInt(long long value, bool flag);//flag - это костыль, чтоб отличить от предыдущего конструктора
    //конструктор приобразовывает строку в число в нашей сс
    BigInt(const string &value);
    ~BigInt();

    //перегрузка арифметических опператоров
    BigInt operator+(BigInt &second);
    BigInt operator-(BigInt &second);
    BigInt operator*(BigInt &second);
    BigInt operator/(BigInt &second);
    BigInt operator^(BigInt &second);

    //перегрузка булевских операторов
    bool operator>(BigInt &second);
    bool operator<(BigInt &second);
    bool operator==(BigInt &second);

    //перегрузка для вывода
    friend ostream &operator<<(ostream &out, BigInt number);

    //получить размер числа
    long long getSizeNumber();
    //получить элемент по индексу (нужен чтоб случайно не обратиться к занятому куску памяти)
    long long getDigit(long long id);


};

BigInt::BigInt(long long size)
{
    if(size == 0){
        digits.push_back(0);
    } else {
        digits.resize(size);
    }
}

BigInt::BigInt(long long value, bool flag) 
{
    if (flag) {
        if (value == 0) {
           digits.push_back(0); 
        } else {
            for (long long i = value; i > 0; i /= BASE) {
                digits.push_back(i % BASE);
            }
        }
    }
}

BigInt::BigInt(const string &value) 
{
    for (long long i = value.length() - 1; i >= 0; i -= DIGIT_LENGTH) {
        long long currentDigit = 0;
        long long currentDegree = 1;

        for(long long k = 0; k < DIGIT_LENGTH && i >=k; ++k){
            currentDigit += (value[i - k] - '0') * currentDegree;
            currentDegree *= 10;
        }
        digits.push_back(currentDigit);
    }
    removeZeroes();
}
BigInt::~BigInt(){}




BigInt BigInt::operator+(BigInt &second)
{   
    //выбираем размер ответа
    long long sizeNumber = max(getSizeNumber(), second.getSizeNumber()) + 1;
    BigInt sum(sizeNumber);

    //избыток при сложении
    long long excess = 0;
    //пробегаемся по числам и складывам соответствующие разряды
    // стандартное деление в столбик
    for(long long i = 0; i < sizeNumber - 1; ++i){ // здесь падает
        sum.digits[i] = getDigit(i) + second.getDigit(i) + excess; // обратился к несуществующему куску памяти, тут артефакт появился
        excess = sum.digits[i] / BASE;
        sum.digits[i] %= BASE;    
    }

    //избыток который не смог)))
    if (excess > 0) {
        sum.digits[sum.getSizeNumber() - 1] = excess;
    }

    //убрали нули
    sum.removeZeroes();
    return sum;
}

BigInt BigInt::operator-(BigInt &second)
{
    long long sizeNumber = getSizeNumber();
    BigInt dif(sizeNumber);

    //избыток при вычитании
    long long excess = 0;

    //стандартное вычитание в столбик
    for (long long i = 0; i < sizeNumber; ++i) {
        dif.digits[i] = getDigit(i) - second.getDigit(i) - excess;

        if (dif.digits[i] < 0) {
            dif.digits[i] += BASE;
            excess = 1;
        } else {
            excess = 0;
        }
    }

    dif.removeZeroes();
    return dif;
}


/* BigInt BigInt::operator*(BigInt &second)
{
    long long sizeNumber = getSizeNumber() + second.getSizeNumber();
    BigInt mult(sizeNumber);
    BigInt firstSum(sizeNumber);

    long long current, excess;

    for (long long i = 0; i < second.getSizeNumber(); ++i) {
        if (second.getDigit(i) == 0) {
            continue;
        }
        excess = 0;
        for (long long k = 0; k < getSizeNumber(); ++k) {
            firstSum.digits[k+i] = getDigit(k) * second.getDigit(i) + excess;
            excess = firstSum.digits[k+i] / BASE;
            firstSum.digits[k+i] %= BASE;
            current = k+i;
        }
        if (excess != 0) {
            firstSum.digits[current+1] = excess;
        }

        mult = mult + firstSum; 

        for (long long i = 0; i < sizeNumber; ++i) {
            firstSum.digits[i] = 0;
        }

    }

    mult.removeZeroes();
    return mult;
}   */ 

BigInt BigInt::operator*(BigInt &second) {
    long long newNumSize = getSizeNumber() + second.getSizeNumber();
    BigInt mult(newNumSize);

    for (long long j = 0; j < second.getSizeNumber(); ++j) {
        if (second.digits[j] == 0) {
            continue;
        }

        long long excess = 0;
        for (long long i = 0; i < getSizeNumber(); ++i) {
            //сразу чтоб не производить сложение n сумм после перемножения на все разряды, клеим к одному числу все и учитываем смещен
            long long tmp = getDigit(i) * second.getDigit(j) + excess + mult.getDigit(i + j);
            mult.digits[i + j] = tmp % BASE;
            excess = tmp / BASE; 
        }
        if (excess > 0) {
            mult.digits[j + getSizeNumber()] = excess;
        }
    }

    mult.removeZeroes();
    return mult;
}

BigInt BigInt::operator/(BigInt &second)
{
    long long sizeNumber = getSizeNumber();
    BigInt div(sizeNumber);
    BigInt currentPath(0, true);

    for (long long i = getSizeNumber() - 1; i >= 0; --i) {
        currentPath.digits.insert(currentPath.digits.begin(), digits[i]);
        currentPath.removeZeroes();

        long long currentValue = 0, leftFlang = 0, rightFlang = BASE;
        while (leftFlang <= rightFlang) {
            long long middle = (rightFlang + leftFlang) / 2;
            BigInt tmp = BigInt(middle, true) * second;
            if (!(tmp > currentPath)) {
                currentValue = middle;
                leftFlang = middle + 1;
            } else {
                rightFlang = middle - 1;
            }
        }
        div.digits[i] = currentValue;
        BigInt tmp = BigInt(currentValue, true) * second;
        currentPath = currentPath - tmp;
       // cout << div.digits[i] <<  "  " << tmp << "  " << currentPath << endl;
    }
    div.removeZeroes();
    return div; 
}

BigInt BigInt::operator^(BigInt &second)
{
    BigInt pow(1, true);
    BigInt zero(0, true);
    BigInt one(1, true);
    BigInt two(2, true);

    while (second > zero) {
        if (second.getDigit(0) % 2 == 1) {
            pow = pow * (*this);
            second = second - one;
        } else {
            (*this) = (*this) * (*this);
            second = second / two;
        }
    }
    return pow;
}

bool BigInt::operator>(BigInt &second)
{
    if (getSizeNumber() > second.getSizeNumber()) {
        return true;
    } else if (getSizeNumber() == second.getSizeNumber()){
        for (long long i = getSizeNumber() - 1; i >= 0; --i) {
            if (digits[i] != second.getDigit(i)) {
                return digits[i] > second.getDigit(i);
            }
        }
    }
    
    return false;
}

bool BigInt::operator<(BigInt &second)
{
    if (getSizeNumber() < second.getSizeNumber()) {
        return true;
    } else if (getSizeNumber() == second.getSizeNumber()) {
        for (long long i = getSizeNumber() - 1; i >= 0; --i) { // все заменяю на getDigit() иначе ошибка обращения к памяти
            if (digits[i] != second.getDigit(i)) {
                return digits[i] < second.getDigit(i);
            }
        }
    } 
    return false;
}

bool BigInt::operator==(BigInt &second)
{
    if (getSizeNumber() == second.getSizeNumber()) {
        for (long long i = getSizeNumber() - 1; i >= 0; --i) {
            if (digits[i] != second.getDigit(i)) {
                return false;
            }
        }
    } else {
        return false;
    }

    return true;
}


ostream &operator<<(ostream &out, BigInt number) 
{
    if(number.getSizeNumber() == 0){
        out << 0;
    } else {
        out << number.getDigit(number.getSizeNumber() - 1);
        for(long long i = number.getSizeNumber() - 2; i >= 0; --i){
            out << setfill('0') <<  setw(DIGIT_LENGTH) << number.getDigit(i);
        }
        
    }
    return out;
}

void BigInt::removeZeroes()
{
    long long id = getSizeNumber() - 1;
    while(id > 0 && digits[id] == 0){
        digits.pop_back();
        --(id);
    }
}

long long BigInt::getSizeNumber()
{
    return digits.size();
}

long long BigInt::getDigit(long long id)
{
    if (id >= getSizeNumber()) {
        return 0;
    } 
    return digits[id];
}