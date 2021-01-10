struct PrimeIterator {
    bool operator!=(long x) const;

    PrimeIterator& operator++();

    long operator*() const;
private:
    long current{ 1 }, last{ 1 };
};

struct PrimeRange {
    explicit PrimeRange(long max);
    [[nodiscard]] PrimeIterator begin() const;
    [[nodiscard]] long end() const;
private:
    const long max;
};

