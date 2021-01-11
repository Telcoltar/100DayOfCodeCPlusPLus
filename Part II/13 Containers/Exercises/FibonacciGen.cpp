class FibonacciGen {

    int current{1};
    int last{0};

public:

    int next() {
        int tmp = current;
        current = current + last;
        last = tmp;
        return tmp;
    }
};