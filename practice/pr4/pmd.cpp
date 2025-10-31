#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <set>
#include <cassert>
#include <sstream>


struct Stop {
    std::string name;
    std::string line;

    Stop(const std::string name, const std::string line){
        this-> name = name;
        this->line = line;
    } 

    ~Stop() = default;
};

std::ostream& operator<<(std::ostream& os, const Stop& stop){
    os << stop.name << "(" << stop.line <<")";
    return os;
}

template<typename T, typename U>
void printList(std::map<T , U> matrix){
    for(const auto& [key,value] : matrix ){
        std::cout << key << " | ";
        for(size_t j = 0; j != value.size();j++){
            std::cout << value[j];
            if(j != value.size()-1){
                std::cout <<" -> ";
            }
        }
        std::cout << '\n';
    }
    std::cout << '\n';
};

struct DPP {
    std::vector<std::string> stopsNames;

    std::map<std::string, std::vector<Stop>> conAtStop;
    std::map<std::string, std::vector<std::string>> lineStops;

    int validStop(std::string name){
        const auto found = std::find(stopsNames.begin(),stopsNames.end(),name);
        if(found!=stopsNames.end()){
            return 1;
        } else {
            return -1;
        }
    }

    void addStopToLine(std::string stopName,std::string line){
        if(validStop(stopName)==-1){
            stopsNames.push_back(stopName);
        }
        lineStops[line].push_back(stopName);
    }

    void addConnection(std::string stopName, Stop stopNxt){
        conAtStop[stopName].push_back(stopNxt);
    }

    void bfs(std::string from, const std::string to){
        if(validStop(from) ==-1 || validStop(to) ==-1){
            std::cout << "Stop doesnt exist\n";
        }

        std::queue<std::string> queue;

        std::map<std::string, bool> visited;
        std::map<std::string, int> distance;
        std::map<std::string,std::string> path;
        std::map<std::string,std::string> linesOnPath;

        for(std::string stop : stopsNames){
            visited[stop] = false;
            distance[stop] = -1;
        }

        queue.push(from);
        visited[from] = true;
        distance[from] = 0;

        while (!queue.empty()){
            std::string curr = queue.front();
            queue.pop();


            for(const Stop& s : conAtStop[curr]){
                std::string nextStop = s.name;
                std::string nextLine = s.line;
                if(!visited[nextStop]){
                    visited[nextStop] = true;
                    distance[nextStop] = distance[curr] + 1;
                    path[nextStop] = curr;
                    linesOnPath[nextStop] = nextLine;
                    queue.push(nextStop);
                }
            }

            if(curr == to){
                break;
            }
        }

        if(!visited[to]){
            std::cout << "YOU SHALL NOT PASS\n";
            return;
        }

        std::vector<std::string> plan;
        std::vector<std::string> lines;
        for(std::string at = to; at!= from; at = path[at]){
            plan.push_back(at);
            lines.push_back(linesOnPath[at]);
        }

        plan.push_back(from);


        std::reverse(plan.begin(),plan.end());
        std::reverse(lines.begin(),lines.end());


        std::cout<<"Planned route: ";
        for(int i = 0; i < plan.size(); i++){
            std::cout<< plan[i];
            if (i < lines.size()) {
                std::cout << " (" << lines[i] << ")";
                std::cout << " -> ";
            }
    
        }
        std::cout << " (End)"<< "\n";
    }
};

void testData(DPP& p){
    p.addStopToLine("Budatinska","99");
    p.addStopToLine("Gercenova","99");
    p.addStopToLine("Farskeho","99");
    p.addStopToLine("Ovsiste","99");
    p.addStopToLine("Bradacova","99");
    p.addStopToLine("Jiraskova","99");


    p.addStopToLine("Budatinska","93");
    p.addStopToLine("Stanica Petrzalka","93");
    p.addStopToLine("Dvory","93");
    p.addStopToLine("Aupark","93");

    p.addStopToLine("Aupark","88");
    p.addStopToLine("Gercenova","88");
    p.addStopToLine("Namestie Hraniciarov","88");
    p.addStopToLine("Sustekova","88");
    p.addStopToLine("Ekonomicka","88");

    p.addStopToLine("Lachova","96");
    p.addStopToLine("Namestie Hraniciarov","96");
    p.addStopToLine("Romanova","96");
    p.addStopToLine("Bradacova","96");


    p.addConnection("Budatinska",Stop("Stanica Petrzalka","93"));
    p.addConnection("Stanica Petrzalka",Stop("Dvory","93"));
    p.addConnection("Dvory",Stop("Aupark","93"));
    p.addConnection("Aupark",Stop("Budatinska","93"));

    p.addConnection("Lachova",Stop("Namestie Hraniciarov","96"));
    p.addConnection("Namestie Hraniciarov",Stop("Romanova","96"));
    p.addConnection("Romanova",Stop("Bradacova","96"));
    p.addConnection("Bradacova",Stop("Lachova","96"));


    p.addConnection("Aupark",Stop("Gercenova","88"));
    p.addConnection("Gercenova",Stop("Namestie Hraniciarov","88"));
    p.addConnection("Namestie Hraniciarov",Stop("Sustekova","88"));
    p.addConnection("Sustekova",Stop("Ekonomicka","88"));
    p.addConnection("Ekonomicka",Stop("Aupark","88"));


    p.addConnection("Bradacova",Stop("Jiraskova","99"));
    p.addConnection("Jiraskova",Stop("Gercenova","99"));
    p.addConnection("Gercenova",Stop("Farskeho","99"));
    p.addConnection("Farskeho",Stop("Sustekova","99"));
    p.addConnection("Sustekova",Stop("Ovsiste","99"));
    p.addConnection("Ovsiste",Stop("Bradacova","99"));

}

int main(){
    DPP petrzka;

    testData(petrzka);

    printList(petrzka.conAtStop);
    printList(petrzka.lineStops);


    std::string from;
    std::string to;
    while(1){
        std::cout << "Kde si? (q to quit): ";
        std::getline(std::cin,from);    

        if(from == "q") {
            break;
        }   

        std::cout << "Kam ides? : ";
        std::getline(std::cin,to);

        petrzka.bfs(from,to);
    }

    return 0;
}