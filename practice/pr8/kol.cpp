#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;
 
struct Cbase {
    Cbase(string sector, string species, int when) : m_Sector(std::move(sector)), m_Species(std::move(species)), m_When(when) {}
    ~Cbase() = default;
 
    string m_Sector;
    string m_Species;
    int m_When;
};
 
#endif /* __PROGTEST__ */
 
struct CUniverse {
 
 
    CUniverse & addConnection(const string & from, const string & to) {
 
    }
 
    CUniverse & optimize() {
        return *this;
    }
 
    map<string, string> colonise(const vector<Cbase> & bases) const {
 
    } 
};
 
 
#ifndef __PROGTEST__
 
int main ( void ) {
    CUniverse u1;
 
    u1 .addConnection("Earth", "X1")
       .addConnection("Earth", "Y1")
       .addConnection("Vulcan", "X1")
       .addConnection("Vulcan", "Y2")
       .addConnection("Y2", "Y1")
       .addConnection("Kronos", "X1")
       .addConnection("X1", "X2")
       .addConnection("X2", "X3")
       .optimize();
 
    auto r1 = u1.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 0}});
    assert(r1 == (map<string, string> ({{"Earth", "Humans"}, {"Y1", "Humans"}, {"Vulcan", "Vulcans"}, {"Y2", "Vulcans"}, {"Kronos", "Clingons"}})));
    auto r2 = u1.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Humans", 0}});
    assert(r2 == (map<string, string> ({{"Earth", "Humans"}, {"Y1", "Humans"}, {"Vulcan", "Vulcans"}, {"Y2", "Vulcans"}, {"Kronos", "Humans"}})));
    auto r3 = u1.colonise({{"Unknown", "Unknown", 0}});
    assert(r3 == (map<string, string> ({{"Unknown", "Unknown"}})));
    auto r4 = u1.colonise({});
    assert(r4 == (map<string, string>({})));
 
 
    CUniverse u2;
 
    u2 .addConnection("Earth", "Z1")
       .addConnection("Earth", "Y1")
       .addConnection("Earth", "Kronos")
       .addConnection("Earth", "Vulcan")
       .addConnection("Vulcan", "Y3")
       .addConnection("Vulcan", "X1")
       .addConnection("Kronos", "Z2")
       .addConnection("Kronos", "X4")
       .addConnection("Kronos", "Vulcan")
       .addConnection("Y1", "Y2")
       .addConnection("Y2", "Y3")
       .addConnection("Z1", "Z2")
       .addConnection("X1", "X2")
       .addConnection("X2", "X3")
       .addConnection("X1", "X3")
       .addConnection("X3", "X4")
       .addConnection("Bajor", "Cassiopea Prime")
       .optimize();
 
    auto r5 = u2.colonise({{"Earth", "Humans", 0}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 0}, {"Cassiopea Prime", "Cassiopeans", 0}});
    assert(r5 == (map<string, string> ({{"Earth", "Humans"}, {"Kronos", "Clingons"}, {"Vulcan", "Vulcans"}, {"Cassiopea Prime", "Cassiopeans"}, {"Bajor", "Cassiopeans"}, {"Z1", "Humans"}, {"Z2", "Clingons"}, {"Y1", "Humans"}, {"Y3", "Vulcans"}, {"X1", "Vulcans"}, {"X2", "Vulcans"}, {"X4", "Clingons"}})));
 
    //Harder tests when != 0 for all colonies
    CUniverse u3;
 
    u3 .addConnection("Earth", "Z1")
            .addConnection("Earth", "Y1")
            .addConnection("Earth", "Kronos")
            .addConnection("Earth", "Vulcan")
            .addConnection("Vulcan", "Y3")
            .addConnection("Vulcan", "X1")
            .addConnection("Kronos", "Z2")
            .addConnection("Kronos", "X4")
            .addConnection("Y1", "Y2")
            .addConnection("Y2", "Y3")
            .addConnection("Z1", "Z2")
            .addConnection("X1", "X2")
            .addConnection("X1", "X3")
            .addConnection("X2", "X3")
            .addConnection("X3", "X4")
            .addConnection("Bajor", "Cassiopea Prime")
            .optimize();
 
    auto r8 = u3.colonise({{"Earth", "Humans", 1}, {"Vulcan", "Vulcans", 0}, {"Kronos", "Clingons", 2}, {"Cassiopea Prime", "Cassiopeans", 10000}});
    assert(r8 == (map<string, string> ({{"Earth", "Humans"}, {"Kronos", "Clingons"}, {"Vulcan", "Vulcans"}, {"Y1", "Humans"}, {"Z1", "Humans"}, {"Y3", "Vulcans"}, {"Y2", "Vulcans"}, {"X1", "Vulcans"}, {"X2", "Vulcans"}, {"X3", "Vulcans"}, {"Cassiopea Prime", "Cassiopeans"}, {"Bajor", "Cassiopeans"}})));
 
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */