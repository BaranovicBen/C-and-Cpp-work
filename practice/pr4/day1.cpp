#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>

struct City {
    std::string name;

    City(std::string name){
        this->name = name;
    }
};


struct Graph {
    std::vector<City> cities;
    std::vector<std::vector<int>> cdMatrix;

    Graph() = default;

    void addCity(City city){
        cities.push_back(city);
        for (auto& row : cdMatrix) {
            row.push_back(0);
        }
        cdMatrix.push_back(std::vector<int>(cities.size(), 0));
    }

    void addEdge(const std::string& src, const std::string& des, const std::map<std::string, int>& cityToID){
        int srs = cityToID.at(src);
        int dst = cityToID.at(des);

        cdMatrix[srs][dst] = 1;
    }
};

int main() {
    std::map<std::string,int> cityToID;
    Graph graph;

    cityToID["Praha"]=0;
    cityToID["Brno"]=1;
    cityToID["Ostrava"]=2;
    cityToID["Plzen"]=3;
    cityToID["Liberec"]=4;
    cityToID["Zlin"]=5;

    graph.addCity(City("Praha"));
    graph.addCity(City("Brno"));
    graph.addCity(City("Ostrava"));
    graph.addCity(City("Plzen"));
    graph.addCity(City("Liberec"));
    graph.addCity(City("Zlin"));

    graph.addEdge("Praha", "Brno",cityToID);
    graph.addEdge("Brno", "Ostrava",cityToID);
    graph.addEdge("Praha", "Plzen",cityToID);
    graph.addEdge("Plzen", "Liberec",cityToID);
    graph.addEdge("Brno", "Zlin",cityToID);

    std::cout << "Počet jedinečných miest: " << cityToID.size() << std::endl;

    return 0;
}
