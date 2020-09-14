//
// Created by yc_qian on 20-8-10.
//

#ifndef MY_REDIS_MY_REDIS_HPP
#define MY_REDIS_MY_REDIS_HPP

#include <stddef.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"

#ifdef DEBUG
#define dbg_print(...)  printf(__VA_ARGS__)
#else
#define dbg_print(...)  (1)
#endif

#define DEFAULT_HASH_SIZE   64

typedef enum value_type {
    LITERAL,
    LIST
} value_type;

typedef struct list_entry {
    struct list_entry *next;
    char *val;
} list_entry;

typedef struct cache_object {
    struct cache_object *prev;
    struct cache_object *next;
    char *key;
    void *val;
    void *opt;          /* if type==LIST, this field is used as tail of the list */
    value_type type;
} cobject;

typedef struct my_redis {
    cobject **list;
    size_t size;
} mredis;

void cache_init(mredis *cache);

void cache_free(mredis *cache);

void print_value(mredis *cache, char *key);

int exists(mredis *cache, char *key);

int set(mredis *cache, char *key, char *val);

char *get(mredis *cache, char *key);

int lpush(mredis *cache, char *key, char *val);

char *lpop(mredis *cache, char *key);

int rpush(mredis *cache, char *key, char *val);

char *rpop(mredis *cache, char *key);

int del(char *key);

int incr(char *key);

int decr(char *key);

#endif //MY_REDIS_MY_REDIS_HPP
