#include<stdio.h>
#include<assert.h>

void* MyMemmove(void* dest, void* source, size_t count) 
{ 
    assert((dest!=NULL) && (source !=NULL)); 
    void* ret = dest; 
    if (dest <= source || dest >= (source + count)) 
    { 
    while (count --) 
       *dest++ = *source++; 
    } 
   else 
   { 
      dest += count - 1; 
      source += count - 1; 
      while (count--) 
         *dest-- = *source--;
  } 
  return ret; 
}
