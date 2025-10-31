#include <cassert>
#include <climits>
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include <limits>


struct CDoprava {
    std::vector<std::string> validWH;
    std::map<std::string, std::vector<std::pair<std::string,unsigned>>> conToWH;

    CDoprava() = default;

    bool isValidWH(std::string sus){
        std::cerr << "Checking if valid\n";
        const auto found = std::find(validWH.begin(), validWH.end(),sus);
        if(found != validWH.end()){
            return true;
        } else {
            return false;
        }

    }

    void road(const std::string &from, const std::string &to, unsigned weight) {
        if(weight == 0 || from == to)
            throw std::invalid_argument("Invalid_argument");
        if(!isValidWH(from)) 
            validWH.push_back(from);        
        if(!isValidWH(to)) 
            validWH.push_back(to);
        std::cerr << "Adding connection "<<from << " -> "<< to<< " with w: " << weight << "\n";

        conToWH[from].push_back(std::make_pair(to,weight));
        conToWH[to].push_back(std::make_pair(from,weight));
    }

    void optimize() {
        for (auto& [city, edges] : conToWH) {
            std::map<std::string, unsigned> best;
            for (const auto& [dest, weight] : edges) {
                auto it = best.find(dest);
                if (it == best.end() || weight > it->second) {
                    best[dest] = weight;
                }
            }

            edges.clear();
            for (const auto& [dest, weight] : best) {
                edges.emplace_back(dest, weight);
            }
        }
    }

    unsigned bfsDepo(const std::vector<std::string> &depots, unsigned reqCities, unsigned minW){
        std::queue<std::string> queue;
        std::map<std::string,bool> visited;
        unsigned count = 0;

        for(std::string name : validWH){
            visited[name] = false;
        }

        for(const auto &depo : depots){
            visited[depo] =  true;
            queue.push(depo);
            count++;
        }

        while(!queue.empty()){
            std::string curr = queue.front();
            queue.pop();

            for(const auto& [dest, weigth] : conToWH[curr]){
                if(!visited[dest] && weigth >=minW){
                    visited[dest] = true;
                    queue.push(dest);
                    count++;
                }
            }
        }

        return count >= reqCities;
    }

    unsigned maxWeight(const std::vector<std::string> &depots, unsigned reqCities) {
        if(depots.size() >= reqCities)
            return UINT_MAX;
        if(conToWH.size() < reqCities)
            throw std::logic_error("Not enough cities!");
        unsigned maxEdge = 0;

        for(const auto& [city , con] : conToWH){
            for(const auto& [dest,  w ] : conToWH[city]){
                maxEdge = std::max(maxEdge,w);
            }
        }
        for(int w = maxEdge ; w >= 1 ;w--){
            if(bfsDepo(depots, reqCities, w)){
                std::cerr << "returning " << w << "\n";
                return w;
            }
        }
        return 0;
    }
};

void testy(){
    CDoprava doprava;

// Prvé tri mestá
doprava.road("Trosky", "Troskovice", 10);
doprava.road("Troskovice", "Semin", 5);

// Vetva Ratbor - Bohunice - Malesov
doprava.road("Troskovice", "Ratbor", 12);
doprava.road("Ratbor", "Bohunice", 15);
doprava.road("Bohunice", "Malesov", 20);

// Vetva Kuttenberg - Suchdol - Nebakov
doprava.road("Ratbor", "Kuttenberg", 11);
doprava.road("Kuttenberg", "Suchdol", 7);
doprava.road("Kuttenberg", "Suchdol", 9);   // silnejšia
doprava.road("Kuttenberg", "Suchdol", 6);   // slabšia
doprava.road("Suchdol", "Nebakov", 4);
doprava.road("Kuttenberg", "Nebakov", 3);

// -- Test 1: prepojenie troch miest priamo
assert(doprava.maxWeight({"Trosky"}, 3) == 10);  // cesta Trosky->Troskovice->Semin
std::cerr << "Test 1 passed\n";

// -- Test 2: test slabších hrán pred optimalizáciou
assert(doprava.maxWeight({"Kuttenberg"}, 3) == 11);  // cesta ide cez váhu 3
std::cerr << "Test 2 passed\n";

// -- Test 3: po optimalizácii by sa mali slabšie hrany odstrániť
doprava.optimize();  // odstráni Kuttenberg->Suchdol 6 a 7, nechá len 9
assert(doprava.maxWeight({"Kuttenberg"}, 9) == 4);  // slabšia časť cez Nebakov
std::cerr << "Test 3 passed\n";

// -- Test 4: test vetvy cez Ratbor
assert(doprava.maxWeight({"Troskovice"}, 5) == 11);  // najhoršia hrana do Kuttenbergu 
std::cerr << "Test 4 passed\n";

// -- Test 5: kontrola že UINT_MAX sa vráti keď máme dosť depozitov
assert(doprava.maxWeight({"Trosky", "Troskovice", "Semin"}, 3) == UINT_MAX);
std::cerr << "Test 5 passed\n";

std::cout << "Všetky rozšírené testy prebehli úspešne.\n";
}

int main() {
    CDoprava doprava;
    testy();
    return 0;
}