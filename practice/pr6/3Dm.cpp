#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */
 
template <typename T_>
struct CMatrix3 {
    std::vector<T_> data;
    size_t d1,d2,d3;
    struct Proxy2D {
        CMatrix3 & parent;
        size_t i, j;

        Proxy2D(CMatrix3 & parent, size_t i, size_t j)
            : parent(parent), i(i), j(j) {}

        T_ & operator[](size_t k) {
            parent.checkBounds(i, j, k);
            return parent.data[parent.index(i, j, k)];
        }
    };

    struct Proxy1D {
        CMatrix3 & parent;
        size_t i;

        Proxy1D(CMatrix3 & parent, size_t i)
            : parent(parent), i(i) {}

        Proxy2D operator[](size_t j) {
            if (j >= parent.d2)
                throw std::out_of_range("Missing exception for access");
            return Proxy2D(parent, i, j);
        }
    };

    size_t index(size_t i,size_t j,size_t k){
        return i * d2 * d3 + j * d3 + k;
    }

    CMatrix3() = default;
    ~CMatrix3() = default;
 
    CMatrix3(unsigned n1, unsigned n2, unsigned n3 ){
        this->d1 = n1;
        this->d2 = n2;
        this->d3 = n3;
        data.resize(n1 * n2 * n3);
        std::cerr <<"Konstruktor here \n";
    }


    CMatrix3 & operator=(const CMatrix3 & other) {
        if (this != &other) {
            d1 = other.d1;
            d2 = other.d2;
            d3 = other.d3;
            data = other.data;
        }
        std::cerr <<"DeepCopy here \n";


        return *this;
    }


    CMatrix3 operator+(const CMatrix3& other){
        CMatrix3 tmp(*this);
        for(size_t i = 0; i < data.size();i++){
            tmp[i] = data[i] + other.data[i];
        }
        return tmp;
    };

    bool checkBounds(size_t i, size_t j, size_t k){
                std::cerr <<"CheckBounds here \n";
        if(i >= d1 || j >= d2 || k >= d3 ){
            throw std::out_of_range("Missing exception for access");
        }
        return true;
    }
        
    T_& operator()(size_t i, size_t j, size_t k) {
        checkBounds(i, j, k);
        return data[index(i, j, k)];
    }

    Proxy1D operator[](size_t i) {
        if (i >= d1)
            throw std::out_of_range("Missing exception for access");
        return Proxy1D(*this, i);
    }

    CMatrix3 &operator+(){

    };    

    bool operator==(const CMatrix3& other){
                std::cerr <<"Rovnaju sa here \n";
        if(d1 != other.d1 ||d2 != other.d2 ||d3 != other.d3)
            return false;

        for(size_t i = 0; i < data.size();i++){
            if(!(data[i]==other.data[i])){
                return false;
            }
        }

        return true;
    };
};
 
#ifndef __PROGTEST__
int main ( void ){
    CMatrix3<int> m ( 3, 4, 5 );
    m[0][0][0] = 128;
    m[2][3][1] = 7;
    assert ( m[0][0][0] == 128 );
    assert ( m[2][3][1] == 7 );
 
    m[1][1][1] = m[0][0][0] + m[2][3][1];
    m[2][3][4] = 135;
    assert ( m[1][1][1] == m[2][3][4] );
 
    CMatrix3<int> copy ( m );
    try { copy[4][1][2]; assert ( "Missing exception for access i0 = 4, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[1][4][2]; assert ( "Missing exception for access i1 = 4, limit = 3" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[2][1][6]; assert ( "Missing exception for access i2 = 6, limit = 4" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
 
    copy[2][3][1] = 42;
    assert ( m[2][3][1] == 7 );
    assert ( copy[2][3][1] == 42 );
 
    CMatrix3<int> n ( 2, 2, 2 );
    n[0][1][0] = 56;
    n[1][0][1] = 42;
    m = n;
    n[0][1][0] = 0;
    assert ( m[0][1][0] == 56 );
    try { m[2][1][0]; assert ( "Missing exception for access i0 = 2, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
 
    CMatrix3<string> s ( 2, 1, 1 );
    s[0][0][0] = "Hello ";
    s[1][0][0] = "World!";
    assert ( ( s[0][0][0] + s[1][0][0] ) == string("Hello World!") );
 
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */