/*
#include <vector>
using Vec = std::vector<int>;
Vec v;

// 📥 Pridanie
v.push_back(42);        // vloží na koniec

// 📤 Prístup
int x = v[0];           // rýchly, bez kontroly rozsahu
int y = v.at(0);        // pomalší, ale s kontrolou (out_of_range)

// 📏 Veľkosť a kontrola
size_t n = v.size();    
bool empty = v.empty(); 

// ❌ Odstránenie
v.pop_back();           // odstráni posledný prvok
v.clear();              // vymaže všetko

// 🔁 Iterácia
for (int a : v)       
  std::cout << a;
for (auto it = v.begin(); it != v.end(); ++it)
  std::cout << *it;

// 🔍 Hľadanie
#include <algorithm>
auto it = std::find(v.begin(), v.end(), 42);
if (it != v.end()) { // našiel  }

// 🔃 Triedenie a odstraňovanie duplicít
std::sort(v.begin(), v.end());
v.erase(std::unique(v.begin(), v.end()), v.end());
*/

/*
#include <set>
std::set<std::string> s;
s.insert("hi");             // O(log n)
s.erase("hi");
bool has = s.count("hi");   // 0 alebo 1
for (auto &e : s)
  std::cout << e << "\n";

std::set<int> s;
s.insert(10);
s.erase(10);
s.count(10);
s.find(10);                // iterator alebo s.end()
for (int x : s)
  std::cout << x;

#include <unordered_set>
std::unordered_set<int> us;
us.insert(5);              // priemerne O(1)
us.erase(5);
*/

/*
#include <map>
std::map<std::string,int> m;
m["one"] = 1;               // vkladá alebo prepisuje
m.insert({"two",2});        // vkladá, ak neexistuje
m.erase("two");
auto it = m.find("one");
if (it != m.end()) {
  std::cout<< it->first <<":"<< it->second;
}

std::map<std::string, int> m;
m["a"] = 5;
m.erase("a");
m.at("a");                  // výnimka ak neexistuje
m.count("a");
for (const auto& [key, val] : m) 
  std::cout << key << " -> " << val;

#include <unordered_map>
std::unordered_map<int,std::string> um;
um[3] = "tri";             // priemerne O(1)
*/

/*
#include <algorithm>
std::sort(v.begin(), v.end());
std::reverse(v.begin(), v.end());
std::find(v.begin(), v.end(), 5);
std::count(v.begin(), v.end(), 5);
std::unique(v.begin(), v.end());
std::binary_search(v.begin(), v.end(), 5);
std::lower_bound(v.begin(), v.end(), x);
std::upper_bound(v.begin(), v.end(), x);
auto new_end = std::remove_if(v.begin(), v.end(), [](int x){ return x<0; });
v.erase(new_end, v.end());
*/

/*
void dfs(int u,
         const vector<vector<int>>& adj,
         vector<bool>& vis) {
  if (vis[u]) return;
  vis[u] = true;
  for (int w : adj[u])
    dfs(w, adj, vis);
}

void dfs(const std::string& node, std::set<std::string>& visited, const std::map<std::string, std::vector<std::string>>& graph) {
    if (visited.count(node)) return;
    visited.insert(node);
    for (const auto& neighbor : graph.at(node)) {
        dfs(neighbor, visited, graph);
    }
}
*/

/*
#include <queue>
vector<int> dist(n, -1);
queue<int> q;
q.push(start);
dist[start]=0;
while (!q.empty()) {
  int u = q.front(); q.pop();
  for (int w: adj[u]) {
    if (dist[w]==-1) {
      dist[w] = dist[u]+1;
      q.push(w);
    }
  }
}

void bfs(const std::string& start, const std::map<std::string, std::vector<std::string>>& graph) {
    std::set<std::string> visited;
    std::queue<std::string> q;
    q.push(start);
    while (!q.empty()) {
        std::string node = q.front(); q.pop();
        if (visited.count(node)) continue;
        visited.insert(node);
        for (const auto& neighbor : graph.at(node)) {
            q.push(neighbor);
        }
    }
}
*/
