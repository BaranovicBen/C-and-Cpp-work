#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <map>

struct City {
    std::string name;

    City(std::string name){
        this->name = name;
    }
    City() = default;
};

std::ostream& operator<<(std::ostream& os, const City& city){
    os << city.name;
    return os;
}

struct Graph {
    std::vector<City> cities;                  
    std::vector<std::vector<int>> regioJet;    

    Graph() = default;

    void addStanice(int id, const City& city) {
        if ((int)cities.size() <= id) {
            cities.resize(id+1);
        }
        cities[id] = city;

        if ((int)regioJet.size() <= id) {
            regioJet.resize(id+1);
        }
    }

    void addTrasa(int src, int dst) {
        regioJet[src].push_back(dst);
        regioJet[dst].push_back(src);
    }

    void bfSearch(int src, int dst) {
        int n = (int)regioJet.size();
        std::queue<int> q;
        std::vector<bool> visited(n, false);
        std::vector<int> dist(n, -1);

        visited[src] = true;
        dist[src] = 0;
        q.push(src);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            std::cout << cities[u].name
                      << " = Visited (distance = " << dist[u] << ")\n";

            if (u == dst) break;

            for (int v : regioJet[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }
        if (dist[dst] >= 0) {
            std::cout << "Cesta do " << cities[dst].name
                      << " ma dlzku " << dist[dst] << " prestupov.\n";
        } else {
            std::cout << "Cesta neexistuje.\n";
        }
    }
};

int main() {
    Graph g;

    // Predpokladajme, že už poznáš ID pre každé mesto:
    // napr. Praha má ID = 0, Brno = 1, Ostrava = 2, atď.
g.addStanice(0, City("Praha"));
g.addStanice(1, City("Pardubice"));
g.addStanice(2, City("Kutna Hora"));
g.addStanice(3, City("Brno"));
g.addStanice(4, City("Breclav"));
g.addStanice(5, City("Bratislava"));
g.addStanice(6, City("Olomouc"));
g.addStanice(7, City("Ostrava"));
g.addStanice(8, City("Plzen"));
g.addStanice(9, City("Karlovy Vary"));
g.addStanice(10, City("Usti nad Labem"));
g.addStanice(11, City("Liberec"));


g.addTrasa(0, 1);  // Praha ↔ Pardubice
g.addTrasa(1, 2);  // Pardubice ↔ Kutna Hora
g.addTrasa(2, 3);  // Kutna Hora ↔ Brno
g.addTrasa(3, 4);  // Brno ↔ Breclav
g.addTrasa(4, 5);  // Breclav ↔ Bratislava

g.addTrasa(4, 3);  // Breclav ↔ Brno  (toto bolo už pridané vyššie, ale je v poriadku mať duplikát iba raz)
g.addTrasa(3, 6);  // Brno ↔ Olomouc
g.addTrasa(6, 7);  // Olomouc ↔ Ostrava
g.addTrasa(7,11);

g.addTrasa(0, 8);   // Praha ↔ Plzen
g.addTrasa(8, 9);   // Plzen ↔ Karlovy Vary
g.addTrasa(9, 10);  // Karlovy Vary ↔ Usti nad Labem
g.addTrasa(10, 11); // Usti nad Labem ↔ Liberec

g.bfSearch(1, 7);

return 0;
}
