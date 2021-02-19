#include <algorithm>
#include <iostream>
#include <limits.h>
#include <string>
using namespace std;

inline int digits(long long);
inline string intToStr(long long);

class bigNum {
  string _number;
  int _sign;

public:
  bigNum(long long num = 0L) {
    if ((num >= LONG_MIN) and (num <= LONG_MAX)) {
      string sNum = intToStr(num);
      if (sNum[0] != '-') {
        this->_number = sNum;
        this->_sign = 1;
      } else {
        this->_number = string(sNum.begin() + 1, sNum.end());
        this->_sign = -1;
      }
    } else {
      this->_number = '0';
      this->_sign = 1;
    }
  }

  bigNum(string num) {
    if (num[0] != '-') {
      this->_number = num;
      this->_sign = 1;
    } else {
      this->_number = string(num.begin() + 1, num.end());
      this->_sign = -1;
    }
  }

  /* Less than and greater than */
  inline bool operator<(const bigNum &b2) const {
    int a, b;
    if (this->_sign != b2._sign)
      return this->_sign < b2._sign;

    if (this->_number.size() == b2._number.size()) {
      for (size_t i = 0; i < this->_number.size(); i++) {
        a = this->_number[i] - '0';
        b = b2._number[i] - '0';
        if (a != b) {
          if (this->_sign == -1)
            return a > b;
          return a < b;
        }
      }
    }

    return (this->_sign == -1) ? this->_number.size() > b2._number.size()
                               : this->_number.size() < b2._number.size();
  }

  inline bool operator>(const bigNum &rhs) const { return rhs < *this; }
  inline bool operator<=(const bigNum &rhs) const { return !(*this > rhs); }
  inline bool operator>=(const bigNum &rhs) const { return !(*this < rhs); }

  /* */
  int toInt() const {
    int result = 0;
    int mult = 1;
    if ((*this >= INT_MIN) and (*this <= INT_MAX)) {
      for (int i = this->_number.size() - 1; i >= 0; i--) {
        result += (this->_number[i] - '0') * mult;
        mult *= 10;
      }
    } else
      result = -1;

    return result;
  }

  /* */
  long long toLongLong() const {
    long long result = 0L;
    long long mult = 1L;
    if ((*this >= LLONG_MIN) and (*this <= LLONG_MAX)) {
      for (long long i = this->_number.size() - 1; i >= 0; i--) {
        result += (this->_number[i] - '0') * mult;
        mult *= 10L;
      }
    } else
      result = -1;

    return result;
  }

  /* */
  bigNum fillWithZeros(int c) {
    string newBigNum = this->_number;
    int Zeros = c - this->_number.size();
    if (Zeros > 0)
      newBigNum = string(Zeros, '0') + this->_number;

    if (this->_sign == -1)
      newBigNum = '-' + newBigNum;
    return bigNum(newBigNum);
  }

  bigNum removeZeros() {
    size_t i = 0;
    string newBigNum = "";
    while (i < this->_number.size() && this->_number[i] <= '0')
      i++;

    for (; i < this->_number.size(); i++)
      newBigNum += this->_number[i];

    if (this->_sign == -1)
      newBigNum = '-' + newBigNum;
    return bigNum(newBigNum);
  }

  bigNum divide(bigNum n) {
    bigNum Q, R;
    n += n == 0;

    if (n < 0) {
      Q = this->divide(-n);
      return -Q;
    }
    if (*this < 0) {
      Q = -this->divide(n);
      if (R == 0)
        return -Q;
      else
        return -Q - 1;
    }
    Q = 0;
    R = *this;
    while (R >= n) {
      Q = Q + 1;
      R = R - n;
    }
    return Q;
  }

  bigNum modulus(bigNum n) {
    bigNum Q, R;
    n += n == 0;

    if (n < 0) {
      R = this->modulus(-n);
      return -R;
    }
    if (*this < 0) {
      R = -this->modulus(n);
      if (R == 0)
        return 0;
      else
        return n - R;
    }
    Q = 0;
    R = *this;
    while (R >= n) {
      Q = Q + 1;
      R = R - n;
    }
    return R;
  }

  bigNum add(bigNum n) {
    if (this->_sign != n._sign)
      return this->subtract(-n);

    int maxZeros = max(this->_number.size(), n._number.size()), ac = 0;
    bigNum bA = this->fillWithZeros(maxZeros);
    bigNum bB = n.fillWithZeros(maxZeros);
    string newBigNum = "";
    char buff[3];
    int a, b, s;

    for (int i = maxZeros - 1; i >= 0; i--) {
      a = bA._number[i] - '0';
      b = bB._number[i] - '0';
      s = ac + a + b;
      ac = s / 10;

      sprintf(buff, "%d", s % 10);
      newBigNum = string(buff) + newBigNum;
    }

    if (ac > 0) {
      sprintf(buff, "%d", ac);
      newBigNum = string(buff) + newBigNum;
    }

    if (this->_sign == -1)
      newBigNum = '-' + newBigNum;
    return bigNum(newBigNum);
  }

