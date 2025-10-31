#include <iostream>
#include <tuple>
#include <string>

struct Citizen{
    std::string name;
    std::string address;
    std::string account;
    Citizen(std::string name, std::string address, std::string account){
        std::cout << "Citizen created" << std::endl;
        this->name = name;
        this->address = address;
        this->account = account;
    }

    bool operator<(const Citizen &other) const {
        if (name != other.name) return name < other.name;
        if (address != other.address) return address < other.address;
        return account < other.account;
    }
};

int main() {
    Citizen c1("Alice Smith", "Main Street", "123456");
    Citizen c2("Alice Smith", "Main Street", "987654");  // Same everything except account
    Citizen c3("Alice Smith", "Second Avenue", "234567"); // Different address
    Citizen c4("Bob Brown", "Elm Street", "555555"); // Different name

    std::cout << std::boolalpha;
    std::cout << "(c1 < c2): " << (c1 < c2) << std::endl;  // true (account number decides)
    std::cout << "(c1 < c3): " << (c1 < c3) << std::endl;  // true (address comparison)
    std::cout << "(c2 < c3): " << (c1 < c4) << std::endl;  // true (name comparison)
}

/*
int main(){
    std::string name;
    std::string surname;
    std::string address;
    int bankAccount;
    int adressNumber;

    std::cout << "Enter your name: ";
    std::cin >> name;
    std::cin >> surname;
    std::cout << "Enter your address: ";
    std::cin >> address;
    std::cin >> adressNumber;
    std::cout << "Enter your bank account: ";
    std::cin >> bankAccount;

    std::cout << "Your name is: " << name << " " << surname << ", Your address is: " << address << " " << adressNumber << ", Your bank account number is: " << bankAccount << std::endl;
}*/