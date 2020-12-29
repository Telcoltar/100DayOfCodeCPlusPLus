#include <cstdio>

enum class Operation{
    Add,
    Subtract,
    Multiply,
    Divide
};

struct Calculator {
    explicit Calculator(Operation op_in) {
        op = op_in;
    }

    int calculate(int a, int b) {
        switch (op) {
            case Operation::Add: {
                return a + b;
            }
            case Operation::Subtract: {
                return a - b;
            }
            case Operation::Multiply: {
                return a * b;
            }
            case Operation::Divide: {
                return a / b;
            }
        }
    }

private:
    Operation op;

};

int main() {

    auto calc_add = Calculator(Operation::Add);
    printf("5 + 10 = %d\n", calc_add.calculate(5,10));

    auto calc_div = Calculator(Operation::Divide);
    printf("10 / 7 = %d\n", calc_div.calculate(10,7));

    return 0;
}