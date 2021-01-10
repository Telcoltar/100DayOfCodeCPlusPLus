#include "primegenerator.h"
#include "primetest.cpp"

long next_prime(long current) {
    current++;
    while (!is_prime(current)) {
        current++;
    }
    return current;
}

bool PrimeIterator::operator!=(long x) const {
    return x >= current;
}

PrimeIterator& PrimeIterator::operator++() {
    current = next_prime(current);
    return *this;
}

long PrimeIterator::operator*() const {
    return current;
}

PrimeRange::PrimeRange(long max) : max{ max } { }

PrimeIterator PrimeRange::begin() const{
    return PrimeIterator{};
}

long PrimeRange::end() const {
    return max;
}



