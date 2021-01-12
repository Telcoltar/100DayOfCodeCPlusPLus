#ifndef INC_10_TESTING_ALPHAHISTOGRAM_H
#define INC_10_TESTING_ALPHAHISTOGRAM_H

#include <cstdio>
#include <string>

struct AlphaHistogram {
    void ingest(const std::string& x);
    void print() const;
private:
    size_t counts[26]{};
};

#endif //INC_10_TESTING_ALPHAHISTOGRAM_H
