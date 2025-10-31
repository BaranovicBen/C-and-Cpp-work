#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <compare>
#include <functional>
#include <stdexcept>
#include <algorithm>
#endif /* __PROGTEST__ */

struct Citizen {
    std::string name, addr, account;
    int income=0,expenses=0;

    Citizen(const std::string &name,const std::string &addr,const std::string &account){
        this->name = name;
        this->addr = addr;
        this->account = account;
    }

    Citizen() = default;

    bool operator<(const Citizen &other) const {
        if(name != other.name){
            return name < other.name;
        }
        return addr < other.addr;
    }

    bool compareByAccount(const Citizen &other) const {
        return account < other.account;
    }
};

class CIterator {
    public:
        CIterator(const std::vector<Citizen> &reg) : index(0), registry(reg) {};

        bool atEnd() const {
            return index >= registry.size();
        }

        bool next(){
            if(!atEnd()){
                index++;
                return true;
            }
            return false;
        }

        const std::string &name() const {
            return registry[index].name;
        }

        const std::string &addr() const {
            return registry[index].addr;
        }

        const std::string &account() const {
            return registry[index].account;
        }

    private:
        size_t index;
        const std::vector<Citizen> &registry;
};

class CTaxRegister {
    public:
        CTaxRegister() = default;
        ~CTaxRegister() = default;

        bool birth(const std::string &name, const std::string &addr, const std::string &account) {
            auto it = namAddFinder(name, addr);
            if (citExists(it, name, addr)) return false; 

            auto accountIt = accFinder(account);
            if (accExists(accountIt)) return false;  

            registry.insert(it, Citizen(name, addr, account));
            return true;
        }

        bool death(const std::string &name, const std::string &addr) {
            auto it = namAddFinder(name, addr);
            if (!citExists(it, name, addr)) return false;  

            registry.erase(it);
            return true;
        }

        bool income(const std::string &account, int amount) {
            auto it = accFinder(account);
            if (!accExists(it)) return false;  

            it->income += amount;
            return true;
        }

        bool income(const std::string &name, const std::string &addr, int amount) {
            auto it = namAddFinder(name, addr);
            if (!citExists(it, name, addr)) return false;  

            it->income += amount;
            return true;
        }

        bool expense(const std::string &account, int amount) {
            auto it = accFinder(account);
            if (!accExists(it)) return false;  

            it->expenses += amount;
            return true;
        }

        bool expense(const std::string &name, const std::string &addr, int amount) {
            auto it = namAddFinder(name, addr);
            if (!citExists(it, name, addr)) return false;  

            it->expenses += amount;
            return true;
        }

        bool audit(const std::string &name, const std::string &addr, std::string &account, int &sumIncome, int &sumExpense) {
            auto it = namAddFinder(name, addr);
            if (!citExists(it, name, addr)) return false;  

            account = it->account;
            sumIncome = it->income;
            sumExpense = it->expenses;
            return true;
        }

        CIterator listByName() const {
            return CIterator(registry);
        }

    private:
        std::vector<Citizen> registry;

        std::vector<Citizen>::iterator namAddFinder(const std::string &name, const std::string &addr) {
            return std::lower_bound(registry.begin(), registry.end(), Citizen{name, addr, ""},
                [](const Citizen &a, const Citizen &b) {
                    return (a.name < b.name) || (a.name == b.name && a.addr < b.addr);
                });
        }

        std::vector<Citizen>::iterator accFinder(const std::string &account) {
            return std::find_if(registry.begin(), registry.end(), [&](const Citizen &other) {
                return other.account == account;
            });
        }

        bool citExists(std::vector<Citizen>::iterator it, const std::string &name, const std::string &addr) {
            return it != registry.end() && it->name == name && it->addr == addr;
        }

        bool accExists(std::vector<Citizen>::iterator it) {
            return it != registry.end();
        }
};



