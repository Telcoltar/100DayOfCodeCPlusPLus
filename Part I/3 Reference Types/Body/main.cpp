#include <cstdio>

struct College {
    char name[256];
};
void print_name(College* college_ptr) {
printf("%s College\n", college_ptr->name);
}

int main() {
    College test = {"Best"};
    print_name(&test);
    College best_colleges[] = { "Magdalen", "Nuffield", "Kellogg" };
    print_name(best_colleges);

    char lower[8] = "abc?e";
    char upper[] = "ABC?E";
    char* upper_ptr = upper;

    lower[3] = 'd';
    upper_ptr[3] = 'D';

    printf("lower: %s\nupper: %s\n", lower, upper);

    lower[7] = 'g';

    printf("lower: %s\nupper: %s\n", lower, upper);

    *(lower + 3) = 'u';
    *(upper_ptr + 3) = 'U';

    *(lower + 6) = 'g';

    printf("lower: %s\nupper: %s\n", lower, upper);

    *(lower + 5) = 'g';

    printf("lower: %s\nupper: %s\n", lower, upper);

    *(lower + 7) = '\0';

    printf("lower: %s\nupper: %s\n", lower, upper);

    int original = 100;
    int& original_ref = original;
    printf("Original:  %d\n", original);
    printf("Reference: %d\n", original_ref);

    int new_value = 200;
    original_ref = new_value;
    printf("Original:  %d\n", original);
    printf("New Value: %d\n", new_value);
    printf("Reference: %d\n\n", original_ref);

    original = 100;
    int* original_ptr = &original;
    printf("Original:  %d\n", original);
    printf("Reference: %d\n", *original_ptr);

    new_value = 200;
    original_ptr = &new_value;
    printf("Original:  %d\n", original);
    printf("New Value: %d\n", new_value);
    printf("Reference: %d\n", *original_ptr);
}
