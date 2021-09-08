#include<stdio.h>
#include<assert.h>

char *myStrcpy(char *dest, const char *src) {

    assert(dest != NULL && src != NULL);

    if (dest == NULL || src == NULL)
        return NULL;

    if (dest == src)
        return dest;

    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';

    return dest;
}