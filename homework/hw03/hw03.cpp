#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>

struct Records {
  char* date;
  char* street;
  char* city;
};

struct Entry {
  char* str;
};

struct myString {
  Entry* entries;
  size_t count;
  size_t cap;

  myString() : entries(nullptr), count(0), cap(0) {}

  ~myString() {
    for (size_t i = 0; i < count; ++i)
      delete[] entries[i].str;
    delete[] entries;
  }

  const char* intern(const char* src) {
    for (size_t i = 0; i < count; ++i) {
      if (strcmp(entries[i].str, src) == 0)
        return entries[i].str;
    }

    if (count >= cap) {
      cap = cap ? cap * 2 : 16;
      Entry* newArr = new Entry[cap];
      for (size_t i = 0; i < count; ++i)
        newArr[i] = entries[i];
      delete[] entries;
      entries = newArr;
    }

    size_t len = strlen(src);
    char* newStr = new char[len + 1];
    strcpy(newStr, src);
    entries[count].str = newStr;
    return entries[count++].str;
  }
};

char* duplicate(const char* src) {
  if (!src) return nullptr;
  size_t len = strlen(src);
  char* copy = new char[len + 1];
  strcpy(copy, src);
  return copy;
}

struct Person
{
  char*        id;
  const char*  name;
  const char*  surname;

  Records*     adress;
  size_t       addrCount;
  size_t       addrMax;

  Person() = default;

  Person(const Person& other)
  : id(duplicate(other.id)),
    name(other.name),
    surname(other.surname),
    addrCount(other.addrCount),
    addrMax(other.addrCount)
  {
    adress = new Records[addrMax];
    for (size_t i = 0; i < addrCount; ++i)
    {
      adress[i].date   = duplicate(other.adress[i].date);
      adress[i].street = other.adress[i].street;
      adress[i].city   = other.adress[i].city;
    }
  }

  Person& operator=(const Person& other)
  {
    if (this != &other)
    {
      clear();
      id           = duplicate(other.id);
      name         = other.name;
      surname      = other.surname;
      addrCount = other.addrCount;
      addrMax   = other.addrCount;

      adress = new Records[addrMax];
      for (size_t i = 0; i < addrCount; ++i)
      {
        adress[i].date   = duplicate(other.adress[i].date);
        adress[i].street = other.adress[i].street;
        adress[i].city   = other.adress[i].city;
      }
    }
    return *this;
  }

  void addAddress(const char* date, const char* street, const char* city, myString& pool)
  {
    if (addrCount >= addrMax)
    {
      addrMax = addrMax ? addrMax * 2 : 4;
      Records* newList = new Records[addrMax];
      for (size_t i = 0; i < addrCount; ++i)
        newList[i] = adress[i];
      delete[] adress;
      adress = newList;
    }

    adress[addrCount].date   = duplicate(date);
    adress[addrCount].street = const_cast<char*>(pool.intern(street));
    adress[addrCount].city   = const_cast<char*>(pool.intern(city));
    ++addrCount;
  }

  ~Person()
  {
    clear();
  }

  void clear()
  {
    delete[] id;
    for (size_t i = 0; i < addrCount; ++i)
      delete[] adress[i].date;
    delete[] adress;

    id           = nullptr;
    adress  = nullptr;
    addrCount = 0;
    addrMax   = 0;
  }
};

struct CRegister {
  Person** people;
  size_t pCount;
  size_t pMax;
  myString pool;

  CRegister() : people(nullptr), pCount(0), pMax(0) {}

  ~CRegister() {
    clear();
  }

  CRegister(const CRegister& other) {
    pCount = other.pCount;
    pMax = other.pMax;
    people = new Person*[pMax];
    for (size_t i = 0; i < pCount; ++i) {
      people[i] = new Person(*other.people[i]);
    }
  }

  CRegister& operator=(const CRegister& other) {
    if (this != &other) {
      clear();
      pCount = other.pCount;
      pMax = other.pMax;
      people = new Person*[pMax];
      for (size_t i = 0; i < pCount; ++i)
        people[i] = new Person(*other.people[i]);
    }
    return *this;
  }

  void clear() {
    for (size_t i = 0; i < pCount; ++i)
      delete people[i];
    delete[] people;
    people = nullptr;
    pCount = 0;
    pMax = 0;
  }

  int binarySearch(const char* id) const {
    int low = 0, high = static_cast<int>(pCount) - 1;
    while (low <= high) {
      int mid = (low + high) / 2;
      int cmp = strcmp(people[mid]->id, id);
      if (cmp == 0)
        return mid;
      if (cmp < 0)
        low = mid + 1;
      else
        high = mid - 1;
    }
    return -1;
  }

  void insrt(Person* p) {
    size_t i = pCount;
    while (i > 0 && strcmp(people[i - 1]->id, p->id) > 0) {
      people[i] = people[i - 1];
      --i;
    }
    people[i] = p;
    ++pCount;
  }

  bool add(const char* id, const char* name, const char* surname, const char* date, const char* street, const char* city) {
    if (binarySearch(id) != -1)
      return false;

    if (pCount >= pMax) {
      pMax = pMax ? pMax * 2 : 4;
      Person** newArr = new Person*[pMax];
      for (size_t i = 0; i < pCount; ++i)
        newArr[i] = people[i];
      delete[] people;
      people = newArr;
    }

    Person* p = new Person();
    p->id = duplicate(id);
    p->name = const_cast<char*>(pool.intern(name));
    p->surname = const_cast<char*>(pool.intern(surname));
    p->addAddress(date, street, city, pool);
    insrt(p);
    return true;
  }

  bool resettle(const char* id, const char* date, const char* street, const char* city) {
    int idx = binarySearch(id);
    if (idx == -1)
      return false;
  
    Person* p = people[idx];
  
    for (size_t j = 0; j < p->addrCount; ++j) {
      if (strcmp(p->adress[j].date, date) == 0)
        return false;
    }
  
    p->addAddress(date, street, city, pool);
  
    for (size_t m = 0; m < p->addrCount - 1; ++m) {
      for (size_t n = 0; n < p->addrCount - m - 1; ++n) {
        if (strcmp(p->adress[n].date, p->adress[n + 1].date) > 0) {
          Records tmp = p->adress[n];
          p->adress[n] = p->adress[n + 1];
          p->adress[n + 1] = tmp;
        }
      }
    }
  
    return true;
  }  

  bool print(std::ostream& os, const char* id) const {
    int idx = binarySearch(id);
    if (idx == -1)
      return false;
  
    const Person* p = people[idx];
    os << p->id << " " << p->name << " " << p->surname;
  
    for (size_t j = 0; j < p->addrCount; ++j) {
      os << "\n" << p->adress[j].date << " "
         << p->adress[j].street << " "
         << p->adress[j].city;
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