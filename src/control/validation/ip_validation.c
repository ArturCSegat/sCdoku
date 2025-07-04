#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>

bool is_valid_ip(const char *ip) {
    if (ip == NULL) return false;

    int num, dots = 0;
    const char *ptr = ip;

    while (*ptr) {
        if (!isdigit(*ptr)) {
            if (*ptr == '.') {
                dots++;
                ptr++;
                continue;
            } else {
                return false;
            }
        }

        num = 0;
        int digits = 0;
        while (*ptr && isdigit(*ptr)) {
            num = num * 10 + (*ptr - '0');
            ptr++;
            digits++;
            if (digits > 3) return false;
        }

        if (num < 0 || num > 255) return false;

        if (*ptr && *ptr != '.') return false;
    }

    return dots == 3;
}
