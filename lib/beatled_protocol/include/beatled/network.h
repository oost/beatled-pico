#ifndef BEATLED_PROTOCOL__NETWORK_H
#define BEATLED_PROTOCOL__NETWORK_H

#include <arpa/inet.h>

#ifndef __APPLE__

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN ||                   \
    defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) ||   \
    defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) ||         \
    defined(__MIBSEB__)
// Big endian
#define htonll(x) x

#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN ||              \
    defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) ||                        \
    defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) ||      \
    defined(__MIPSEL) || defined(__MIPSEL__)
// Little endian

#define htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))

#else
#error "I don't know what architecture this is!"
#endif

// Define the network to hardware equivalent
#define ntohll(x) htonll(x)

#endif

#endif // BEATLED_PROTOCOL__NETWORK_H
