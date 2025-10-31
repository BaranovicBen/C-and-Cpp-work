#include <iostream>    
#include <vector>     
#include <string>  
#include <sstream>     
#include <cmath>    
#include <limits>       

bool smallDiff(double a, double b) {
  if (a == 0.0 && b == 0.0)
    return true;

  double tolerance = std::abs(b) * 1000 * std::numeric_limits<double>::epsilon();
  return std::abs(a - b) <= tolerance;
}


struct CPolynomial{
  std::vector<double> polynom;

  CPolynomial(){
    polynom.push_back(0.0);
  }
  CPolynomial(const std::initializer_list<double> vals){
    polynom.assign(vals.begin(), vals.end());
  }
  ~CPolynomial() = default;

  CPolynomial operator=(const CPolynomial& other){
      if(this != &other){
        polynom = other.polynom;
      }
      return *this;
  };

  CPolynomial operator*(double scalar) const {
    CPolynomial result = *this;
    for (size_t i = 0; i < result.polynom.size(); ++i) {
      result.polynom[i] *= scalar;
    }
    return result;
  };
  

  CPolynomial& operator*=(double scalar) {
    *this = *this * scalar;
    return *this;
  };
  

  CPolynomial operator*(const CPolynomial& other){
    CPolynomial result;
    size_t resultSize = polynom.size() + other.polynom.size() - 1;
    result.polynom.assign(resultSize,0.0);

    for(int i = 0; i < polynom.size(); i++){
      for(int j = 0; j < other.polynom.size(); j++){
        result.polynom[i+j] += polynom[i] * other.polynom[j];
      }
    }
    return result;
  };

  CPolynomial operator*=(const CPolynomial& other){
    *this = *this * other;
    return *this;
  };

  bool operator==(const CPolynomial& other) const {
      size_t lenA = polynom.size();
      size_t lenB = other.polynom.size();
      const double EPS = std::numeric_limits<double>::epsilon();

      // Trim trailing near-zero coefficients
      while (lenA > 1 && std::abs(polynom[lenA - 1]) < EPS)
          --lenA;
      while (lenB > 1 && std::abs(other.polynom[lenB - 1]) < EPS)
          --lenB;

      if (lenA != lenB)
          return false;

      for (size_t i = 0; i < lenA; ++i) {
          if (!smallDiff(polynom[i], other.polynom[i]))
              return false;
      }

      return true;
  }


  bool operator!=(const CPolynomial& other) const {
    return !(*this == other);
  };

  double operator()(double value) const {
    long result = 0.0;
    for(size_t i = 0; i < polynom.size();i++){
      result += polynom[i]*pow(value,i);
    }
    return result;
  };

  int degree() const {
    if (polynom.empty()) return 0;
  
    for (int i = polynom.size() - 1; i >= 0; --i) {
      if (std::abs(polynom[i]) > std::numeric_limits<double>::epsilon())
        return i;
    }
    return 0;
  }
  
  bool operator!() const {
    const double EPS = std::numeric_limits<double>::epsilon();
    for (double coef : polynom) {
      if (std::abs(coef) > EPS)
        return false;
    }
    return true;
  };
  
  explicit operator bool() const {
    return !(!*this );
  };

  double operator[](size_t index) const {
    if(index >= polynom.size())
      return 0.0;
    return polynom[index];

  };

  double& operator[](size_t index) {
    if(index >= polynom.size())
      polynom.resize(index+1, 0.0);
    return polynom[index];
  };
};

std::ostream& operator<<(std::ostream& stream, const CPolynomial& pol) {
  bool first = true;
  const double EPS = std::numeric_limits<double>::epsilon();

  for (int i = pol.polynom.size() - 1; i >= 0; --i) {
      double val = pol.polynom[i];

      if (std::abs(val) < EPS)
          continue;

      if (!first) {
          stream << (val < 0 ? " - " : " + ");
      } else if (val < 0) {
          stream << "- ";
      }

      double absval = std::abs(val);

      if (i == 0 || std::abs(val) != 1.0)
          stream << std::abs(val);

      if (i >= 1) {
        if (absval != 1.0)
            stream << "*";
        stream << "x";
        if (i >= 1)
            stream << "^" << i;
      }

      first = false;
  }

  if (first)
      stream << "0";

  return stream;
}

