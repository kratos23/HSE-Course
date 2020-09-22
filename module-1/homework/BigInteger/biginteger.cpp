#include <cassert>
#include "biginteger.h"

BigInteger::BigInteger() {
    negative = false;
}

BigInteger::BigInteger(int x) {
    negative = x < 0;
    if (x < 0) {
        word absX = -x;
        words.push_back(absX);
    } else {
        word absX = x;
        words.push_back(absX);
    }
}

BigInteger::BigInteger(std::string& s) : BigInteger() {
    const int groupSize = 9;
    const BigInteger multiply((int) 1e9);
    int firstGroupSize = (int) s.size() % groupSize;
    if (firstGroupSize > 0) {
        std::string firstGroup = s.substr(0, firstGroupSize);
        *this = (*this) + BigInteger(std::stoi(firstGroup));
    }
    int cursorPos = firstGroupSize;
    while (cursorPos < (int) s.size()) {
        std::string curGroup = s.substr(cursorPos, groupSize);
        *this = (*this) * multiply;
        *this = (*this) + BigInteger(std::stoi(curGroup));
        cursorPos += groupSize;
    }
}

BigInteger BigInteger::operator+(const BigInteger& b) const {
    if (isPositive() && b.isPositive()) {
        return this->add(b);
    } else if (isNegative() && b.isNegative()) {
        BigInteger aAbs = abs();
        BigInteger bAbs = abs();
        BigInteger result = aAbs.add(bAbs);
        result.negative = true;
        return result;
    } else if (isNegative() && b.isPositive()) {
        return b - (*this);
    } else if (isPositive() && b.isNegative()) {
        return (*this) - b;
    } else {
        assert(false);
    }
}

BigInteger BigInteger::operator-(const BigInteger& b) const {
    if (isPositive() && b.isPositive()) {
        return this->substract(b);
    } else if (isNegative() && b.isNegative()) {
        BigInteger bAbs = b.abs();
        return bAbs.substract(*this);
    } else if (isNegative() && b.isPositive()) {
        BigInteger aAbs = abs();
        BigInteger result = aAbs.add(b);
        result.negative = true;
        return result;
    } else if (isPositive() && b.isNegative()) {
        BigInteger bAbs = b.abs();
        BigInteger result = add(bAbs);
        return result;
    } else {
        assert(false);
    }
}

BigInteger BigInteger::operator*(const BigInteger& b) const {
    int sign1 = sign();
    int sign2 = b.sign();
    int signResult = sign1 * sign2;
    if (signResult == 0) {
        return BigInteger(0);
    } else if (signResult == -1) {
        BigInteger aAbs = abs();
        BigInteger bAbs = b.abs();
        BigInteger result = aAbs.multiply(b);
        result.negative = true;
        return result;
    } else if (signResult == 1) {
        BigInteger aAbs = abs();
        BigInteger bAbs = b.abs();
        BigInteger result = aAbs.multiply(bAbs);
        return result;
    } else {
        assert(false);
    }
}

BigInteger BigInteger::operator/(const BigInteger& b) const {
    int sign1 = sign();
    int sign2 = b.sign();
    int signResult = sign1 * sign2;
    if (signResult == 0) {
        assert(false);
    }

    BigInteger aAbs = abs();
    BigInteger bAbs = b.abs();
    BigInteger l = BigInteger(0);
    BigInteger r = BigInteger(*this) + BigInteger(1);
    while (r - l > BigInteger(1)) {
        BigInteger m = (r + l).shr();
        if (m * bAbs <= aAbs) {
            l = m;
        } else {
            r = m;
        }
    }

    BigInteger& result = l;
    if (signResult == 1) {
        result.negative = false;
    } else if (signResult == -1) {
        result.negative = true;
    } else {
        assert(false);
    }
    return result;
}

BigInteger BigInteger::operator%(const BigInteger& b) const {
    BigInteger k = (*this) / b;
    return (*this) - k * b;
}

BigInteger BigInteger::abs() const {
    auto result = BigInteger(*this);
    result.negative = false;
    return result;
}

bool BigInteger::isNegative() const {
    return this->negative;
}

bool BigInteger::isPositive() const {
    return !this->negative;
}

bool BigInteger::wordsEq(const BigInteger& b) const {
    for (int i = 0; i < std::max(words.size(), b.words.size()); i++) {
        if (getWord(i) != b.getWord(i)) {
            return false;
        }
    }
    return true;
}

bool BigInteger::operator==(const BigInteger& b) const {
    return this->negative == b.negative && wordsEq(b);
}

bool BigInteger::operator!=(const BigInteger& b) const {
    return !(*this == b);
}

bool BigInteger::operator<(const BigInteger& b) const {
    if (this->isNegative() && b.isPositive()) {
        return true;
    } else if (this->isPositive() && b.isNegative()) {
        return false;
    } else {
        BigInteger aAbs = abs();
        BigInteger bAbs = b.abs();
        bool result = aAbs.words < bAbs.words;
        if (aAbs.isNegative()) {
            assert(bAbs.isNegative());
            result = !result;
        }
        return result;
    }
}

bool BigInteger::operator>(const BigInteger& b) const {
    return !(*this <= b);
}

