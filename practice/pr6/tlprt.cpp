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
#include <limits>
#include <algorithm>
#include <memory>
#endif /* __PROGTEST__ */
 
struct CTeleport
{
    std::vector<std::string> knownPorts;
    std::map<std::string, std::vector<std::tuple<unsigned, std::string, unsigned>>> conAtPort;

    CTeleport() = default;
    ~CTeleport() = default;

    bool validPorts(std::string name){
        const auto found = std::find(knownPorts.begin(),knownPorts.end(),name);
        if(found!=knownPorts.end()){
            return true;
        } else {
            return false;
        }
    }
 
    CTeleport & Add ( const std::string & from, const std::string & to, unsigned fromTime, unsigned toTime ){
        if(!validPorts(from))
            knownPorts.push_back(from);
        if(!validPorts(to))
            knownPorts.push_back(to);
        conAtPort[from].emplace_back(fromTime, to, toTime);
        return *this;
    }   

    unsigned teleBFS(const std::string& from, const std::string& to, unsigned time){
            std::cerr <<"Volam bfs\n";
        if(!validPorts(from) || !validPorts(to))
            throw std::invalid_argument("Invalid exception");
            std::cerr <<"Valid porty "<< from << " & "<< to <<"\n";

        std::queue<std::pair<std::string,unsigned>> queue;
        std::map<std::string,unsigned> visited;
        unsigned bestTime = UINT_MAX;

        queue.push(std::make_pair(from,time));
        visited[from] = time;

        while(!queue.empty()){
            std::pair<std::string,unsigned> curr = queue.front();
            queue.pop();
            
            if(curr.first == to && bestTime > curr.second){
                bestTime = curr.second;
            }

            for(const auto& [departure, destination, arrival] : conAtPort[curr.first])
                if(departure >= curr.second){
                    if(!visited.count(destination)|| visited[destination] > arrival){
                        visited[destination] = arrival;
                        std::cerr << " " <<curr.first <<" -> " << destination;
                        queue.push({destination,arrival});
                    }
                }
        }
        if(bestTime!= UINT_MAX){
            return bestTime;
        }else {   
            std::cerr <<"\nNo spoj :(\n";
            return UINT_MAX;
        }
    }
 
    unsigned FindWay ( const std::string & from, const std::string & to,unsigned time ){
        std::cerr <<"Volam findWay\n";
        auto arrivalTime = teleBFS(from,to,time);

        if(arrivalTime == UINT_MAX)
            throw std::invalid_argument("Missing exception");
        std::cerr <<"\nArrival Time:  " <<arrivalTime << "\n";
        return arrivalTime;
    }
 
    CTeleport & Optimize ( void ){
        return *this;
    }
};
 
#ifndef __PROGTEST__
int main ( void )
{
    CTeleport t;
    t . Add ( "Prague", "Vienna", 0, 7 )
      . Add ( "Vienna", "Berlin", 9, 260 )
      . Add ( "Vienna", "London", 8, 120 )
      . Add ( "Vienna", "Chicago", 4, 3 )
      . Add ( "Prague", "Vienna", 10, 10 ) . Optimize ( );
 
    assert ( t . FindWay ( "Prague", "Vienna", 0 ) == 7 );
    assert ( t . FindWay ( "Prague", "Vienna", 1 ) == 10 );
    assert ( t . FindWay ( "Prague", "London", 0 ) == 120 );
    assert ( t . FindWay ( "Vienna", "Chicago", 4 ) == 3 );

    try { t . FindWay ( "Prague", "London", 2 ); assert ( "Missing exception" == nullptr ); }
    catch ( const std::invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }
    try { t . FindWay ( "Prague", "Chicago", 0 ); assert ( "Missing exception" == nullptr ); }
    catch ( const std::invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }

    t . Add ( "Dallas", "Atlanta", 150, 30 )
      . Add ( "Berlin", "Helsinki", 1080, 2560 )
      . Add ( "Chicago", "Frankfurt", 50, 0 )
      . Add ( "Helsinki", "Vienna", 3200, 3 )
      . Add ( "Chicago", "London", 10, 12 )
      . Add ( "London", "Atlanta", 20, 40 )
      . Add ( "Vienna", "Atlanta", 10, 50 )
      . Add ( "Prague", "Vienna", 1, 6 )
      . Add ( "Berlin", "Helsinki", 265, 265 ) 
      . Add ( "Berlin", "London", 259, 0 ) . Optimize ( );
    assert ( t . FindWay ( "Prague", "Frankfurt", 0 ) == 0 );
    assert ( t . FindWay ( "Prague", "Atlanta", 0 ) == 40 );
    assert ( t . FindWay ( "Prague", "Atlanta", 10 ) == 50 );
      std::cerr << "    Passed ALL\n";

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */