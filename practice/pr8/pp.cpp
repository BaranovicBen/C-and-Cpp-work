#ifndef __PROGTEST__
#include <algorithm>
#include <assert.h>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#endif /* __PROGTEST__ */
 #include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <map>

struct CSpread {
    enum State { 
        UNTOUCHED, BELIEVER, IMMUNE 
    };
    std::unordered_map<std::string, std::vector<std::string>> adj;
  
    struct Info { 
            State st;
            std::string theory; 
    };
    
    std::unordered_map<std::string,Info> status;
  
    CSpread& addfriend(const std::string &a, const std::string &b) {
        adj[a].push_back(b);
        adj[b].push_back(a);

        if (!status.count(a)) status[a]={UNTOUCHED,""};
        if (!status.count(b)) status[b]={UNTOUCHED,""};
        return *this;
    }
    CSpread& optimize() { 
        return *this; 
    }
        
    std::map<std::string,int> simulate(const std::map<std::string,std::string> &spreaders) {

        for (auto &kv : status) {
        kv.second = { UNTOUCHED, "" };
        }
        std::queue<std::pair<std::string,std::string>> q;

        for (auto &kv : spreaders) {
            const auto &person = kv.first;
            const auto &theory = kv.second;
            status[person] = { BELIEVER, theory };
            q.push({person,theory});
        }
        
        std::unordered_map<std::string,std::vector<std::string>> nextSeen;
        
        while (!q.empty()) {
            size_t n = q.size();
            for (size_t i = 0; i < n; ++i) {
                auto [from, th] = q.front(); q.pop();
                
                for (auto &nbr : adj[from]) {
                    if (status[nbr].st == UNTOUCHED)
                        nextSeen[nbr].push_back(th);
                }
            }

            for (auto &kv : nextSeen) {
                const auto &p = kv.first;
                auto &seenTheories = kv.second;
                if (status[p].st == UNTOUCHED) {
                    std::unordered_set<std::string> distinct(seenTheories.begin(), seenTheories.end());
                    if (distinct.size() == 1) {
                        auto only = *distinct.begin();
                        status[p] = { BELIEVER, only };
                        q.push({p, only});
                    } else if (distinct.size() > 1) {
                        status[p].st = IMMUNE;
                    }
                }
            }
            nextSeen.clear();
        }
        
        std::map<std::string,int> result;
        for (auto &kv : status) {
            if (kv.second.st == BELIEVER) {
                result[kv.second.theory]++;
            }
        }
        return result;
    }
};

 
 
#ifndef __PROGTEST__
 
int main() {
    CSpread x0;
    x0.addfriend("Dave", "Eve");
    x0.addfriend("Dave", "John");
    x0.addfriend("Eve", "Peter");
    x0.addfriend("John", "Peter");
    x0.addfriend("Peter", "Rob");
    x0.addfriend("Peter", "Mark");
    x0.addfriend("Rob", "Lily");
    x0.addfriend("Rob", "Dave");
    x0.addfriend("Rob", "Eve");
    x0.addfriend("Mark", "Lily");
    x0.optimize();
 
    assert(x0.simulate({{"Dave", "chemtrails"}}) == (std::map<std::string, int> { {"chemtrails", 7} }));
 
    assert(x0.simulate({{"Dave", "chemtrails"}, {"Eve", "chemtrails"}}) == (std::map<std::string, int> { {"chemtrails", 7} }));
 
    assert(x0.simulate({{"Lily", "chemtrails"}}) == (std::map<std::string, int> { {"chemtrails", 7} }));
 
    assert(x0.simulate({{"Dave", "chemtrails"}, {"John", "moon-landing"}}) == (std::map<std::string, int> { {"chemtrails", 4}, {"moon-landing", 3 }}));
    // Dave - chemtrails, John - moon, Eve - chemtrails, Peter - moon, Rob - chemtrails, Mark - moon, Lily - chemtrails
 
 
    CSpread x1;
    x1.addfriend("Dave", "Eve");
    x1.addfriend("Dave", "John");
    x1.addfriend("Eve", "Peter");
    x1.addfriend("Eve", "John");
    x1.addfriend("Peter", "Rob");
    x1.addfriend("Peter", "Mark");
    x1.addfriend("Rob", "Lily");
    x1.addfriend("Rob", "Dave");
    x1.addfriend("Rob", "Eve");
    x1.addfriend("Mark", "Lily");
    x1.optimize();
 
    assert(x1.simulate({{"Dave", "chemtrails"}, {"Eve", "moon-landing"}}) == (std::map<std::string, int> { {"chemtrails", 1}, {"moon-landing", 4 }}));
    // Dave - chemtrails, Eve - moon,  Peter - moon, Mark - moon, Lily - moon, Rob - immune, John - immune
 
    std::cerr << "All passed\n";
 
}
#endif /* __PROGTEST__ */