  bigNum subtract(bigNum n) {
    if (this->_sign != n._sign)
      return this->add(-n);

    char buff[3];
    string newBigNum = "";
    int change = 1, ac = 0, numA, numB, i;
    int maxC = max(this->_number.size(), n._number.size());

    bigNum a, b;
    if (*this < n) {
      a = n.fillWithZeros(maxC), b = this->fillWithZeros(maxC);
      change = -1;
    } else {
      a = this->fillWithZeros(maxC), b = n.fillWithZeros(maxC);
      change = 1;
    }

    for (i = b._number.size() - 1; i >= 0; i--) {
      numA = (a._number[i] - '0') + ac;
      numB = b._number[i] - '0';
      if (numB > numA)
        ac = -1, numA += 10;
      else
        ac = 0;

      sprintf(buff, "%d", numA - numB);
      newBigNum = string(buff) + newBigNum;
    }

    if ((this->_sign * change) == -1)
      newBigNum = '-' + newBigNum;
    return bigNum(newBigNum).removeZeros();
  }

  bigNum multiply(bigNum bigNumB) {
    size_t bigNumBSize = bigNumB._number.size();
    size_t thisSize = this->_number.size();

    if (thisSize <= 2 and bigNumBSize <= 2) {
      return bigNum(this->toInt() * bigNumB.toInt());
    }

    // Ceil of max size / 2
    size_t half = (max(bigNumBSize, thisSize) + 1) >> 1;

    bigNum a, b, c, d;
    if (thisSize >= half) {
      a = bigNum(this->_number.substr(0, thisSize - half));
      b = bigNum(this->_number.substr(thisSize - half));
    } else {
      b = bigNum(*this);
    }

    if (bigNumBSize >= half) {
      c = bigNum(bigNumB._number.substr(0, bigNumBSize - half));
      d = bigNum(bigNumB._number.substr(bigNumBSize - half));
    } else {
      d = bigNum(bigNumB);
    }

    bigNum ac = (a * c);
    bigNum bd = (b * d);
    bigNum next = ((a + b) * (c + d)) - ac - bd;

    string extraZero(half, '0');

    return (bigNum(ac._number + extraZero + extraZero) +
            bigNum(next._number + extraZero) + bd)
        .removeZeros();
  }

  /* Increment */
  bigNum &operator++() {
    *this = this->add(bigNum(1));
    return *this;
  }

  bigNum operator++(int) {
    bigNum newBigNum(*this);
    operator++();
    return newBigNum;
  }

  /* Unary minus */
  bigNum operator-() const {
    bigNum num(*this);
    num._sign = -1 * this->_sign;
    return num;
  }

  /* Equality and inequality */
  inline bool operator==(const bigNum &b2) const {
    bigNum a = bigNum(*this).removeZeros();
    bigNum b = bigNum(b2).removeZeros();
    return !(a._number.compare(b._number)) && (a._sign == b._sign);
  }

  inline bool operator!=(const bigNum &b2) const { return !(*this == b2); }

  /* Add operator */
  bigNum &operator+=(const bigNum &n) {
    *this = this->add(n);
    return *this;
  }

  friend bigNum operator+(bigNum lhs, const bigNum &rhs) {
    lhs += rhs;
    return lhs;
  }

  /* Subtract operator */
  bigNum &operator-=(const bigNum &n) {
    *this = this->subtract(n);
    return *this;
  }

  friend bigNum operator-(bigNum lhs, const bigNum &rhs) {
    lhs -= rhs;
    return lhs;
  }

  /* Multiply operator */
  bigNum &operator*=(const bigNum &n) {
    *this = this->multiply(n);
    return *this;
  }

  friend bigNum operator*(bigNum lhs, const bigNum &rhs) {
    lhs *= rhs;
    return lhs;
  }

  /* Modulus operator */
  bigNum &operator%=(const bigNum &n) {
    *this = this->modulus(n);
    return *this;
  }

  friend bigNum operator%(bigNum lhs, const bigNum &rhs) {
    lhs %= rhs;
    return lhs;
  }

  /* Divide operator */
  bigNum &operator/=(const bigNum &n) {
    *this = this->divide(n);
    return *this;
  }

  friend bigNum operator/(bigNum lhs, const bigNum &rhs) {
    lhs /= rhs;
    return lhs;
  }

  /* cout overload */
  friend ostream &operator<<(ostream &os, const bigNum &num) {
    if (num._sign == -1)
      os << "-";
    os << ((num._number.size() > 0) ? num._number : "0");
    return os;
  }

  friend istream &operator>>(istream &is, bigNum &num) {
    string s;
    is >> s;
    num = bigNum(s);
    return is;
  }
};

inline int digits(long long i) {
  unsigned long long x = abs(i);
  if (x < 10L)
    return 1;
  else if (x < 100L)
    return 2;
  else if (x < 1000L)
    return 3;
  else if (x < 10000L)
    return 4;
  else if (x < 100000L)
    return 5;
  else if (x < 1000000L)
    return 6;
  else if (x < 10000000L)
    return 7;
  else if (x < 100000000L)
    return 8;
  else if (x < 1000000000L)
    return 9;
  else if (x < 10000000000L)
    return 10;
  else if (x < 100000000000L)
    return 11;
  else if (x < 1000000000000L)
    return 12;
  else if (x < 10000000000000L)
    return 13;
  else if (x < 100000000000000L)
    return 14;
  else if (x < 1000000000000000L)
    return 15;
  else if (x < 10000000000000000L)
    return 16;
  else if (x < 100000000000000000L)
    return 17;
  else if (x < 1000000000000000000L)
    return 18;
  return 19;
}

inline string intToStr(long long x) {
  string num;
  int neg = (x < 0);
  int dig = digits(x);
  if (neg)
    dig++;
  unsigned long long i = abs(x);
  num = string(dig, '0');

  while (i) {
    num[--dig] += (i % 10);
    i /= 10;
  }
  if (neg)
    num[0] = '-';
  return num;
}
