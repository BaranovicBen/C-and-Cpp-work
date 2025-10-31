#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
#endif /* __PROGTEST__ */
 
struct CStarBaseException {
    std::string m_Name;

    CStarBaseException(const std::string& name) : m_Name(name) {}

    friend std::ostream & operator << (std::ostream & os, const CStarBaseException & ex) {
        return os << "Unknown star base '" << ex.m_Name << "'\n";
    }
};
 
struct TStarBase{
    TStarBase     * m_Next;
    string          m_Name;

    TStarBase  ( const string & name )
    : m_Next ( NULL ), m_Name ( name )
    { }
};

struct CTripResult {
    std::vector<TStarBase*> trip;

    CTripResult(std::vector<TStarBase*> t) : trip(std::move(t)) {}

    unsigned Bases() const {
        return trip.size();
    }

    friend std::ostream& operator<<(std::ostream& os, const CTripResult& res) {
        for (size_t i = 0; i < res.trip.size(); ++i) {
            os << res.trip[i]->m_Name;
            if (i + 1 < res.trip.size())
                os << "\n -> ";
        }
        os << '\n';

        return os;
    }

};

struct CStarLiner {
    TStarBase   * m_StarBases;

    CStarLiner() 
        : m_StarBases(nullptr)
        {};
    ~CStarLiner(){
        if (!m_StarBases) return;

        TStarBase* curr = m_StarBases->m_Next;
        while (curr != m_StarBases) {
            TStarBase* next = curr->m_Next;
            delete curr;
            curr = next;
        }
        delete m_StarBases;
        m_StarBases = nullptr;
    }

   CStarLiner& operator=(const CStarLiner&) = delete;

    CStarLiner(const CStarLiner& other)
        :m_StarBases(nullptr) {
            if(!other.m_StarBases)
                return;
            
                m_StarBases = new TStarBase(other.m_StarBases->m_Name);
                TStarBase* tail = m_StarBases;

                TStarBase* curr = other.m_StarBases->m_Next;
                while(curr != other.m_StarBases){
                    tail->m_Next = new TStarBase(curr->m_Name);
                    tail = tail->m_Next;
                    curr = curr->m_Next;
                }
                
            tail->m_Next = m_StarBases;
        };
    
    void Add ( istream & is ){
        std::string tmp;
        while(std::getline(is,tmp)){
            if(tmp.empty()) continue;

            TStarBase* newNode = new TStarBase(tmp);

            if (!m_StarBases) {
                m_StarBases = newNode;
                newNode->m_Next = m_StarBases;
            } else {
                TStarBase* last = m_StarBases;
                while (last->m_Next != m_StarBases) {
                    last = last->m_Next;
                }

                last->m_Next = newNode;
                newNode->m_Next = m_StarBases;
            }

        }
    }; 
    
    CTripResult Trip(const string & from, const string & to) const {
        return CTripResult(findTrip(from, to));
    }

    std::vector<TStarBase*> findTrip(const string & from, const string & to) const {
        std::vector<TStarBase*> result;

        if (!m_StarBases)
            throw CStarBaseException(from); 

        TStarBase * start = nullptr;
        TStarBase * cur = m_StarBases;

        do {
            if (cur->m_Name == from) {
                start = cur;
                break;
            }
            cur = cur->m_Next;
        } while (cur != m_StarBases);

        if (!start)
            throw CStarBaseException(from);

        cur = start;

        if (from == to) {
            result.push_back(start);
            return result;
        }

        do {
            result.push_back(cur);
            if (cur->m_Name == to)
                return result;

            cur = cur->m_Next;
        } while (cur != start);

        throw CStarBaseException(to);
    }

};

