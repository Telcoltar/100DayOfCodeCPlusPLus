#include <cstdio>

template <typename Fn, typename In, typename Out>
constexpr Out fold(Fn function, In* input, size_t length, Out initial) {
    Out current{initial};
    for (size_t i{}; i < length; i++) {
        current  = function(current, input[i]);
    }
    return current;
}

int main() {
    int data[]{ 100, 200, 300, 400, 500 };
    size_t data_len = 5;
    auto sum = fold([](auto x, auto y) { return x + y; }, data, data_len,
                    0);
    printf("Sum: %d\n", sum);
    auto max = fold([](auto x, auto y) { if ( y  > x) return y; return x;}, data, data_len, data[0]);
    printf("Max: %d\n", max);
    auto min = fold([](auto x, auto y) { if ( y  < x) return y; return x;}, data, data_len, data[0]);
    printf("Min: %d\n", min);
    auto count_greater_200 = fold([](auto x, auto y) { if ( y > 200) return x + 1; return x;},
                                  data, data_len, 0);
    printf("Count: %d\n", count_greater_200);
}
