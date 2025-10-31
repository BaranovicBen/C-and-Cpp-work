#include <iostream>
#include <vector>

struct  Node {
    char data;

    Node(char data){
        this->data = data;
    }
};

std::ostream& operator<<(std::ostream& os, const Node& node) {
    os << node.data;
    return os;
};

struct Graph {
    std::vector<std::vector<Node>> adjList;
    Graph() = default;
    
    void addNode(Node node){
        std::vector<Node> currentList;
        currentList.push_back(node);
        adjList.push_back(currentList);
    }

    void addEdge(int src, int des){
        adjList[src].push_back(adjList[des][0]);
    }

    void printGraph(){

        for(int i = 0; i != adjList.size(); i++) {
            std::cout << adjList[i][0] << ':';

            for(int j = 0; j != adjList[i].size();j++){
                std::cout << adjList[i][j] << "->";
            }
            std::cout << std::endl;
        }
    }
};

int main(){
    Graph graph;

    graph.addNode(Node('A'));
    graph.addNode(Node('B'));
    graph.addNode(Node('C'));
    graph.addNode(Node('D'));
    graph.addNode(Node('E'));

    graph.addEdge(0,1);
    graph.addEdge(1,2);
    graph.addEdge(1,4);
    graph.addEdge(2,3);
    graph.addEdge(2,4);
    graph.addEdge(4,0);
    graph.addEdge(4,2);

    graph.printGraph();

    return 0;
};