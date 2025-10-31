#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <unordered_set>
#include <stdexcept>
#include <queue>
#include <sstream>
#include <cassert>

using ll = long long;

struct CAnalyzer {
    std::map<std::string, std::vector<std::string>> children;
    std::map<std::string, std::pair<ll,int>> stats;

    CAnalyzer & defCategories(std::istream & is) {
        children.clear();
        stats.clear();
        std::string line;
        std::vector<std::string> stk;
        while (std::getline(is, line)) {
            if (line.empty()) break;
            int spaces = 0;
            while (spaces < (int)line.size() && line[spaces] == ' ')
                ++spaces;
            if (spaces % 2 != 0)
                throw std::invalid_argument("Bad indentation");
            int depth = spaces / 2;
            std::string name = line.substr(spaces);
            if (depth > (int)stk.size()) depth = stk.size();
            stk.resize(depth);
            children[name];
            if (depth > 0) {
                const std::string & parent = stk[depth - 1];
                children[parent].push_back(name);
            }
            stk.push_back(name);
        }
        return *this;
    }

    CAnalyzer & add(const std::string & category, int salary) {
        if (!children.count(category))
            throw std::invalid_argument("Unknown category");
        auto & p = stats[category];
        p.first  += salary;
        p.second += 1;
        return *this;
    }

    double average(const std::list<std::string> & cats) const {
        // 1) existence check
        for (auto const & c : cats)
            if (!children.count(c))
                throw std::invalid_argument("Unknown category");
        // 2) find root branches (ignore categories covered by others)
        std::vector<std::string> vec(cats.begin(), cats.end()), roots;
        for (auto const & c : vec) {
            bool covered = false;
            for (auto const & p : vec) {
                if (p == c) continue;
                if (isAncestor(p, c)) {
                    covered = true;
                    break;
                }
            }
            if (!covered) roots.push_back(c);
        }
        // 3) ensure each root has data
        for (auto const & r : roots) {
            if (subtreeCount(r) == 0)
                throw std::invalid_argument("No data");
        }
        // 4) accumulate all with global visited
        ll sum = 0;
        int cnt = 0;
        std::unordered_set<std::string> visited;
        for (auto const & c : vec)
            accumulateRec(c, sum, cnt, visited);

        return double(sum) / cnt;
    }

    bool isAncestor(const std::string & anc, const std::string & node) const {
        std::queue<std::string> q;
        q.push(anc);
        while (!q.empty()) {
            auto u = q.front(); q.pop();
            for (auto const & v : children.at(u)) {
                if (v == node) return true;
                q.push(v);
            }
        }
        return false;
    }

    int subtreeCount(const std::string & cat) const {
        int cnt = 0;
        std::function<void(const std::string&)> dfs = [&](auto const & u) {
            if (auto it = stats.find(u); it != stats.end())
                cnt += it->second.second;
            for (auto const & v : children.at(u))
                dfs(v);
        };
        dfs(cat);
        return cnt;
    }

    void accumulateRec(const std::string & cat,
                       ll & sum, int & cnt,
                       std::unordered_set<std::string> & visited) const
    {
        if (!visited.insert(cat).second) return;
        if (auto it = stats.find(cat); it != stats.end()) {
            sum += it->second.first;
            cnt += it->second.second;
        }
        for (auto const & sub : children.at(cat))
            accumulateRec(sub, sum, cnt, visited);
    }

    void printChildren(std::ostream & os = std::cout) const {
        for (auto const & kv : children) {
            os << kv.first << ":";
            for (auto const & sub : kv.second)
                os << " " << sub;
            os << "\n";
        }
    }
};

#ifndef __PROGTEST__
int main(){
    std::istringstream iss(
        "EU\n"
        "  Czechia\n"
        "    Prague\n"
        "      Prague 1\n"
        "      Prague 2\n"
        "      Prague 3\n"
        "      Prague 4\n"
        "        Nusle\n"
        "        Pankrac\n"
        "        Branik\n"
        "      Prague 5\n"
        "      Prague 6\n"
        "        Dejvice\n"
        "        Podbana\n"
        "        Vokovice\n"
        "    Brno\n"
        "    Ostrava\n"
        "  Slovakia\n"
        "    Bratislava\n"
        "    Kosice\n"
        "  Germany\n"
        "    West Germany\n"
        "      Frankfurt\n"
        "      Hamburg\n"
        "    East Germany\n"
        "      Berlin\n"
        "      Dresden\n"
        "      Munich\n"
        "USA\n"
        "  California\n"
        "      LA\n"
        "      Sacramento\n"
        "      SF\n"
        "  Texas\n"
        "      Texas\n"
        "  Penslylvania\n"
        "      NewYork\n"
    );

    CAnalyzer x;
    x.defCategories(iss);
    x.printChildren();

    x.add("Dejvice",40000)
     .add("Prague 4",20400)
     .add("Brno",35000)
     .add("Munich",100000)
     .add("Kosice",21000);

    assert(x.average({"Prague"}) == 30200);
    assert(x.average({"Dejvice","Brno","Kosice"}) == 32000);
    assert(x.average({"Prague","Dejvice","Czechia","Vokovice","Ostrava"}) == 31800);
    assert(x.average({"Czechia","Slovakia","Bratislava"}) == 29100);

    try { x.add("Liberec",50000); assert(false && "Missing add exception"); }
    catch (const std::invalid_argument&) {}
    try { x.average({"East Germany","Hamburg"}); assert(false && "Missing average exception (no data)"); }
    catch (const std::invalid_argument&) {}
    try { x.average({"Ostrava"}); assert(false && "Missing average exception (empty subtree)"); }
    catch (const std::invalid_argument&) {}
    try { x.add("Liberec",50000); assert(false && "Missing add exception"); } catch (...) {}

    try { x.average({"NonExist"}); assert(false && "Missing average exception (no valid cats)"); } catch (...) {}

    {
        std::istringstream bad("A\n B\n");
        CAnalyzer tmp;
        try { tmp.defCategories(bad); assert(false && "Missing indentation exception"); }
        catch (const std::invalid_argument&) {}
    }

    {
        std::istringstream ris("Root\n  A\n  B\n  C\n  D\n");
        CAnalyzer randA;
        randA.defCategories(ris);
        for(int i=0; i<100; ++i) {
            randA.add("A", i);
            randA.add("B", i+100);
            randA.add("C", i+200);
            randA.add("D", i+300);
        }
        double exp = (4950 + 14950 + 24950 + 34950) / 400.0; 
        assert(std::abs(randA.average({"Root"}) - exp) < 1e-9);
    }

    {
        std::ostringstream oss;
        oss << "Root\n";
        for(int i=0; i<10000; ++i) oss << "  C" << i << "\n";
        std::istringstream pis(oss.str());
        CAnalyzer perf;
        perf.defCategories(pis);
        for(int i=0; i<10000; ++i) perf.add("C" + std::to_string(i), i);
        auto t1 = std::chrono::high_resolution_clock::now();
        perf.average({"Root"});
        auto t2 = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "Performance: " << ms << " ms\n";
    }

    std::cout << "All tests passed" << std::endl;
    return 0;
}
#endif