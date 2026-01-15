#ifndef DYNLOAD_H
#define DYNLOAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef int dynload_version_t[3];

int
dynload_parse(const char *file, char *name, size_t len, dynload_version_t version);

int
dynload_compare(dynload_version_t a, dynload_version_t b);

int
dynload_resolve(const char *name, const char *prefix, char *result, size_t *len);

#ifdef __cplusplus
}
#endif

#endif // DYNLOAD_H
