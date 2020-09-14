//
// Created by yc_qian on 20-8-10.
//

#include "my-redis.h"

/**==========================================================
 * helper functions' declaration
 ===========================================================*/
cobject *find_object(mredis *cache, char *key);

cobject *new_object(char *key, char *val, value_type type);

void insert_object(mredis *cache, cobject *obj);

/**==========================================================
 * header functions' definition
 ===========================================================*/

void cache_init(mredis *cache) {
    dbg_print("cache init\n");
    cache->size = DEFAULT_HASH_SIZE;
    cache->list = calloc(cache->size, sizeof(cobject *));
    for (int i = 0; i < cache->size; ++i) {
        cache->list[i] = NULL;
    }
}

void cache_free(mredis *cache) {
    dbg_print("cache free\n");
    for (int i = 0; i < cache->size; ++i) {
        cobject *obj = cache->list[i];
        while (obj) {
            cobject *tmp = obj->next;
            free(obj->key);
            if (obj->type == LITERAL) {
                free(obj->val);
            } else if (obj->type == LIST) {
                list_entry *ent = (list_entry *) obj->val;
                while (ent) {
                    list_entry *tmpent = ent->next;
                    free(ent->val);
                    free(ent);
                    ent = tmpent;
                }
            }
            free(obj);
            obj = tmp;
        }
    }
    free(cache->list);
}

void print_value(mredis *cache, char *key) {

}

/**
 * check if the given key exists in the cache
 * @param cache
 * @param key
 * @return 1 if exists, otherwise 0
 */
int exists(mredis *cache, char *key) {
    dbg_print("%s exists?\n", key);
    return find_object(cache, key) != NULL;
}

/**
 * set the key-value pair. If key exists, update the value, otherwise insert new value
 * @param cache
 * @param key
 * @param val
 * @return
 */
int set(mredis *cache, char *key, char *val) {
    dbg_print("set <%s : %s>\n", key, val);
    cobject *obj = find_object(cache, key);
    if (obj) {
        if (obj->type == LITERAL) {
            obj->val = realloc(obj->val, strlen(val));
//            obj->val = calloc(strlen(val), sizeof(char));
            memcpy(obj->val, val, strlen(val));
        } else {
            fprintf(stderr, "Incompatible type %d\n", obj->type);
            exit(0);
        }
    } else {
        obj = new_object(key, val, LITERAL);
        insert_object(cache, obj);
    }
    return 0;
}

char *get(mredis *cache, char *key) {
    dbg_print("get <%s>\n", key);
    cobject *obj = find_object(cache, key);
    if (obj) {
        if (obj->type == LITERAL) {
            return (char *) obj->val;
        } else {
            fprintf(stderr, "Incompatible type %d\n", obj->type);
            exit(0);
        }
    } else {
        return NULL;
    }
}

/**============================================================
 * helper functions                                           *
 =============================================================*/

/**
 * find the pointer to cobject specified by the key
 * @param obj
 * @param key
 * @return
 */
cobject *find_object(mredis *cache, char *key) {
    size_t index = hash(key, cache->size);
    cobject *obj = cache->list[index];
    while (obj) {
        if (!strncmp(obj->key, key, strlen(obj->key))) {
            return obj;
        }
        obj = obj->next;
    }
    return NULL;
}

cobject *new_object(char *key, char *val, value_type type) {
    cobject *obj = calloc(1, sizeof(cobject));
    obj->type = type;
    obj->key = malloc(strlen(key));
    memcpy(obj->key, key, strlen(key));

    if (type == LITERAL) {
        obj->val = malloc(strlen(val));
        memcpy(obj->val, val, strlen(val));
        obj->prev = NULL;
        obj->next = NULL;
    } else if (type == LIST) {
        obj->val = calloc(1, sizeof(list_entry));
        obj->opt = obj->val;

        list_entry *ent = obj->val;
        ent->val = malloc(strlen(val));
        memcpy(ent->val, val, strlen(val));
        ent->next = NULL;
    } else {
        fprintf(stderr, "Unknown value type %d\n", type);
        exit(0);
    }
    return obj;
}

void insert_object(mredis *cache, cobject *obj) {
    size_t index = hash(obj->key, cache->size);
    cobject *prev = NULL, *next = cache->list[index];
    while (next) {
        prev = next;
        next = next->next;
    }
    if (prev) {
        prev->next = obj;
        obj->prev = prev;
    } else {
        cache->list[index] = obj;
    }
}