bool dumpMatch(const CPolynomial& x, const std::vector<double>& ref) {
  size_t lenA = x.polynom.size();
  size_t lenB = ref.size();

  while (lenA > 1 && std::abs(x.polynom[lenA - 1]) < std::numeric_limits<double>::epsilon())
    --lenA;
  while (lenB > 1 && std::abs(ref[lenB - 1]) < std::numeric_limits<double>::epsilon())
    --lenB;

  if (lenA != lenB) {
    return false;
  }
  for (size_t i = 0; i < lenA; ++i) {
    if (!smallDiff(x.polynom[i], ref[i])) {
      return false;
    }
  }
  return true;
}

int main () {
  CPolynomial a, b, c;
  std::ostringstream out, tmp;

  // Setup a
  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;

  std::cout << "\n== Polynomial a setup ==\n";
  std::cout << "a(x=2) = " << a(2) << " (expected: 5)\n";
  std::cout << "a = " << a << " (expected: x^3 + 3.5*x^1 - 10)\n";

  // Multiply a by -2
  c = a * -2;
  std::vector<double> expected1{ 20.0, -7.0, -0.0, -2.0 };
  std::cout << "\n== c = a * -2 ==\n";
  std::cout << "c = " << c << "\n";
  std::cout << "c.degree() = " << c.degree() << " (expected: 3)\n";
  std::cout << "dumpMatch: " << std::boolalpha << dumpMatch(c, expected1) << "\n";

  // Print empty b
  std::cout << "\n== Polynomial b (empty) ==\n";
  std::cout << "b = " << b << " (expected: 0)\n";

  // Setup b
  b[5] = -1;
  b[2] = 3;
  std::cout << "\n== Polynomial b modified ==\n";
  std::cout << "b = " << b << " (expected: - x^5 + 3*x^2)\n";

  // Multiply a * b
  c = a * b;
  std::vector<double> expected2{ -0.0, -0.0, -30.0, 10.5, -0.0, 13.0, -3.5, 0.0, -1.0 };
  std::cout << "\n== c = a * b ==\n";
  std::cout << "c = " << c << "\n";
  std::cout << "c.degree() = " << c.degree() << " (expected: 8)\n";
  std::cout << "dumpMatch: " << dumpMatch(c, expected2) << "\n";

  // Scale a *= 5
  a *= 5;
  std::vector<double> expected3{ -50.0, 17.5, 0.0, 5.0 };
  std::cout << "\n== a *= 5 ==\n";
  std::cout << "a = " << a << "\n";
  std::cout << "a.degree() = " << a.degree() << " (expected: 3)\n";
  std::cout << "dumpMatch: " << dumpMatch(a, expected3) << "\n";

  // a *= b
  a *= b;
  std::vector<double> expected4{ 0.0, 0.0, -150.0, 52.5, -0.0, 65.0, -17.5, -0.0, -5.0 };
  std::cout << "\n== a *= b ==\n";
  std::cout << "a = " << a << "\n";
  std::cout << "a.degree() = " << a.degree() << " (expected: 8)\n";
  std::cout << "dumpMatch: " << dumpMatch(a, expected4) << "\n";

  // Logical checks
  std::cout << "\n== Logical Comparisons ==\n";
  std::cout << "a != b: " << (a != b) << " (expected: true)\n";
  b[5] = 0;
  std::cout << "b[5] = 0 -> b = " << b << "\n";
  std::cout << "static_cast<bool>(b): " << static_cast<bool>(b) << " (expected: true)\n";
  b[2] = 0;
  std::cout << "b[2] = 0 -> b = " << b << "\n";
  std::cout << "a == b: " << (a == b) << " (expected: false)\n";

  // Zero out a
  a *= 0;
  std::vector<double> expected5{ 0.0 };
  std::cout << "\n== a *= 0 ==\n";
  std::cout << "a = " << a << "\n";
  std::cout << "a internal: ";
for (auto val : a.polynom) std::cout << "[" << val << "] ";
std::cout << "\nb internal: ";
for (auto val : b.polynom) std::cout << "[" << val << "] ";
std::cout << std::endl;

  std::cout << "a.degree() = " << a.degree() << " (expected: 0)\n";
  std::cout << "dumpMatch: " << dumpMatch(a, expected5) << "\n";
  std::cout << "a == b: " << (a == b) << " (expected: true)\n";
}

//"- x^8 - 3.5*x^6 + 13*x^5 + 10.5*x^3 - 30*x^2"
//std::vector<double>{ -0.0, -0.0, -30.0, 10.5, -0.0, 13.0, -3.5, 0.0, -1.0 }

// int degree() const {
//   int i = polynom.size() - 1;
//   while (i > 0 && std::abs(polynom[i]) < std::numeric_limits<double>::epsilon())
//     --i;
//   return i;
// }
// };