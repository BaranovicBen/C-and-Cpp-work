#ifndef __PROGTEST__
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <array>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <memory>
#include <iterator>
#include <functional>
#include <utility>
#include <stdexcept>
#include <optional>

using namespace std::literals;
class CDumbString
{
  public:
    CDumbString             ( std::string v )
      : m_Data ( std::move ( v ) )
    {
    }
    bool        operator == ( const CDumbString & rhs ) const = default;
  private:
    std::string m_Data;
};

#endif /* __PROGTEST__ */

template<typename T, typename = void>
struct HasLess : std::false_type {};

template<typename T>
struct HasLess<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>> : std::true_type {};

template<typename T_>
struct CNet {
    CNet() = default;

    std::vector<T_> node;
    std::vector<std::vector<std::pair<size_t, int>>> conn;
    std::vector<size_t> par;
    std::vector<size_t> dep;
    std::vector<int> sum;

    typename std::conditional<HasLess<T_>::value, std::map<T_, size_t>, void>::type idxMap;

    size_t getIdx(const T_& val) {
        if constexpr (HasLess<T_>::value) {
            auto it = idxMap.find(val);
            if (it != idxMap.end()) {
                return it->second;
            }
            size_t idx = node.size();
            node.push_back(val);
            conn.emplace_back();
            idxMap[val] = idx;
            return idx;
        } else {
            for (size_t idx = 0; idx < node.size(); ++idx) {
                if (node[idx] == val) {
                    return idx;
                }
            }
            node.push_back(val);
            conn.emplace_back();
            return node.size() - 1;
        }
    }

    size_t idxOf(const T_& val) const {
        if constexpr (HasLess<T_>::value) {
            auto it = idxMap.find(val);
            if (it != idxMap.end()) {
                return it->second;
            }
            return static_cast<size_t>(-1);
        } else {
            for (size_t idx = 0; idx < node.size(); ++idx) {
                if (node[idx] == val) {
                    return idx;
                }
            }
            return static_cast<size_t>(-1);
        }
    }

    void dfsPrep(size_t cur, size_t parent, size_t depth, int cost) {
        par[cur] = parent;
        dep[cur] = depth;
        sum[cur] = cost;

        for (size_t i = 0; i < conn[cur].size(); ++i) {
            size_t nbr = conn[cur][i].first;
            int fee = conn[cur][i].second;
            if (nbr != parent) {
                dfsPrep(nbr, cur, depth + 1, cost + fee);
            }
        }
    }

    size_t getLca(size_t a, size_t b) const {
        while (a != b) {
            if (dep[a] > dep[b]) {
                a = par[a];
            } else {
                b = par[b];
            }
        }
        return a;
    }

    CNet& add(const T_& a, const T_& b, int fee) {
        size_t ida = getIdx(a);
        size_t idb = getIdx(b);
        conn[ida].emplace_back(idb, fee);
        conn[idb].emplace_back(ida, fee);
        return *this;
    }

    CNet& optimize() {
        size_t n = node.size();
        par.assign(n, 0);
        dep.assign(n, 0);
        sum.assign(n, 0);

        if (n > 0) {
            dfsPrep(0, 0, 0, 0);
        }
        return *this;
    }

    int totalCost(const T_& a, const T_& b) const {
        size_t ida = idxOf(a);
        size_t idb = idxOf(b);

        if (ida == static_cast<size_t>(-1) || idb == static_cast<size_t>(-1)) {
            return -1;
        }

        size_t lca = getLca(ida, idb);
        return sum[ida] + sum[idb] - 2 * sum[lca];
    }
};

#ifndef __PROGTEST__
int main ()
{
  CNet<std::string> a;
  a . add ( "Adam", "Bob", 100 )
    . add ( "Bob", "Carol", 200 )
    . add ( "Dave", "Adam", 300 )
    . add ( "Eve", "Fiona", 120 )
    . add ( "Kate", "Larry", 270 )
    . add ( "Ivan", "John", 70 )
    . add ( "Kate", "Ivan", 300 )
    . add ( "George", "Henry", 10 )
    . add ( "Eve", "George", 42 )
    . add ( "Adam", "Eve", 75 )
    . add ( "Ivan", "George", 38 )
    . optimize ();
  assert ( a . totalCost ( "Adam", "Bob" ) == 100 );
  assert ( a . totalCost ( "John", "Eve" ) == 150 );
  assert ( a . totalCost ( "Dave", "Henry" ) == 427 );
  assert ( a . totalCost ( "Carol", "Larry" ) == 1025 );
  assert ( a . totalCost ( "George", "George" ) == 0 );
  assert ( a . totalCost ( "Alice", "Bob" ) == -1 );
  assert ( a . totalCost ( "Thomas", "Thomas" ) == -1 );

  CNet<int> b;
  b . add ( 0, 1, 100 )
    . add ( 1, 2, 200 )
    . add ( 3, 0, 300 )
    . add ( 4, 5, 120 )
    . add ( 10, 11, 270 )
    . add ( 8, 9, 70 )
    . add ( 10, 8, 300 )
    . add ( 6, 7, 10 )
    . add ( 4, 6, 42 )
    . add ( 0, 4, 75 )
    . add ( 8, 6, 38 )
    . optimize ();
  assert ( b . totalCost ( 0, 1 ) == 100 );
  assert ( b . totalCost ( 9, 4 ) == 150 );
  assert ( b . totalCost ( 3, 7 ) == 427 );
  assert ( b . totalCost ( 2, 11 ) == 1025 );
  assert ( b . totalCost ( 6, 6 ) == 0 );
  assert ( b . totalCost ( 0, 1 ) == 100 );
  assert ( b . totalCost ( 19, 19 ) == -1 );

  CNet<CDumbString> c;
  c . add ( "Adam"s, "Bob"s, 100 )
    . add ( "Bob"s, "Carol"s, 200 )
    . add ( "Dave"s, "Adam"s, 300 )
    . add ( "Eve"s, "Fiona"s, 120 )
    . add ( "Kate"s, "Larry"s, 270 )
    . add ( "Ivan"s, "John"s, 70 )
    . add ( "Kate"s, "Ivan"s, 300 )
    . add ( "George"s, "Henry"s, 10 )
    . add ( "Eve"s, "George"s, 42 )
    . add ( "Adam"s, "Eve"s, 75 )
    . add ( "Ivan"s, "George"s, 38 )
    . optimize ();
  assert ( c . totalCost ( "Adam"s, "Bob"s ) == 100 );
  assert ( c . totalCost ( "John"s, "Eve"s ) == 150 );
  assert ( c . totalCost ( "Dave"s, "Henry"s ) == 427 );
  assert ( c . totalCost ( "Carol"s, "Larry"s ) == 1025 );
  assert ( c . totalCost ( "George"s, "George"s ) == 0 );
  assert ( c . totalCost ( "Alice"s, "Bob"s ) == -1 );
  assert ( c . totalCost ( "Thomas"s, "Thomas"s ) == -1 );
std::cout<<"Succes" << std::endl;
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */