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

BigInteger BigInteger::operator+(const BigInteger& b) const {
    //TODO check signs
    return this->add(b);
}

BigInteger BigInteger::operator-(const BigInteger& b) const {
    //TODO check signs;
    return this->substract(b);
}

BigInteger BigInteger::operator*(const BigInteger& b) const {

}

BigInteger BigInteger::operator/(const BigInteger& b) const {

}

BigInteger BigInteger::operator%(const BigInteger& b) const {

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

bool BigInteger::wordsEq(const BigInteger &b) const {
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
        BigInteger bAbs = abs();
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
