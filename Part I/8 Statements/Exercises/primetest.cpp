#include <algorithm>
#include <iostream>

long power_mod(long b, long e, long mod) {
    if (e == 0) return 1;
    if (e == 1) return b % mod;
    if ( e % 2 == 0) return power_mod((b * b) % mod, e / 2, mod) % mod;
    else {
        long r{(b  * power_mod((b * b) % mod, (e - 1) / 2, mod)) % mod};
        return r;
    };
}

bool miller_rabin(long n, long a, long k, long j) {
    if (a >= n) return true;
    long m{power_mod(a, k, n)};
    if (m == 1) {
        return true;
    }
    if (m == n - 1) {
        return true;
    }
    for (long i{}; i < j - 1; i++) {
        m *= m;
        m %= n;
        if (m == n - 1) {
            return true;
        }
    }
    return false;
}

bool is_prime(long n) {
    if (n == 2) return true;
    if (n  % 2 == 0) return false;
    long j{};
    long k{n - 1};
    while (k % 2 == 0) {
        j++;
        k /= 2;
    }
    std::vector<long> bases = {2,3,5,7, 11, 13, 17};
    return std::ranges::all_of(bases, [n, k, j](long a){return miller_rabin(n, a, k, j);});
}
