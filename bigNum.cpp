#include <iostream>
using namespace std;

class bigNum {
  private:
    string _number;
    int _sign;

  public:
    bigNum(int num = 0) {
      char buff[25];
      sprintf(buff, "%d", num);
      if (num >= 0) {
        this->_number = string(buff);
        this->_sign = 1;
      }
      else {
        this->_number = string(&buff[1]);
        this->_sign = -1; 
      }
    }

    bigNum(string num) {
      if (num[0] != '-') {
        this->_number = num;
        this->_sign = 1;
      }
      else {
        this->_number = string(num.begin() + 1, num.end());
        this->_sign = -1;
      }
    }

    int toInt() const {
      int result = 0;
      const bigNum minI = bigNum("-32767");
      const bigNum maxI = bigNum("32767");
      int mult = 1;
      if ((*this > minI) and (*this < maxI)) {
        for (int i = this->_number.size() - 1; i >= 0; i--) {
          result += (this->_number[i] - '0') * mult;
          mult *= 10;
        }
      }
      else result = -1;

      return result;
    }    

    long long toLongLong() const {
      long long result = 0;
      const bigNum minLL = bigNum("-9223372036854775807");
      const bigNum maxLL = bigNum("9223372036854775807");
      long long mult = 1;
      if ((*this > minLL) and (*this < maxLL)) {
        for (long long i = this->_number.size() - 1; i >= 0; i--) {
          result += (this->_number[i] - '0') * mult;
          mult *= 10;
        }
      }
      else result = -1;

      return result;
    }

    bigNum fillWithZeros(int c) {
      string newBigNum = this->_number;
      int Zeros = c - this->_number.size();
      if (Zeros > 0)
        newBigNum = string(Zeros, '0') + this->_number;

      if (this->_sign == -1) newBigNum = '-' + newBigNum;
      return bigNum(newBigNum);
    }

    bigNum removeZeros() {
      size_t i = 0;
      string newBigNum = "";
      while (i < this->_number.size() && this->_number[i] <= '0')
        i++;

      for (; i < this->_number.size(); i++)
        newBigNum += this->_number[i];

      if (this->_sign == -1) newBigNum = '-' + newBigNum;
      return bigNum(newBigNum);
    }

    bigNum add(bigNum n) {
      if (this->_sign != n._sign) return this->subtract(-n);

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

      if (this->_sign == -1) newBigNum = '-' + newBigNum;
      return bigNum(newBigNum);
    }

    bigNum subtract(bigNum n) {
      if (this->_sign != n._sign) return this->add(-n);

      char buff[3];
      string newBigNum = "";
      int change = 1, ac = 0, numA, numB, i;
      int maxC = max(this->_number.size(), n._number.size());

      bigNum a, b;
      if (*this < n) {
        a = n.fillWithZeros(maxC), b = this->fillWithZeros(maxC);
        change = -1;
      }
      else {
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

      if ((this->_sign * change) == -1) newBigNum = '-' + newBigNum;
      return bigNum(newBigNum).removeZeros();
    }

    bigNum multiply(bigNum n) {
      char buff[3];
      string Zeros = "";
      int numA, numB, ac, m;
      bigNum a = *this, b = n, newBigNum;

      if (*this > n) 
        a = n, b = *this;

      string toAdd[a._number.size()];

      for (int i = a._number.size() - 1; i >= 0; i--) {
        numA = a._number[i] - '0', ac = 0;
        for (int j = b._number.size() - 1; j >= 0; j--) {
          numB = b._number[j] - '0';
          m = (numA * numB) + ac;
          sprintf(buff, "%d", m % 10);
          toAdd[i] = string(buff) + toAdd[i];
          ac = m / 10;
        }

        if (ac > 0) {
          sprintf(buff, "%d", ac);
          toAdd[i] = string(buff) + toAdd[i];
        }
        toAdd[i] += Zeros;
        Zeros += "0";
      }

      for (size_t i = 0; i < a._number.size(); i++)
        newBigNum = newBigNum.add(bigNum(toAdd[i]));

      newBigNum._sign = this->_sign * n._sign;
      return newBigNum;
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

    /* Less than and greater than */
    inline bool operator<(const bigNum &b2) const {
      int a, b;
      if (this->_sign != b2._sign) return this->_sign < b2._sign;

      if (this->_number.size() == b2._number.size()) {
        for (size_t i = 0; i < this->_number.size(); i++) {
          a = this->_number[i] - '0';
          b = b2._number[i] - '0';
          if (a != b) {
            if (this->_sign == -1) return a > b;
            return a < b;
          }
        }
      }

      return (this->_sign == -1) ? this->_number.size() > b2._number.size() : this->_number.size() < b2._number.size();
    }

    inline bool operator>(const bigNum &rhs) const { return rhs < *this; }
    inline bool operator<=(const bigNum &rhs) const { return !(*this > rhs); }
    inline bool operator>=(const bigNum &rhs) const { return !(*this < rhs); }

    /* Equality and inequality */
    inline bool operator==(const bigNum &b2) const {
      bigNum a = bigNum(*this).removeZeros();
      bigNum b = bigNum(b2).removeZeros();
      return !(a._number.compare(b._number)) && (a._sign == b._sign);
    }

    inline bool operator!=(const bigNum &b2) const {
      return !(*this == b2);
    }

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

    /* cout overload */
    friend ostream &operator<<(ostream& os, const bigNum &num) {
      if (num._sign == -1) os << "-";
      os << ((num._number.size() > 0) ? num._number : "0");
      return os;    
    } 
};