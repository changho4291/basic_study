#include <stdio.h>
#include <time.h>

int main(int argc, char const *argv[]) {
    struct tm *t;
    time_t now = time(NULL);
    t = localtime(&now);

    printf("%04d-%02d-%02d", 1900 + t->tm_year, t->tm_mon + 1, t->tm_mday);
    return 0;
}