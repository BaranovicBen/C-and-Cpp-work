#include <iostream>

template<typename T, typename U>
auto max(T x,U y){
    return (x > y) ? x : y;
}

int main(){

    std::cout << max(3,4) <<'\n';
    std::cout << max(4.3,4.2) <<'\n';
    std::cout << max('a','b') <<'\n';

    std::cout << max(3.2,1) <<'\n';

    return 0;
}