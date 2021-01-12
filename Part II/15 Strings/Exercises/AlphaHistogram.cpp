#include "AlphaHistogram.h"
#include <string>

constexpr char pos_A{ 65 }, pos_Z{ 90 }, pos_a{ 97 }, pos_z{ 122 };
constexpr bool within_AZ(char x) { return pos_A <= x && pos_Z >= x; }
constexpr bool within_az(char x) { return pos_a <= x && pos_z >= x; }

void AlphaHistogram::ingest(const std::string& x) {
    for(const auto c : x) {
        if (within_AZ(c)) counts[c - pos_A]++;
        else if (within_az(c)) counts[c - pos_a]++;
    }
}

void AlphaHistogram::print() const {
    for(auto index{ pos_A }; index <= pos_Z; index++) {
        printf("%c: ", index);
        auto n_asterisks = counts[index - pos_A];
        while (n_asterisks--) printf("*");
        printf("\n");
    }
}
