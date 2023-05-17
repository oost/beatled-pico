#ifndef BEATLED_PROTOCOL__NETWORK_H
#define BEATLED_PROTOCOL__NETWORK_H

#include <arpa/inet.h>

#ifndef __APPLE__
#if BYTE_ORDER == BIG_ENDIAN
#define htonll(x) x
#else
#define htonll(x) ((((uint64_t)htonl(x)) << 32) + htonl((x) >> 32))
#endif
#define ntohll(x) htonll(x)
#else
#endif

#endif // BEATLED_PROTOCOL__NETWORK_H
