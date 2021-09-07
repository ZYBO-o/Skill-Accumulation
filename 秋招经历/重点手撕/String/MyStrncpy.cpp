#include<stdio.h>
#include<assert.h>

char *myStrncpy(char *strDest, const char *strSrc, size_t n)   //对源字符串不希望其修改，所以加上const来修饰
{
    assert(strDest != NULL && strSrc != NULL && n > 0);

    if (strDest == NULL || strSrc == NULL)
        return NULL;

    if (strDest == strSrc)
        return strDest;

    char *address = strDest;

    while( n-- && '\0' != ( *strDest++ = *strSrc++ ) ){;}

    *strDest='\0';

    return address;
}