int main() {   
istringstream is;  
int l;
CStarLiner a;
is . clear ();
is . str ( "Earth Spacedock\nFarpoint Station\nCorinth IV\nLya III\nDeep Space 9\n" );
a . Add ( is );
l = a . Trip ( "Farpoint Station", "Lya III" ) . Bases (); // l = 3
cout << a . Trip ( "Farpoint Station", "Lya III" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Farpoint Station
 -> Corinth IV
 -> Lya III
--8<----8<----8<----8<----8<----8<----8<--
*/
 
l = a . Trip ( "Corinth IV", "Farpoint Station" ) . Bases (); // l = 5
cout << a . Trip ( "Corinth IV", "Farpoint Station" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Corinth IV
 -> Lya III
 -> Deep Space 9
 -> Earth Spacedock
 -> Farpoint Station
--8<----8<----8<----8<----8<----8<----8<--
*/
 
l = a . Trip ( "Lya III", "Lya III" ) . Bases (); // l = 1
cout << a . Trip ( "Lya III", "Lya III" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Lya III
--8<----8<----8<----8<----8<----8<----8<--
*/
 
is . clear ();
is . str ( "Starbase Earhart\nEmpok Nor\n" );
a . Add ( is );
l = a . Trip ( "Corinth IV", "Earth Spacedock" ) . Bases (); // l = 6
cout << a . Trip ( "Corinth IV", "Earth Spacedock" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Corinth IV
 -> Lya III
 -> Deep Space 9
 -> Starbase Earhart
 -> Empok Nor
 -> Earth Spacedock
--8<----8<----8<----8<----8<----8<----8<--
*/
 
try { cout << a . Trip ( "Berengaria VII", "Earth Spacedock" ); }
catch ( const CStarBaseException & e ) { cout << e; }
/*
--8<----8<----8<----8<----8<----8<----8<--
Unknown star base 'Berengaria VII'
--8<----8<----8<----8<----8<----8<----8<--
*/
 
CStarLiner b;
is . clear ();
is . str ( "Earth Spacedock\nFarpoint Station\n\n\nCorinth IV\nLya III\nStarbase 375\n" );
b . Add ( is );
l = b . Trip ( "Farpoint Station", "Lya III" ) . Bases (); // l = 3
cout << b . Trip ( "Farpoint Station", "Lya III" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Farpoint Station
 -> Corinth IV
 -> Lya III
--8<----8<----8<----8<----8<----8<----8<--
*/
 
is . clear ();
is . str ( "Empok Nor\nEmpok Nor\nTerok Nor" );
b . Add ( is );
l = b . Trip ( "Corinth IV", "Terok Nor" ) . Bases (); // l = 6
cout << b . Trip ( "Corinth IV", "Terok Nor" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Corinth IV
 -> Lya III
 -> Starbase 375
 -> Empok Nor
 -> Empok Nor
 -> Terok Nor
--8<----8<----8<----8<----8<----8<----8<--
*/
 
l = b . Trip ( "Farpoint Station", "Earth Spacedock" ) . Bases (); // l = 8
cout << b . Trip ( "Farpoint Station", "Earth Spacedock" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Farpoint Station
 -> Corinth IV
 -> Lya III
 -> Starbase 375
 -> Empok Nor
 -> Empok Nor
 -> Terok Nor
 -> Earth Spacedock
--8<----8<----8<----8<----8<----8<----8<--
*/
 
CStarLiner c;
is . clear ();
is . str ( "Earth Spacedock\nFarpoint Station\n\n\nCorinth IV\nLya III\nStarbase 375\n" );
c . Add ( is );
CStarLiner d = c;
is . clear ();
is . str ( "Starbase Earhart\nStarbase Montgomery\nTerok Nor\nEmpok Nor\n" );
c . Add ( is );
is . clear ();
is . str ( "Starbase Allen\nBerengaria VII\nEmpok Nor\n" );
d . Add ( is );
l = c . Trip ( "Farpoint Station", "Empok Nor" ) . Bases (); // l = 8
cout << c . Trip ( "Farpoint Station", "Empok Nor" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Farpoint Station
 -> Corinth IV
 -> Lya III
 -> Starbase 375
 -> Starbase Earhart
 -> Starbase Montgomery
 -> Terok Nor
 -> Empok Nor
--8<----8<----8<----8<----8<----8<----8<--
*/
 
l = d . Trip ( "Farpoint Station", "Empok Nor" ) . Bases (); // l = 7
cout << d . Trip ( "Farpoint Station", "Empok Nor" );
/*
--8<----8<----8<----8<----8<----8<----8<--
Farpoint Station
 -> Corinth IV
 -> Lya III
 -> Starbase 375
 -> Starbase Allen
 -> Berengaria VII
 -> Empok Nor
--8<----8<----8<----8<----8<----8<----8<--
*/
}