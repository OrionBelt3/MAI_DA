#include <iostream>
#include <string>
#include <cstring>

// сигнатура: строка, кэш, левый фланг, правый фланг
long countingVariants(std::string &str,long cache[100][100], int i, int j){
    //если i и j поменялись местами, то вернем 0
    if(j < i){
        return 0;
    } else {
        //это условие проверит наличие в кэше нашего значения
        if(cache[i][j] == 0){
            if(str[i] == str[j]){
                // варианты: если вычеркнуть слева, если  - справа
                cache[i][j] = countingVariants(str,cache,i + 1,j) + countingVariants(str,cache,i,j - 1) + 1;
            } else {
                // варианты: если вычеркнуть слева, если  - справа, отнимаем варики, если черканем слева и справа, т.к. они неравны
                cache[i][j] = countingVariants(str,cache,i + 1,j) + countingVariants(str,cache,i,j - 1) - countingVariants(str,cache,i + 1,j - 1);
            }
        }
        return cache[i][j];
    }
}

int main(){
    // матрица кэша
    long cache[100][100];
    std::memset(cache,0,sizeof(long)*10000);
    std::string str;// входная строка
    std::cin >> str;
    std::cout << countingVariants(str,cache,0,str.size() - 1) << std::endl;
    return 0;
}