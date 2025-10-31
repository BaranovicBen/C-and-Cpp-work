#ifndef __TRAINER__
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <string>
#include <stdexcept>
#include <cassert>
#include <queue>
#include <stack>
#endif

typedef std::vector<std::string> strvec;

struct CPkgSys {
    std::map<std::string, std::vector<std::string>> packages;
    std::set<std::string> installed;
    
    bool registerPackage(const std::string &name, const std::vector<std::string> &dependencies) {
        if (packages.count(name)) {
            return false;
        }
        packages[name] = dependencies;
        return true;
    }

    void dfs(const std::string& pkg,std::set<std::string> &visited, std::set<std::string> &needed) const {
            if (!packages.count(pkg)) {
                throw std::runtime_error("Neregistrovaná závislost: " + pkg);
            }
            if (visited.count(pkg)) {
                return;
            }
            visited.insert(pkg);
            for (const auto &dep : packages.at(pkg)) {
                dfs(dep,visited,needed);
            }
            if (!installed.count(pkg)) {
                needed.insert(pkg);
            }
    }

    std::vector<std::string> previewInstall(const std::vector<std::string> &packages) const {
        std::set<std::string> needed;
        std::set<std::string> visited;



        for (const auto &pkg : packages) {
            dfs(pkg,visited,needed);
        }

        return { needed.begin(), needed.end() };
    }

    std::vector<std::string> install(
        const std::vector<std::string> &packages)
    {
        auto toInstall = previewInstall(packages);
        for (const auto &pkg : toInstall) {
            installed.insert(pkg);
        }
        return toInstall;
    }
};


#ifndef __TRAINER__
int main () {
    CPkgSys a;
    assert(a.registerPackage("libc", {}));
    assert(!a.registerPackage("libc", {}));
    assert(a.registerPackage("build-essentials", { "libc", "gcc", "make" }));
    assert(a.registerPackage("make", { "libc" }));
    assert(a.registerPackage("gcc", { "libc" }));
    assert(!a.registerPackage("libc", { "electricity" }));
    assert(a.registerPackage("xviewer", { "libjpeg", "libpng", "x11", "libc" }));
    assert(a.registerPackage("libjpeg", { "libc" }));
    assert(a.registerPackage("libpng", { "zlib", "libc" }));
    assert(a.registerPackage("zlib", { "libc" }));
    assert(a.registerPackage("x11", { "libc", "libframebuffer" }));
    assert(a.registerPackage("libframebuffer", {}));
    assert(a.registerPackage("xed", { "x11", "bison", "libc" }));
    assert(a.registerPackage("gnome", { "x11", "xviewer", "xed" }));
    assert(a.previewInstall({ "libc", "make" }) == strvec({ "libc", "make" }));
    assert(a.previewInstall({ "build-essentials" }) == strvec({ "build-essentials", "gcc", "libc", "make" }));
    assert(a.previewInstall({ "xviewer" }) == strvec({ "libc", "libframebuffer", "libjpeg", "libpng", "x11", "xviewer", "zlib" }));
    assert(a.previewInstall({}) == strvec({}));
    try {
        a.previewInstall({ "gnome" });
        assert(0);
    }
    catch(std::runtime_error & e) {
    }
    assert(a.install({}) == strvec({}));
    assert(a.install({}) == strvec({}));
    assert(a.install({ "libc" }) == strvec({ "libc" }));
    assert(a.previewInstall({ "libc" }) == strvec({}));
    assert(a.install({ "libc" }) == strvec({}));
    assert(a.install({"libpng"}) == strvec({ "libpng", "zlib" }));
    assert(a.previewInstall({ "xviewer" }) == strvec({ "libframebuffer", "libjpeg", "x11", "xviewer" }));
    try {
        a.install({ "xed" });
        assert(0);
    }
    catch(std::runtime_error & e) {
    }
    return 0;
}
#endif