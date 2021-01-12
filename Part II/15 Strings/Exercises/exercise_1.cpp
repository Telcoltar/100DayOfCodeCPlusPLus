#include "AlphaHistogram.h"
#include <cstdio>
#include <cstdint>

int main(int argc, char** argv) {
    AlphaHistogram hist;
    for(size_t i{ 1 }; i<argc; i++) {
        hist.ingest(argv[i]);
    }
    hist.print();
}
