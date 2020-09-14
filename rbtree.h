//
// Created by yc_qian on 20-9-12.
//

#ifndef MY_REDIS_RBTREE_H
#define MY_REDIS_RBTREE_H

#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef enum color {
    RED,
    BLACK
} color;

typedef struct node {
    color c;
    struct node *left;
    struct node *right;
    struct node *parent;
    int val;
    void *data;
} node;

typedef struct rbtree {
    node *root;
} rbtree;

void tree_init(rbtree *tree);

void tree_destroy(rbtree *tree);

void rbt_insert(rbtree *tree, int val, void *data, size_t size);

void *rbt_find(rbtree *tree, int val);

bool rbt_exists(rbtree *tree, int val);

bool rbt_delete(rbtree *tree, int val, void *ptr);

#endif //MY_REDIS_RBTREE_H