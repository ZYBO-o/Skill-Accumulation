#include<stdio.h>
#include<assert.h>

int Mystrcmp(const char* s1, const char* s2) {
    assert(s1 != NULL && s2 != NULL);

    while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) {
        s1++;
        s2++;
    }

    if (*s1 > *s2)
        return 1;
    else if (*s1 < *s2)
        return -1;
    else {
        return 0;
    }

}