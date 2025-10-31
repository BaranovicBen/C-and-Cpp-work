#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

struct Records {
  std::string date;
  std::string street;
  std::string city;

};

struct Person{
  std::string id;
  std::string name;
  std::string surname;

  std::vector<Records> adress;

  Person()=default;
  ~Person()=default;
};

struct CRegister
{
    std::map<std::string, Person> people; 
    CRegister() = default;
    CRegister(const CRegister& other){
      people = other.people;
    };
    ~CRegister() = default;

    CRegister& operator=(const CRegister& other) {
      if(this != &other){
        people = other.people;
      }
      return *this;
    };

    bool add(const char id[], const char name[], const char surname[], const char date[], const char street[], const char city[]) {
        std::string key(id);
        std::string meno(name);
        std::string priez(surname);
        std::string datum(date);
        std::string ulica(street);
        std::string mesto(city);
        
        if (people.find(key) != people.end()) {
            return false;
        }
    
        Person dano;
        dano.id = key;
        dano.name = meno;
        dano.surname = priez;
        Records dummyAdr = {datum, ulica, mesto};
    
        dano.adress.push_back(dummyAdr);
        people[key] = dano;
    
        return true;
    };
    

    bool resettle(const char id[], const char date[], const char street[], const char city[]) {
        std::string key(id);
        std::string datum(date);
        std::string ulica(street);
        std::string mesto(city);
        
        auto index = people.find(key);
        if (index == people.end()) {
            return false;
        }
    
        auto& rc = index->second;
    
        for (int i = 0; i < rc.adress.size(); i++) {
            if (rc.adress[i].date == datum) {
                return false;
            }
        }
    
        Records newAdr = {datum, ulica, mesto};
        rc.adress.push_back(newAdr);

        std::sort(rc.adress.begin(), rc.adress.end(), [](const Records& a, const Records& b) {
            return a.date < b.date;
        });

        return true;
    };
    

    bool print(std::ostream& os, const char id[]) const {

        std::string key(id);
    
        auto index = people.find(key);
        if (index == people.end()) {
            return false;
        }
    
        const auto& rc = index->second;
        os <<rc.id << " " << rc.name << " " << rc.surname;
    
        for (int i = 0; i < rc.adress.size(); i++) {
            os << "\n" << rc.adress[i].date << " " << rc.adress[i].street << " " << rc.adress[i].city;
        }

        os << std::endl;
        return true;
    }
    
};

int main ()
{
  char   lID[12], lDate[12], lName[50], lSurname[50], lStreet[50], lCity[50];
  std::ostringstream oss;
  CRegister  a;
  assert ( a . add ( "123456/7890", "John", "Smith", "2000-01-01", "Main street", "Seattle" ) == true );
  assert ( a . add ( "987654/3210", "Freddy", "Kruger", "2001-02-03", "Elm street", "Sacramento" ) == true );
  assert ( a . resettle ( "123456/7890", "2003-05-12", "Elm street", "Atlanta" ) == true );
  assert ( a . resettle ( "123456/7890", "2002-12-05", "Sunset boulevard", "Los Angeles" ) == true );
  oss . str ( "" );
  assert ( a . print ( oss, "123456/7890" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(123456/7890 John Smith
2000-01-01 Main street Seattle
2002-12-05 Sunset boulevard Los Angeles
2003-05-12 Elm street Atlanta
)###" ) );
  oss . str ( "" );
  assert ( a . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
)###" ) );
  CRegister b ( a );
  assert ( b . resettle ( "987654/3210", "2008-04-12", "Elm street", "Cinccinati" ) == true );
  assert ( a . resettle ( "987654/3210", "2007-02-11", "Elm street", "Indianapolis" ) == true );
  oss . str ( "" );
  assert ( a . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2007-02-11 Elm street Indianapolis
)###" ) );
  oss . str ( "" );
  assert ( b . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
)###" ) );
  a = b;
  assert ( a . resettle ( "987654/3210", "2011-05-05", "Elm street", "Salt Lake City" ) == true );
  oss . str ( "" );
  assert ( a . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
2011-05-05 Elm street Salt Lake City
)###" ) );
  oss . str ( "" );
  assert ( b . print ( oss, "987654/3210" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(987654/3210 Freddy Kruger
2001-02-03 Elm street Sacramento
2008-04-12 Elm street Cinccinati
)###" ) );
  assert ( b . add ( "987654/3210", "Joe", "Lee", "2010-03-17", "Abbey road", "London" ) == false );
  assert ( a . resettle ( "987654/3210", "2001-02-03", "Second street", "Milwaukee" ) == false );
  oss . str ( "" );
  assert ( a . print ( oss, "666666/6666" ) == false );

  CRegister  c;
  strncpy ( lID, "123456/7890", sizeof ( lID ) );
  strncpy ( lName, "John", sizeof ( lName ) );
  strncpy ( lSurname, "Smith", sizeof ( lSurname ) );
  strncpy ( lDate, "2000-01-01", sizeof ( lDate) );
  strncpy ( lStreet, "Main street", sizeof ( lStreet ) );
  strncpy ( lCity, "Seattle", sizeof ( lCity ) );
  assert (  c . add ( lID, lName, lSurname, lDate, lStreet, lCity ) == true );
  strncpy ( lID, "987654/3210", sizeof ( lID ) );
  strncpy ( lName, "Freddy", sizeof ( lName ) );
  strncpy ( lSurname, "Kruger", sizeof ( lSurname ) );
  strncpy ( lDate, "2001-02-03", sizeof ( lDate) );
  strncpy ( lStreet, "Elm street", sizeof ( lStreet ) );
  strncpy ( lCity, "Sacramento", sizeof ( lCity ) );
  assert (  c . add ( lID, lName, lSurname, lDate, lStreet, lCity ) == true );
  strncpy ( lID, "123456/7890", sizeof ( lID ) );
  strncpy ( lDate, "2003-05-12", sizeof ( lDate) );
  strncpy ( lStreet, "Elm street", sizeof ( lStreet ) );
  strncpy ( lCity, "Atlanta", sizeof ( lCity ) );
  assert ( c . resettle ( lID, lDate, lStreet, lCity ) == true );
  strncpy ( lID, "123456/7890", sizeof ( lID ) );
  strncpy ( lDate, "2002-12-05", sizeof ( lDate) );
  strncpy ( lStreet, "Sunset boulevard", sizeof ( lStreet ) );
  strncpy ( lCity, "Los Angeles", sizeof ( lCity ) );
  assert ( c . resettle ( lID, lDate, lStreet, lCity ) == true );
  oss . str ( "" );
  assert ( c . print ( oss, "123456/7890" ) == true );
  assert ( ! strcmp ( oss . str () . c_str (), R"###(123456/7890 John Smith
2000-01-01 Main street Seattle
2002-12-05 Sunset boulevard Los Angeles
2003-05-12 Elm street Atlanta
)###" ) );

  return EXIT_SUCCESS;
}