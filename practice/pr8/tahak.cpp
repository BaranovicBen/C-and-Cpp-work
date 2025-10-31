/*
#include <vector>
using Vec = std::vector<int>;
Vec v;

// pridanie
v.push_back(42);

// prístup
int x = v[0];         // bez kontroly rozsahu
int y = v.at(0);      // s kontrolou (vyhodí out_of_range)

// veľkosť + kontrola
size_t n = v.size();
bool empty = v.empty();

// odstránenie
v.pop_back();         // posledný
v.clear();            // všetko

// iterácia
for (int a : v)       
  std::cout << a;
for (auto it = v.begin(); it != v.end(); ++it)
  std::cout << *it;

// hľadanie
#include <algorithm>
auto it = std::find(v.begin(), v.end(), 42);
if (it != v.end()) { //nasiel }

//  triedenie & uniq
std::sort(v.begin(), v.end());
v.erase(std::unique(v.begin(), v.end()), v.end());

std::vector<int> v;
v.push_back(5);        // pridanie na koniec
v.pop_back();          // odstránenie posledného
int x = v[0];          // priamy prístup
v.size();              // veľkosť
v.clear();             // vymaže všetko
std::find(v.begin(), v.end(), 5); // vyhľadávanie
for (int x : v) std::cout << x;
for (auto it = v.begin(); it != v.end(); ++it) 
std::cout << *it;

 #include <set>
std::set<std::string> s;
s.insert("hi");             // O(log n)
s.erase("hi");
bool has = s.count("hi");   // 0/1
for (auto &e : s)           // automaticky zoradené
  std::cout << e << "\n";

std::set<int> s;
s.insert(10);              // vloženie
s.erase(10);               // odstránenie
s.count(10);               // 0 alebo 1
s.find(10);                // iterator alebo s.end()
for (int x : s) 
std::cout << x;

#include <unordered_set>
std::unordered_set<int> us;
us.insert(5);               // priemer O(1)
us.erase(5);

#include <map>
std::map<std::string,int> m;
m["one"] = 1;               // vkladá alebo prepisuje
m.insert({"two",2});        // vkladá, nenahradí
m.erase("two");
auto it = m.find("one");
if (it != m.end()) {
  std::cout<< it->first <<":"<< it->second;
}
 
std::map<std::string, int> m;
m["a"] = 5;                // pridanie / nastavenie
m.erase("a");              // vymazanie
m.at("a");                 // prístup (výnimka ak neexistuje)
m.count("a");              // 0 alebo 1
for (const auto& [key, val] : m) 
std::cout << key << " -> " << val;

#include <unordered_map>
std::unordered_map<int,std::string> um;
um[3] = "tri";               // priemer O(1)

#include <algorithm>
std::sort(v.begin(), v.end());                   // zoradenie
std::reverse(v.begin(), v.end());                // obráti poradie
std::find(v.begin(), v.end(), 5);                // vyhľadávanie
std::count(v.begin(), v.end(), 5);               // koľkokrát sa vyskytne
std::unique(v.begin(), v.end());                 // odstráni duplicity (po zoradení)
std::binary_search(v.begin(), v.end(), 5);       // binárne vyhľadávanie (nutné zoradené)
std::lower_bound(v.begin(), v.end(), x);         // 1. väčší alebo rovný x

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

#include <stack>
std::stack<int> st;          // LIFO
st.push(1); st.pop(); int top = st.top();

#include <queue>
std::queue<int>  q;          // FIFO
q.push(1); q.pop(); int f = q.front();

#include <queue>
std::priority_queue<int> pq; // max-heap
pq.push(5); pq.top(); pq.pop();

// min-heap:
std::priority_queue<int, std::vector<int>, std::greater<>> minpq;

std::stack<int> s;
s.push(5); 
s.pop(); 
s.top();
std::queue<int> q;
q.push(5); 
q.pop(); 
q.front();
std::priority_queue<int> pq;         // max-heap
std::priority_queue<int, std::vector<int>, std::greater<>> minq; // min-heap

#include <deque>
std::deque<int> d;
d.push_back(1); 
d.push_front(0);
d.pop_back();
d.pop_front();
int first = d.front(), last = d.back();

#include <list>
std::list<int> l;
l.push_back(1);
l.push_front(0);
l.erase(l.begin());         // O(1)
for (int a : l) std::cout<<a;

#include <sstream>

std::string toString(int x) {
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

int toInt(const std::string& s) {
    std::istringstream iss(s);
    int x;
    iss >> x;
    return x;
}

#include <algorithm>

// triedenie
std::sort(v.begin(), v.end());
std::stable_sort(v.begin(), v.end());

// reverz
std::reverse(v.begin(), v.end());

// hľadanie
auto it = std::find(v.begin(), v.end(), val);
bool exists = (std::binary_search(v.begin(), v.end(), val));  // zoradené

// dolná / horná hranica
auto lb = std::lower_bound(v.begin(), v.end(), val);
auto ub = std::upper_bound(v.begin(), v.end(), val);

// unikátne
v.erase(std::unique(v.begin(), v.end()), v.end()); // treba predtým sort

// odstráni/presunie podľa predicate
auto new_end = std::remove_if(v.begin(), v.end(), [](int x){ return x<0; });
v.erase(new_end, v.end());

// akumulácia
#include <numeric>
int sum = std::accumulate(v.begin(), v.end(), 0);

// transformácia
std::transform(v.begin(), v.end(), v.begin(), [](int x){ return x*2; });

struct Point {
  int x,y;
  // porovnanie
  bool operator==(Point const& o) const { return x==o.x && y==o.y; }
  bool operator<( Point const& o) const { return x<o.x || (x==o.x && y<o.y); }
  // výpis
  friend std::ostream& operator<<(std::ostream& os, Point const& p) {
    return os<<"("<<p.x<<","<<p.y<<")";
  }
};

class Foo {
  int* data;
  size_t sz;
public:
  Foo(size_t n): data(new int[n]), sz(n){}
  ~Foo(){ delete[] data; }

  // copy ctor
  Foo(Foo const& o): data(new int[o.sz]), sz(o.sz) {
    std::copy(o.data, o.data+sz, data);
  }
  // copy assign
  Foo& operator=(Foo const& o) {
    if (this==&o) return *this;
    delete[] data;
    sz = o.sz;
    data = new int[sz];
    std::copy(o.data, o.data+sz, data);
    return *this;
  }
}
*/