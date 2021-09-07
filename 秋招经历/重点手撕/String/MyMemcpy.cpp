#include<stdio.h>
#include<assert.h>

void * Mymemcpy1(void *dst, const void *src, size_t count)  {  
    if (dst== NULL || src == NULL)  
          return NULL;  
    char *pdest = (char *)(dst);  
    const char *psrc  = (char *)(psrc);  
    int n = count;  
    //pdest地址高于psrc地址，且有重叠
    if (pdest > psrc && pdest < psrc+count)  
    {  
        for (size_t i=n-1; i != -1; --i)  
        {  
                pdest[i] = psrc[i];//从高到低赋值  
        }  
    }  
    //pdest地址低于psrc地址，且有重叠
    else if(pdest < psrc && pdest > psrc-count)
    {  
        for (size_t i= 0; i < n; i++)  
        {  
                pdest[i] = psrc[i];//从低到高赋值  
        }  
    }  
    return dst;  
}

void *mymemcpy(void *dst,const void *src,size_t num)  {  
       if(NULL == dst || NULL == src){  
       return NULL;  
       }  
       //assert((dst !=NULL) && (src!=NULL));

       if(dst >= src + num || src > dst + num){  
            char * psrc = (char *)src;
            char * pdst = (char *)dst;  
            while(num-->0)*pdst++ = *psrc++;  
       }  
       return dst;  
}