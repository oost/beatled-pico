#ifndef UTILS__NETWORK_H
#define UTILS__NETWORK_H

#include <lwip/def.h>

#if BYTE_ORDER == BIG_ENDIAN
#define pico_htonll(x) ((u64_t)(x))
#define pico_ntohll(x) ((u64_t)(x))
#else /* BYTE_ORDER != BIG_ENDIAN */
#define pico_htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))
#define pico_ntohll(x) pico_htonll(x)
#endif

#define htonll(x) pico_htonll(x)
#define ntohll(x) pico_ntohll(x)

#endif // UTILS__NETWORK_H