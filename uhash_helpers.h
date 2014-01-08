#include <string.h>
#include <stdint.h>

#ifndef __HELPER_H__
# define __HELPER_H__

inline int keycmp_uint32(const void *key1, const void *key2);
inline size_t keyidx_uint32(const void *key, size_t max);
inline int keycmp_uint64(const void *key1, const void *key2);
inline size_t keyidx_uint64(const void *key, size_t max);
inline int keycmp_int(const void *key1, const void *key2);
inline size_t keyidx_int(const void *key, size_t max);
inline int keycmp_str(const void *key1, const void *key2);
inline size_t keyidx_str(const void *key, size_t max);
inline void dump_int(const void *val);
inline void dump_str(const void *str);

#endif /* __HELPER_H__ */
