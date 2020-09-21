#include <vector>
#include <cstdint>
#include <string>
#include <iostream>

typedef uint32_t word;
typedef uint64_t longWord;

class BigInteger {
public:
    BigInteger();
    BigInteger(int x); // NOLINT(google-explicit-constructor)
    explicit BigInteger(std::string &s);
    BigInteger operator+(const BigInteger& b) const;
    BigInteger operator-(const BigInteger& b) const;
    BigInteger operator*(const BigInteger& b) const;
    BigInteger operator/(const BigInteger& b) const;
    BigInteger operator%(const BigInteger& b) const;

    BigInteger operator+=(const BigInteger&b);
    BigInteger operator-=(const BigInteger&b);
    BigInteger operator/=(const BigInteger&b);
    BigInteger operator*=(const BigInteger&b);
    BigInteger operator%=(const BigInteger&b);

    bool operator==(const BigInteger& b) const;
    bool operator!=(const BigInteger& b) const;
    bool operator<(const BigInteger& b) const;
    bool operator>(const BigInteger& b) const;
    bool operator<=(const BigInteger& b) const;
    bool operator>=(const BigInteger& b) const;

    BigInteger operator-() const;

    BigInteger& operator++() ;
    BigInteger operator++(int) ;
    BigInteger& operator--() ;
    BigInteger operator--(int) ;

    BigInteger abs() const;
    BigInteger shr() const;


    std::string toBinaryString() const;
    std::string toString() const;
    operator bool() const;

    friend std::istream &operator>>(std::istream &input, BigInteger &x);
    friend std::ostream &operator<<(std::ostream &output, const BigInteger &x);

private:
    static const longWord WORD_MASK = (1ULL << 32U) - 1;
    static const word WORD_SIZE = 32;
    std::vector<word> words;
    bool negative;

    bool isNegative() const;
    bool isPositive() const;
    BigInteger add(const BigInteger& b) const;
    BigInteger substract(const BigInteger& b) const;
    word getWord(int index) const;
    bool wordsEq(const BigInteger& b) const;
    int sign() const;
    bool isEven() const;
    BigInteger multiply(const BigInteger& b) const;
};