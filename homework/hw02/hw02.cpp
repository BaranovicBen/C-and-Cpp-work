#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cassert>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <span>
#include <algorithm>
#include <memory>
#include <compare>
#include <complex>
#include <limits>  
#endif /* __PROGTEST__ */

std::ios_base & ( * poly_var ( const std::string & name ) ) ( std::ios_base & x )
{
  return [] ( std::ios_base & ios ) -> std::ios_base & { return ios; };
}


/**
 * Utility function for comparing floating point numbers with a tolerance.
 * Used to compare polynomial coefficients while accounting for floating point imprecision.
 */
bool smallDiff(double a, double b) {
  if (a == 0.0 && b == 0.0)
    return true;

  double tolerance = std::abs(b) * 1000 * std::numeric_limits<double>::epsilon();
  return std::abs(a - b) <= tolerance;
}
/**
 * CPolynomial - Represents a mathematical polynomial with real-number coefficients.
 * Encapsulates all related operations like evaluation, multiplication, comparison, and output formatting.
 */
struct CPolynomial {
  std::vector<double> polynom;  // Stores coefficients by index = degree of term

  /** Default constructor initializes zero polynomial */
  CPolynomial() {
    polynom.push_back(0.0);
  }

  /** Initializer list constructor to allow syntax like CPolynomial p({1, 2, 3}) */
  CPolynomial(const std::initializer_list<double> vals) {
    polynom.assign(vals.begin(), vals.end());
  }

  /** Default destructor - nothing dynamic to clean */
  ~CPolynomial() = default;

  /** Copy assignment operator - deep copy of vector */
  CPolynomial operator=(const CPolynomial& other) {
    if (this != &other) {
      polynom = other.polynom;
    }
    return *this;
  };

  /**
   * Scalar multiplication (non-modifying).
   * Returns a new polynomial multiplied by a scalar value.
   */
  CPolynomial operator*(double scalar) const {
    CPolynomial result = *this;
    for (size_t i = 0; i < result.polynom.size(); ++i) {
      result.polynom[i] *= scalar;
    }
    return result;
  };

  /**
   * Scalar multiplication (modifying).
   */
  CPolynomial& operator*=(double scalar) {
    *this = *this * scalar;
    return *this;
  };

  /**
   * Polynomial multiplication.
   * Returns the product of this polynomial and another.
   */
  CPolynomial operator*(const CPolynomial& other) const {
    CPolynomial result;
    size_t resultSize = polynom.size() + other.polynom.size() - 1;
    result.polynom.assign(resultSize, 0.0);

    for (size_t i = 0; i < polynom.size(); i++) {
      for (size_t j = 0; j < other.polynom.size(); j++) {
        result.polynom[i + j] += polynom[i] * other.polynom[j];
      }
    }
    return result;
  };

  /** Polynomial multiplication */
  CPolynomial operator*=(const CPolynomial& other) {
    *this = *this * other;
    return *this;
  };

  /**
   * Equality operator.
   * Compares two polynomials for value equality (ignoring trailing zeros).
   */
  bool operator==(const CPolynomial& other) const {
    size_t lenA = polynom.size();
    size_t lenB = other.polynom.size();
    const double EPS = std::numeric_limits<double>::epsilon();

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

  /** Inequality operator - uses inverse of operator== */
  bool operator!=(const CPolynomial& other) const {
    return !(*this == other);
  };

  /**
   * Evaluation operator.
   * Returns the value of the polynomial for a given input.
   */
  double operator()(double value) const {
    double result = 0.0;
    for (size_t i = 0; i < polynom.size(); i++) {
      result += polynom[i] * pow(value, i);
    }
    return result;
  };

  /**
   * Degree function.
   * Returns the highest non-zero degree in the polynomial.
   */
  int degree() const {
    if (polynom.empty()) return 0;
    for (int i = polynom.size() - 1; i >= 0; --i) {
      if (std::abs(polynom[i]) > std::numeric_limits<double>::epsilon())
        return i;
    }
    return 0;
  }

  /**
   * Logical NOT operator.
   * Returns true if the polynomial is zero (all coefficients are effectively zero).
   */
  bool operator!() const {
    const double EPS = std::numeric_limits<double>::epsilon();
    for (double coef : polynom) {
      if (std::abs(coef) > EPS)
        return false;
    }
    return true;
  };

  /** Bool cast operator.
   *  Returns true if the polynomial is non-zero.
   */
  explicit operator bool() const {
    return !(!*this);
  };

  /**
   * Const index operator - read-only access to coefficient at given degree.
   * Returns 0.0 if index is out of bounds.
   */
  double operator[](size_t index) const {
    if (index >= polynom.size())
      return 0.0;
    return polynom[index];
  };

  /**
   * Non-const index operator - allows writing to coefficient.
   * Automatically resizes vector to accommodate the index.
   */
  double& operator[](size_t index) {
    if (index >= polynom.size())
      polynom.resize(index + 1, 0.0);
    return polynom[index];
  };
};

/**
 * Overloaded << operator for printing polynomials in a compact readable form.
 * Prints from highest degree down, omits zero coefficients, formats signs and coefficients nicely.
 */
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
      stream << absval;

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

#ifndef __PROGTEST__

/**
 * Compares the internal state of a polynomial with a reference vector.
 * Trims trailing zeros and uses smallDiff() to compare each coefficient.
 */
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

int main ()
{
  CPolynomial a, b, c;
  std::ostringstream out, tmp;

  a[0] = -10;
  a[1] = 3.5;
  a[3] = 1;
  assert ( smallDiff ( a ( 2 ), 5 ) );
  out . str ("");
  out << a;
  assert ( out . str () == "x^3 + 3.5*x^1 - 10" );
  c = a * -2;
  assert ( c . degree () == 3
           && dumpMatch ( c, std::vector<double>{ 20.0, -7.0, -0.0, -2.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- 2*x^3 - 7*x^1 + 20" );
  out . str ("");
  out << b;
  assert ( out . str () == "0" );
  b[5] = -1;
  b[2] = 3;
  out . str ("");
  out << b;
  assert ( out . str () == "- x^5 + 3*x^2" );
  c = a * b;
  assert ( c . degree () == 8
           && dumpMatch ( c, std::vector<double>{ -0.0, -0.0, -30.0, 10.5, -0.0, 13.0, -3.5, 0.0, -1.0 } ) );

  out . str ("");
  out << c;
  assert ( out . str () == "- x^8 - 3.5*x^6 + 13*x^5 + 10.5*x^3 - 30*x^2" );
  a *= 5;
  assert ( a . degree () == 3
           && dumpMatch ( a, std::vector<double>{ -50.0, 17.5, 0.0, 5.0 } ) );

  a *= b;
  assert ( a . degree () == 8
           && dumpMatch ( a, std::vector<double>{ 0.0, 0.0, -150.0, 52.5, -0.0, 65.0, -17.5, -0.0, -5.0 } ) );

  assert ( a != b );
  b[5] = 0;
  assert ( static_cast<bool> ( b ) );
  assert ( ! ! b );
  b[2] = 0;
  assert ( !(a == b) );
  a *= 0;
  assert ( a . degree () == 0
           && dumpMatch ( a, std::vector<double>{ 0.0 } ) );

  assert ( a == b );
  assert ( ! static_cast<bool> ( b ) );
  assert ( ! b );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */