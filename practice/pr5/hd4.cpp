#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <limits>
 
struct CTransport {
    std::map<std::string, std::vector<std::string>> conAtStop;

    CTransport() = default;
    ~CTransport() = default;
    
    CTransport& addLine(std::istream& is){
        std::string curr;
        std::string prev;
        std::getline(is,prev);
        std::string first = prev;

        while(1){
            std::getline(is,curr);
            if(curr.empty()) break;
            std::cerr << "Prev: [" << prev << "]" <<" Curr [" << curr << "]\n";
            conAtStop[prev].push_back(curr);
            conAtStop[curr].push_back(prev);
            prev = curr;
            if(curr == first ) break;
        }
        std::cerr << '\n';
        return *this;
    }

    std::map<std::string,int> distanceMapBFS(std::string from) const {
       std::queue<std::string> queue;
       std::map<std::string,bool> visited;
       std::map<std::string,int> distance;

        for(const auto& [stop,list] : conAtStop){
                visited[stop] = false;
                distance[stop] = -1;
        }

        queue.push(from);
        visited[from] = true;
        distance[from] = 0;

        while(!queue.empty()){
            std::string curr = queue.front();
            queue.pop();

            auto it = conAtStop.find(curr);
            if(it != conAtStop.end()){
                for(std::string stop : it->second){
                    std::string nextStop = stop;
                    if(!visited[nextStop]){
                        if(conAtStop.find(from)!=conAtStop.end()){
                            visited[nextStop] = true;
                            distance[nextStop] = distance[curr] +1;
                            queue.push(nextStop);
                        }
                    }
                }
            }
        }

        return distance;
    }
    
    std::set<std::string> meetingPoints(const std::vector<std::string>& fromList, int& sumCost) const{
        std::vector<std::map<std::string, int>> distances;
        std::map<std::string,int> totalCount;
        std::map<std::string,int> validCount;

        std::set<std::string> meetUps;

        for(int i = 0 ; i < fromList.size();i++){
            std::map<std::string, int> dist = distanceMapBFS(fromList[i]);
            distances.push_back(dist);
        }
        totalCount.clear();
        validCount.clear();

        for(auto& map : distances){
            for(auto& [key,val] : map){
                if(val!=-1){
                    totalCount[key] += val;
                    validCount[key]++;
                }
            }
        }   

        for (auto& [key, val] : totalCount) {
            if (validCount[key] == fromList.size()) {
                std::cerr << "Stop: [" << key << "], total: " << val << ", valid: " << validCount[key] << "\n";
            }
        }

        int min = INT_MAX;

        for(auto& [key,val] : totalCount){
            int sus = totalCount[key];
            if(sus < min){
                min = sus;
            }
        }

        for(auto& [key,val] : totalCount){
            if(val == min && validCount[key] == fromList.size()){
                std::cerr << "Found a spot: [" << key <<" , "<< min <<"]\n";
                meetUps.insert(key);
            }
        }
        if (!meetUps.empty()) {
            sumCost = min;
        }
        std::cerr <<'\n';
        return meetUps;
    };

};
 
int main() {
  CTransport t;
  std::istringstream iss;
  iss.clear();
  iss.str(
    "Newton\n"
    "Black Hill\n"
    "Wood Side\n"
    "Green Hill\n"
    "Lakeside\n"
    "Newton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Wood Side\n"
    "Green Hill\n"
    "Little Burnside\n"
    "Great Newton\n"
    "Wood Side\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Little Newton\n"
    "Little Burnside\n"
    "Castle Hill\n"
    "Newton Crossroad\n"
    "Lakeside Central\n"
    "Little Newton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Lakeside Central\n"
    "Little Waterton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Little Waterton\n"
    "Waterton West\n"
    "Waterton Central\n"
    "Waterton East\n"
    "Waterton Woods\n"
    "Waterton North\n"
    "Waterton East\n"
    "Little Waterton\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Tidmouth\n"
    "Gordon's Hill\n"
    "Suderry\n"
    "Knapford\n"
    "Great Waterton\n"
    "Brendam Docks\n"
    "Tidmouth\n"
  );
  t.addLine(iss);
  iss.clear();
  iss.str(
    "Sodor Ironworks\n"
    "Sodor Steamwork\n"
    "Knapford\n"
    "Maron\n"
    "Gordon's Hill\n"
    "Sodor Ironworks\n"
  );
  t.addLine(iss);
 
  int cost;
  assert(
    (t.meetingPoints({"Newton"}, cost) ==
     std::set<std::string> {"Newton"}) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Newton", "Newton"}, cost) ==
     std::set<std::string> {"Newton"}) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Brno"}, cost) == std::set<std::string> {"Brno"}
    ) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Brno", "Brno"}, cost) ==
     std::set<std::string> {"Brno"}) &&
    (cost == 0)
  );
 
  assert(
    (t.meetingPoints({"Newton", "Brno"}, cost) ==
     std::set<std::string> {})
  );
 
  assert(
    (t.meetingPoints({"Newton", "Black Hill"}, cost) ==
     std::set<std::string> {"Newton", "Black Hill"}) &&
    cost == 1
  );
 
  assert(
    (t.meetingPoints({"Newton", "Newton", "Black Hill"}, cost) ==
     std::set<std::string> {"Newton"}) &&
    cost == 1
  );
 
  assert(
    (t.meetingPoints({"Newton", "Black Hill", "Wood Side"}, cost) ==
     std::set<std::string> {"Black Hill"}) &&
    cost == 2
  );
 
  assert(
    (t.meetingPoints({"Newton Crossroad", "Little Waterton"}, cost) ==
     std::set<std::string> {
       "Newton Crossroad", "Little Waterton", "Lakeside Central"
     }) &&
    cost == 2
  );
 
  assert(
    (t.meetingPoints({"Suddery", "Little Waterton"}, cost) ==
     std::set<std::string> {})
  );
 
  assert(
    (t.meetingPoints({"Tidmouth", "Newton Crossroad"}, cost) ==
     std::set<std::string> {})
  );
 
  iss.clear();
  iss.str(
    "Newton\n"
    "Newton Crossroad\n"
    "Tidmouth\n"
    "Newton\n"
  );
  t.addLine(iss);
 
  assert(
    (t.meetingPoints({"Tidmouth", "Newton Crossroad"}, cost) ==
     std::set<std::string> {"Tidmouth", "Newton Crossroad"}) &&
    cost == 1
  );

  std::cerr << "Finished!\n";
}