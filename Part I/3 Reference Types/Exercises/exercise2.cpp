#include <cstdio>


void write_to(char* characters, size_t length, char element, size_t position) {
    if (position >= length - 1 || position < 0) {
        printf("Error, out of range.\n");
        return;
    }
    characters[position] = element;
}

char read_from(char* characters, size_t length, size_t position) {
    if (position >= length - 1 || position < 0) {
        printf("Error, out of range.\n");
        return '\0';
    }
    return characters[position];
}

int main() {
    char lower[] = "abc?e";
    char upper[] = "ABC?E";
    char* upper_ptr = upper;

    lower[3] = 'd';
    upper_ptr[3] = 'D';

    char letter_d = lower[3];
    char letter_D = upper_ptr[3];

    printf("lower: %s\nupper: %s\n", lower, upper);

    write_to(lower, sizeof(lower)/sizeof(char), 'u', 3);
    write_to(upper, sizeof(upper)/sizeof(char), 'U', 3);

    printf("lower: %s\nupper: %s\n", lower, upper);

    write_to(lower, sizeof(lower)/sizeof(char), 'g', 7);
    write_to(upper, sizeof(upper)/sizeof(char), 'G', 7);

    write_to(lower, sizeof(lower)/sizeof(char), 'g', -1);
    write_to(upper, sizeof(upper)/sizeof(char), 'G', -1);

    printf("lower: %s\nupper: %s\n", lower, upper);

    printf("Read from position 5: %c\n", read_from(lower, sizeof(lower)/sizeof(char), 5));
    printf("Read from position 4: %c", read_from(lower, sizeof(lower)/sizeof(char), 4));
}