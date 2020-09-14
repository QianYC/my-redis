//
// Created by yc_qian on 20-8-10.
//

#include <string.h>
#include "hash.h"

size_t hash(char *str, size_t mod) {
    unsigned long long hash = 0;
    for (int i = 0; i < strlen(str); ++i) {
        hash = hash * 31 + str[i];
    }
    return hash % mod;
}