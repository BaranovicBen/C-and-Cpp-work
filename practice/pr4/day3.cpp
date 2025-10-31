#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>

struct starBase{
   std::string name;
   std::string& next;

   starBase() = default; //neviem urobit next este
};
struct starTrack {  
    std::vector<starBase> stops;
    std::vector<std::vector<int>> starJet;

    starTrack() = default;
    ~starTrack() = default;

//    starTrack() copy-konstruktor

    void add(){}
    void trip(){}
    void bases(){}
    
};


int main(){
    return 0;
}