/****************************************************************************** *
 *      Developed By:  William T. Jones
 *                     NASA Langley Research Center
 *                     Phone:(757)864-5318
 *                     Email:w.t.jones@nasa.gov
 *
 *      Modifications: 
 *
 *
 *      Developed For: NASA Langley Research Center
 *
 *      Copyright:     Unauthorized use, dissemination or export of this
 *                     software is a violation of U.S. Government computer law.
 *
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "ByteSwap.h"



#if ( ( (defined(__BYTE_ORDER) && (__BYTE_ORDER==__LITTLE_ENDIAN)) || \
        (defined(_BYTE_ORDER) && (_BYTE_ORDER==_LITTLE_ENDIAN)) ||    \
        (defined(BYTE_ORDER) && (BYTE_ORDER==LITTLE_ENDIAN)) ||       \
        (defined(WINNT)) ) &&                                         \
      !defined(NO_BYTE_SWAP) )
#define IO_ENDIANSWAP(size_,x)             \
{                                          \
  register unsigned int  ii_,sz_=size_-1;           \
  register char *c_=(char *)(x);           \
  if( size_ > 1 )                          \
  {	   									   \
    for( ii_=0; ii_<size_/2; ++ii_ ) {     \
      *(c_+ii_)       ^= *(c_+(sz_-ii_));  \
      *(c_+(sz_-ii_)) ^= *(c_+ii_);        \
      *(c_+ii_)       ^= *(c_+(sz_-ii_));  \
    }                                      \
  }                                        \
}
#else
#define IO_ENDIANSWAP(size_,x) {}
#endif
namespace Parfait {
// ----------------------------------------------------------------------------
    inline size_t private_fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        unsigned int i;
        size_t ret;
        char *tmp = (char *) ptr;

        ret = fread(ptr, size, nmemb, stream);

        for (i = 0; i < nmemb; i++) {
            IO_ENDIANSWAP(size, tmp);
            tmp += size;
        }

        return (ret);
    }


// ----------------------------------------------------------------------------
    inline size_t private_fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream) {
        unsigned int i;
        size_t ret;
        char *tmp;

        for (i = 0, tmp = (char *) ptr; i < nmemb; i++) {
            IO_ENDIANSWAP(size, tmp);
            tmp += size;
        }

        ret = fwrite(ptr, size, nmemb, stream);

        for (i = 0, tmp = (char *) ptr; i < nmemb; i++) {
            IO_ENDIANSWAP(size, tmp);
            tmp += size;
        }

        return (ret);
    }
}