bool BigInteger::operator<=(const BigInteger& b) const {
    return (*this) < b || (*this == b);
}

bool BigInteger::operator>=(const BigInteger& b) const {
    return (*this > b) || (*this == b);
}
BigInteger BigInteger::add(const BigInteger& b) const {
    const BigInteger& a = *this;
    assert(a.isPositive());
    assert(b.isPositive());
    BigInteger result;
    int i = 0;
    word carry = 0;
    while (carry > 0 || i < std::max(a.words.size(), b.words.size())) {
        longWord aWord = a.getWord(i);
        longWord bWord = b.getWord(i);
        longWord sum = aWord + bWord + (longWord) carry;
        word newWord = sum & WORD_MASK;
        result.words.push_back(newWord);
        carry = sum >> WORD_SIZE;
        i++;
    }
    return result;
}

BigInteger BigInteger::substract(const BigInteger& b) const {
    const BigInteger& a = *this;
    assert(a.isPositive());
    assert(b.isPositive());
    if (a < b) {
        BigInteger result = b.substract(a);
        result.negative = true;
        return result;
    }
    word carry = 0;
    BigInteger result;
    for (int i = 0; i < std::max(a.words.size(), b.words.size()); i++) {
        longWord aWord = a.getWord(i);
        longWord bWord = b.getWord(i) + carry;
        carry = 0;
        if (aWord < bWord) {
            aWord += 1ULL << WORD_SIZE;
            carry = 1;
        }
        assert(aWord >= bWord);
        result.words.push_back(aWord - bWord);
    }
    return result;
}

word BigInteger::getWord(int index) const {
    if (index < words.size()) {
        return words[index];
    } else {
        return 0;
    }
}

BigInteger BigInteger::shr() const {
    BigInteger result(*this);
    for (int i = 0; i < result.words.size(); i++) {
        word newWord = result.getWord(i) >> 1;
        newWord |= (result.getWord(i + 1) & 1) << (WORD_SIZE - 1);
        result.words[i] = newWord;
    }
    return result;
}

std::string BigInteger::toBinaryString() const {
    std::string reversedResult;
    for (auto word : words) {
        for (int i = 0; i < WORD_SIZE; i++) {
            int bit = (word >> i) & 1;
            reversedResult += (char) ((int) '0' + bit);
        }
    }
    std::string result;
    for (int i = (int) reversedResult.size() - 1; i >= 0; --i) {
        result += reversedResult[i];
    }
    return result;
}

int BigInteger::sign() const {
    if (*this == BigInteger(0)) {
        return 0;
    }
    if (negative) {
        return -1;
    } else {
        return 1;
    }
}

bool BigInteger::isEven() const {
    return (getWord(0) & 1) == 0;
}

BigInteger BigInteger::multiply(const BigInteger& b) const {
    assert(b.isPositive());
    if (b == BigInteger(0)) {
        return 0;
    }
    if (b.isEven()) {
        BigInteger tmp = multiply(b.shr());
        return tmp + tmp;
    } else {
        BigInteger tmp = multiply(b - BigInteger(1));
        return tmp + (*this);
    }
}

BigInteger BigInteger::operator-() const {
    if (*this == BigInteger(0)) {
        return BigInteger(0);
    } else {
        BigInteger result(*this);
        result.negative = !negative;
        return result;
    }
}

BigInteger& BigInteger::operator++() {
    *this = (*this) + BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger tmp = *this;
    *this = (*this) + BigInteger(1);
    return tmp;
}

BigInteger& BigInteger::operator--() {
    *this = (*this) - BigInteger(1);
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger tmp = *this;
    *this = (*this) - BigInteger(1);
    return tmp;
}

std::istream& operator>>(std::istream& input, BigInteger& x) {
    std::string tmp;
    input >> tmp;
    x = BigInteger(tmp);
    return input;
}

std::string BigInteger::toString() const {
    const auto base = BigInteger(10);
    std::string reversedResult;
    BigInteger cur = abs();
    while (cur > BigInteger(0)) {
        int digit = (cur % base).getWord(0);
        reversedResult += (char)((int)'0' + digit);
        cur /= base;
    }
    std::string result;
    if (isNegative()) {
        result += "-";
    }
    for (int i = (int) reversedResult.size() - 1; i >= 0; --i) {
        result += reversedResult[i];
    }
    return result;
}

std::ostream& operator<<(std::ostream& output, const BigInteger& x) {
    output << x.toString();
    return output;
}

BigInteger BigInteger::operator+=(const BigInteger& b) {
    *this = (*this) + b;
    return *this;
}

BigInteger BigInteger::operator-=(const BigInteger& b) {
    *this = (*this) - b;
    return *this;
}

BigInteger BigInteger::operator/=(const BigInteger& b) {
    *this = (*this) / b;
    return *this;
}

BigInteger BigInteger::operator*=(const BigInteger& b) {
    *this = (*this) * b;
    return *this;
}

BigInteger BigInteger::operator%=(const BigInteger& b) {
    *this = (*this) % b;
    return *this;
}
BigInteger::operator bool() const {
    return (*this) != BigInteger(0);
}
