#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

template <typename T>
struct Vector{
    std::vector<T> vctr;

    Vector() = default;
    ~Vector() = default;

    Vector(std::vector<T>& other){
        std::cout << "Copy Constructing vector\n";
        vctr = other;
    }

    T getV(unsigned i){
        std::cout << "Getting element in vector:\n";
        return vctr[i];
    }

    void iterateV(){
        std::cout << "Iterating vector:\n";
        for(const auto& element : vctr){
            std::cout << "->" << element <<"\n";
        }
    }

    void push_back(T inpt){
        std::cout << "Adding value\n";
        vctr.push_back(inpt);
    }
};

template <typename T, typename T_>
struct Map {
    std::map<T,T_> mapa;

    Map() = default;
    ~Map() = default;

    Map(const std::map<T,T_>& other){
        std::cout << "Deep Constructing map\n";
        mapa = other;
    }

    T_ getM(T key){
        std::cout << "Getting element\n";
        return mapa[key];
    }

    void iterateM(){
        std::cout << "Iterating map\n";
        for(const auto& [key, value] : mapa){
            std::cout << key << "->" << value << "\n";
        }
    }

    void iterateMV(){
        std::cout << "Iterating map\n";
        for(const auto& [key, value] : mapa){
            std::cout << key;
            for(const auto& val : value){
                std::cout << "\t->" << val << "\n";
            }
            std::cout<< "\n";
        }
    }

    void emplace(const T key, const T_ value){
        std::cout << "Adding value\n";
        if(mapa.count(key)){
            mapa[key].insert(mapa[key].end(), value.begin(), value.end());
        } else {
            mapa[key] = value;
        }
    }

    void addOne(const T& key, const typename T_::value_type& item) {
        std::cout << "Adding single value\n";
        mapa[key].push_back(item);
    }

};

struct lL{
    std::string city;
    lL* nextCity;

    lL() = default;
    ~lL() = default;

    lL(std::string city)
        : city(city), nextCity(nullptr) {};

    static lL* deepCpy(lL* other){
        if(!other){
            return nullptr;
        }

        lL* copy = new lL(other->city);
        lL* origCur = other->nextCity;
        lL* copyCurr = copy;

        while(origCur && origCur != other){
            copyCurr->nextCity = new lL(origCur->city);
            copyCurr = copyCurr->nextCity;
            origCur = origCur->nextCity;
        }
        copyCurr->nextCity = copy;
        return copy;
    }
};

struct traffik{
    lL* first;

    traffik(lL* first)
        :first(first) {};
    ~traffik() = default;

    void buildTraffic(const std::vector<std::string> cities){
        if(cities.empty()) return;

        first = new lL(cities[0]);
        lL* curr = first;

        for(size_t i = 1; i != cities.size();i++){
            curr->nextCity = new lL(cities[i]);
            curr= curr->nextCity;
        }

        curr->nextCity = first;
    }

    void printRad(){
        if(first == nullptr) return;

        std::string head = first->city;
        lL* curr = first;

        while(1){
            std::cout << curr->city << " -> ";
            curr = curr->nextCity;

            if(curr->city == head) {
                std::cout << head << "\n";
                break;
            }
        }
    }

    void printPath(const std::string& from, const std::string& to){
        if(!first) return;

        lL* curr = first;

        while (curr->city != from ){
            curr = curr->nextCity;
            if(curr->city == first->city){
                std::cout << "City " << from << " isnt on the list\n";
            }
        }

        std::cout << curr->city;

        while(curr->city!= to){
            curr = curr->nextCity;
            std::cout << " -> " << curr->city;
            if(curr->city == from){
                std::cout << "City " << to << " isnt on the list\n";
                return;
            }
        }

        std::cout << "\n";
    }
};

struct AdjList {
    std::map<int, std::vector<int>> adj;

    // ✅ Default constructor
    AdjList() = default;

