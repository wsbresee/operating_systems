#include <stdio.h>
#include <stdlib.h>

int is_a_leap_year(year) {
    if (!(year%4)) {
        if (!(year%100)) {
            if (!(year%400)) {
                return 1;
            } else {
                return 0;
            }
        } else {
            return 1;
        }
    } else {
        return 0;
    }
}

int main(int argc, char *argv[]) {
    int year = atoi(argv[1]);
    if (is_a_leap_year(year)) {
        printf("%i was a leap year\n", year);
        return 1;
    } else {
        printf("%i was not a leap year\n", year);
        return 0;
    }
}
