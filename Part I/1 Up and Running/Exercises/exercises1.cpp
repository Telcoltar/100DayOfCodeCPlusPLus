#include <cstdio>

int absolute_value(int x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}

int sum(int a, int b) {
    return a + b;
}

int main() {
    int my_num = -10;
    int num1 = 12;
    int num2 = 4;
    printf("The absolute value of %d is %d.\n", my_num,
           absolute_value(my_num));
    printf("The sum of %d and %d is %d.", num1, num2, sum(num1, num2));
}