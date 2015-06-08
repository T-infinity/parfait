#ifndef BYTESWAP_H
#define BYTESWAP_H

namespace Parfait {
// binary read/write functions that swap endianness 
    size_t private_fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    size_t private_fwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);
}

#endif