    // ✅ Deep copy constructor
    AdjList(const AdjList& other) {
        std::cout << "[Deep copy constructor called]\n";
        adj = other.adj;
    }

    // ✅ Pridanie hrany (orientovaný graf)
    void addEdge(int from, int to) {
        adj[from].push_back(to);
    }

    // ✅ Prepísanie susedov konkrétneho uzla
    void overwriteNeighbors(int node, const std::vector<int>& newNeighbors) {
        adj[node] = newNeighbors;
    }

    // ✅ Pridanie jedného suseda konkrétnemu uzlu
    void addOneNeighbor(int node, int neighbor) {
        adj[node].push_back(neighbor);
    }

    // ✅ Iterácia a výpis všetkých susedov
    void print() const {
        std::cout << "Graph contents:\n";
        for (const auto& [node, neighbors] : adj) {
            std::cout << node << ": ";
            for (int n : neighbors) std::cout << n << " ";
            std::cout << "\n";
        }
    }

    // ✅ Získanie susedov uzla
    const std::vector<int>& getNeighbors(int node) const {
        return adj.at(node); // môže hodiť výnimku ak node neexistuje
    }

    // ✅ Skontroluj, či hrana existuje
    bool hasEdge(int from, int to) const {
        auto it = adj.find(from);
        if (it == adj.end()) return false;
        return std::find(it->second.begin(), it->second.end(), to) != it->second.end();
    }
};
void testVCTR(){
    std::vector<int> intV = {1,2,34,5};
    std::vector<std::string> strV = {"Dano","Ctibor","Jindra"};

    Vector<int> intVS(intV);
    Vector<std::string> strVS(strV);

    Vector<int> dCopy(intVS);

    intVS.getV(2);
    strVS.getV(0);

    intVS.push_back(6);
    strVS.push_back("Jan");

    intVS.iterateV();
    strVS.iterateV();

    dCopy.iterateV();
};
void testMAP(){
    std::map<int,std::string> poradovnik;
    std::map<std::string,std::vector<std::string>> cesty;

    poradovnik.emplace(0, "Dano");
    poradovnik.emplace(1, "Jano");
    poradovnik.emplace(2, "Ludo");
    poradovnik.emplace(3, "Peder");

    cesty.emplace("Trosky", std::vector<std::string>{"Semin", "Troskovice", "Apoleny"});
    cesty.emplace("KuttenBerg", std::vector<std::string>{"Malesov", "Suchdol", "Buhunice", "Ratbor"});

    Map<int,std::string> pM(poradovnik);
    Map<std::string,std::vector<std::string>> cM(cesty);

    pM.getM(2);
    cM.getM("KuttenBerg");

    cM.addOne("Trosky","Miskovice");
    pM.emplace(2,"Gabor");
    pM.emplace(10,"Kim");
    cM.iterateMV();
    pM.iterateM();
}
void testlL(){
    lL* firstStation = new lL("KuttenBerg");
    traffik rad(firstStation);

    rad.buildTraffic({"KuttenBerg","Malesov","Miskovice","Bohunovice","Certovka","Suchdol","Ratbor","Biliny"});

    rad.printPath("Malesov","KuttenBerg");

    rad.printRad();
};


int main(){
    std::cout << "\t---Vector testing---\n";
    testVCTR();
    std::cout << "\t---Map testing---\n";
    testMAP();
    std::cout << "\t---Linked List testing---\n";
    testlL();
    AdjList g;

    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addOneNeighbor(2, 5);

    g.print();

    std::cout << "\nOverwriting neighbors of 1...\n";
    g.overwriteNeighbors(1, {99, 100});
    g.print();

    std::cout << "\nChecking edge from 2 to 5: " << (g.hasEdge(2, 5) ? "YES" : "NO") << "\n";
    std::cout << "\nChecking edge from 3 to 1: " << (g.hasEdge(3, 1) ? "YES" : "NO") << "\n";

    std::cout << "\nDeep copy test:\n";
    AdjList copied = g;
    copied.print();

    return 0;
}