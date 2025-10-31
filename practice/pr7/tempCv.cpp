#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <cassert>

struct Complex {
    double rat, irt;
    Complex(double a = 0.0, double b = 0.0) : rat(a), irt(b) {}

    Complex& operator+=(Complex const& o) {
        rat += o.rat; irt += o.irt; return *this;
    }
    Complex& operator-=(Complex const& o) {
        rat -= o.rat; irt -= o.irt; return *this;
    }
    Complex& operator*=(Complex const& o) {
        double r = rat*o.rat - irt*o.irt;
        double i = rat*o.irt + irt*o.rat;
        rat = r; irt = i; return *this;
    }
    Complex& operator/=(Complex const& o) {
        double denom = o.rat*o.rat + o.irt*o.irt;
        double r = (rat*o.rat + irt*o.irt)/denom;
        double i = (irt*o.rat - rat*o.irt)/denom;
        rat = r; irt = i; return *this;
    }
    Complex& operator*=(double x) { rat*=x; irt*=x; return *this; }
    Complex& operator/=(double x) { rat/=x; irt/=x; return *this; }
};
inline Complex operator+(Complex a, Complex const& b){ a+=b; return a; }
inline Complex operator-(Complex a, Complex const& b){ a-=b; return a; }
inline Complex operator*(Complex a, Complex const& b){ a*=b; return a; }
inline Complex operator/(Complex a, Complex const& b){ a/=b; return a; }
inline Complex operator-(Complex c){ c.rat=-c.rat; c.irt=-c.irt; return c; }
inline std::ostream& operator<<(std::ostream& os, Complex const& c){
    os<<"["<<c.rat<<", "<<c.irt<<"]"; return os;
}
inline double abs(Complex const& c){
    return std::sqrt(c.rat*c.rat + c.irt*c.irt);
}
inline Complex sqrt(Complex const& c){
    double r = abs(c);
    double th = std::atan2(c.irt, c.rat);
    double sr = std::sqrt(r);
    return Complex(sr*std::cos(th/2), sr*std::sin(th/2));
}


void solve(std::ostream& os, Complex const& a,
                           Complex const& b,
                           Complex const& c)
{
    if (a.rat==0.0 && a.irt==0.0) {
        if (b.rat==0.0 && b.irt==0.0) {
            os<<"Nejednoznacne nebo zadne reseni\n";
        } else {
            Complex x = -c/b;
            os<<x<<"\n";
        }
        return;
    }
    Complex D = b*b - Complex(4,0)*a*c;
    Complex sd = sqrt(D);
    Complex two_a = Complex(2,0)*a;
    Complex x1 = (-b + sd)/two_a;
    Complex x2 = (-b - sd)/two_a;
    if (std::abs(abs(D)) < 1e-12) {
        os<<x1<<"\n";
    } else {
        os<<x1<<"\n"<<x2<<"\n";
    }
}

void solve(double a, double b, double c, std::ostream& out) {
    std::ostringstream tmp;
    solve(tmp, Complex(a,0), Complex(b,0), Complex(c,0));
    std::vector<std::string> lines;
    std::istringstream  iss(tmp.str());
    std::string         line;
    while (std::getline(iss, line)) {
        if (!line.empty()) lines.push_back(line);
    }
    if (lines.size() == 1) {
        out << "Pouze jedno reseni: " << lines[0];
    }
    else if (lines.size() == 2) {
        out << "Dve reseni: " << lines[0] << " a " << lines[1];
    }
    else {
        out << "Nema zadne reseni.";
    }
}

int main () {
    {
        std::ostringstream oss;
        solve(1, 4, 4, oss);
        assert(oss.str() == "Pouze jedno reseni: [-2, 0]");
    }
    {
        std::ostringstream oss;
        solve(1, 0, -4, oss);
        assert(oss.str() == "Dve reseni: [-2, 0] a [2, 0]" || oss.str() == "Dve reseni: [2, 0] a [-2, 0]");
    }
    {
        std::ostringstream oss;
        solve(1, -4, 8, oss);
        assert(oss.str() == "Dve reseni: [2, 2] a [2, -2]" || oss.str() == "Dve reseni: [2, -2] a [2, 2]");
    }
    {
        std::ostringstream oss;
        solve(1, 1, 4, oss);
        assert(oss.str() == "Dve reseni: [-0.5, -1.93649] a [-0.5, 1.93649]" || oss.str() == "Dve reseni: [-0.5, 1.93649] a [-0.5, -1.93649]");
    }
    return 0;
}