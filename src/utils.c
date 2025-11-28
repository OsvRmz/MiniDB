#include "utils.h"
#include <ctype.h>
#include <stddef.h>


int strncasecmp_portable(const char *a, const char *b, size_t n) {
    size_t i;
    for (i = 0; i < n && a[i] && b[i]; ++i) {
        char ca = (char)tolower((unsigned char)a[i]);
        char cb = (char)tolower((unsigned char)b[i]);
        if (ca != cb) return (unsigned char)ca - (unsigned char)cb;
    }
    if (i == n) return 0;
    return (unsigned char)a[i] - (unsigned char)b[i];
}
