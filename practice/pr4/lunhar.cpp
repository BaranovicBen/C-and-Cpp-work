#include <iostream>
#include <algorithm>
#include <string>

int lunharm(const std::string cardNum);

void removeSpace(std::string &dirtyNum);
int main(){
    std::string cardNum;
    std::string tmp;

    int result;

    std::cout << "Enter a credit card #: ";
    std::getline(std::cin,tmp);

    removeSpace(tmp);

    cardNum = tmp;

    result = lunharm(cardNum);

    if(result % 10 == 0){
        std::cout << "Valid Card" << '\n';
    } else {
        std::cout << "Fake Card" << '\n';
    }

    return 0;
}

void removeSpace(std::string &dirtyNum){
    dirtyNum.erase(std::remove(dirtyNum.begin(), dirtyNum.end(), ' '), dirtyNum.end());
};

int lunharm(const std::string cardNum){
    int sum = 0;
    bool doubleIT = false;

    for(int i = cardNum.length()-1; i >= 0; i--){
        int digit = cardNum[i] - '0';

        if(doubleIT){
            digit*=2;
            if(digit > 9)digit -=9;
        }

        sum += digit;
        doubleIT = !doubleIT;
    }

    return sum;
};


//adress of operator & neposiela do funckie kopiu ale to same value 
//const znemozni zmenu values

//4933 0197 9382 1988 
//4 3 0 9 9 8 1 8 (index % 2 == 0)
//8 6 0 1 8 1 8 1 6 1 2 6
// 9 3 1 7 3 2 9 8 (index % 2 != 0)