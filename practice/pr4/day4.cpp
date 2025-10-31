#include <iostream>

int main(){

    std::string name = "Ben";
    int age = 21;
    std::string freeFood[5] = {"Pizza","HotDog","Burger","Cola","Curry"};

    std::string *pName = &name;
    int *pAge = &age;
    std::string *pFreeFood = freeFood;

    std::cout <<"Value "<< *pName << " is stored at "<< pName << '\n';
    std::cout <<"Value "<< *pAge << " is stored at "<< pAge << '\n';

    for(int i = 0; i < sizeof(freeFood)/sizeof(std::string); i++){
        std::cout <<"Value "<< *(pFreeFood + i)<< " is stored at "<< (pFreeFood+i )<< '\n';
    }


    return 0;
}

/*
#include <iostream>

template<typename T, typename U>

auto max(T x,U y){
    return (x > y) ? x : y;
}

int main(){

    std::cout << max(3,4) <<'\n';
    std::cout << max(4.3,4.2) <<'\n';
    std::cout << max('a','b') <<'\n';

    std::cout << max(3,1.88) <<'\n';

    return 0;
}
*/

//adress of operator & neposiela do funckie kopiu ale to same value 
//const znemozni zmenu values

//4933 0197 9382 1988 
//4 3 0 9 9 8 1 8 (index % 2 == 0)
//8 6 0 1 8 1 8 1 6 1 2 6
// 9 3 1 7 3 2 9 8 (index % 2 != 0)