#include <iostream>
#include <vector>
#include <queue>

struct Node{
    char data;

    Node(char data){
        this->data = data;
    }
};

std::ostream& operator<<(std::ostream& os, const Node& node){
    os << node.data;
    return os;
};

struct Graph {
    std::vector<Node> nodes;
    std::vector<std::vector<int>> matrix;

    Graph() = default;

    void addNode(Node node) {
        nodes.push_back(node);

        for (auto& row : matrix) {
            row.push_back(0);
        }
        
        matrix.push_back(std::vector<int>(nodes.size(), 0));
    }

    void addEdge(int src, int des){
        matrix[src][des] = 1;
    }

    bool checkEdge(int src, int des){
        return matrix[src][des]==1;
    }

    void printGraph(){
        std::cout << "  ";
        for(int i = 0; i != matrix.size();i++){
            std::cout << nodes[i] << " ";
        }
        std::cout << std::endl;
        for(int i = 0; i < matrix.size();i++){   
            std::cout << nodes[i] << " ";
            for (int j= 0; j< matrix[i].size();j++){
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void bfSearch(int src){
        std::queue<int> queue;
        std::vector<bool> visited(matrix.size(), false);
        std::vector<int> dist(matrix.size(), -1);

        queue.push(src);
        visited[src] = true;
        dist[src] = 0;
        while (queue.size()!=0){

            src = queue.front();
            queue.pop();
            std::cout << nodes[src] <<  " = Visited (distance = " << dist[src] << ")\n";

            for(int i = 0; i< matrix[src].size();i++){
                if(matrix[src][i] == 1 && !visited[i]){
                    dist[i] = dist[src] + 1;
                    visited[i] = true;
                    queue.push(i);
                }
            }
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

    graph.bfSearch(3);
    return 0;
}