#ifndef __PROGTEST__
int main ()
{
  std::string acct;
  int    sumIncome, sumExpense;
  CTaxRegister b0;
  assert ( b0 . birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
  assert ( b0 . birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
  assert ( b0 . birth ( "Peter Hacker", "Main Street 17", "634oddT" ) );
  assert ( b0 . birth ( "John Smith", "Main Street 17", "Z343rwZ" ) );
  assert ( b0 . income ( "Xuj5#94", 1000 ) );
  assert ( b0 . income ( "634oddT", 2000 ) );
  assert ( b0 . income ( "123/456/789", 3000 ) );
  assert ( b0 . income ( "634oddT", 4000 ) );
  assert ( b0 . income ( "Peter Hacker", "Main Street 17", 2000 ) );
  assert ( b0 . expense ( "Jane Hacker", "Main Street 17", 2000 ) );
  assert ( b0 . expense ( "John Smith", "Main Street 17", 500 ) );
  assert ( b0 . expense ( "Jane Hacker", "Main Street 17", 1000 ) );
  assert ( b0 . expense ( "Xuj5#94", 1300 ) );
  assert ( b0 . audit ( "John Smith", "Oak Road 23", acct, sumIncome, sumExpense ) );
  assert ( acct == "123/456/789" );
  assert ( sumIncome == 3000 );
  assert ( sumExpense == 0 );
  assert ( b0 . audit ( "Jane Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "Xuj5#94" );
  assert ( sumIncome == 1000 );
  assert ( sumExpense == 4300 );
  assert ( b0 . audit ( "Peter Hacker", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "634oddT" );
  assert ( sumIncome == 8000 );
  assert ( sumExpense == 0 );
  assert ( b0 . audit ( "John Smith", "Main Street 17", acct, sumIncome, sumExpense ) );
  assert ( acct == "Z343rwZ" );
  assert ( sumIncome == 0 );
  assert ( sumExpense == 500 );
  CIterator it = b0 . listByName ();
  assert ( ! it . atEnd ()
           && it . name () == "Jane Hacker"
           && it . addr () == "Main Street 17"
           && it . account () == "Xuj5#94" );
  it . next ();
  assert ( ! it . atEnd ()
           && it . name () == "John Smith"
           && it . addr () == "Main Street 17"
           && it . account () == "Z343rwZ" );
  it . next ();
  assert ( ! it . atEnd ()
           && it . name () == "John Smith"
           && it . addr () == "Oak Road 23"
           && it . account () == "123/456/789" );
  it . next ();
  assert ( ! it . atEnd ()
           && it . name () == "Peter Hacker"
           && it . addr () == "Main Street 17"
           && it . account () == "634oddT" );
  it . next ();
  assert ( it . atEnd () );

  assert ( b0 . death ( "John Smith", "Main Street 17" ) );

  CTaxRegister b1;
  assert ( b1 . birth ( "John Smith", "Oak Road 23", "123/456/789" ) );
  assert ( b1 . birth ( "Jane Hacker", "Main Street 17", "Xuj5#94" ) );
  assert ( !b1 . income ( "634oddT", 4000 ) );
  assert ( !b1 . expense ( "John Smith", "Main Street 18", 500 ) );
  assert ( !b1 . audit ( "John Nowak", "Second Street 23", acct, sumIncome, sumExpense ) );
  assert ( !b1 . death ( "Peter Nowak", "5-th Avenue" ) );
  assert ( !b1 . birth ( "Jane Hacker", "Main Street 17", "4et689A" ) );
  assert ( !b1 . birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
  assert ( b1 . death ( "Jane Hacker", "Main Street 17" ) );
  assert ( b1 . birth ( "Joe Hacker", "Elm Street 23", "Xuj5#94" ) );
  assert ( b1 . audit ( "Joe Hacker", "Elm Street 23", acct, sumIncome, sumExpense ) );
  assert ( acct == "Xuj5#94" );
  assert ( sumIncome == 0 );
  assert ( sumExpense == 0 );
  assert ( !b1 . birth ( "Joe Hacker", "Elm Street 23", "AAj5#94" ) );


  std::cout << "SUCCESS!" << std::endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */