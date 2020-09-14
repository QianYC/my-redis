#include <stdio.h>
#include "my-redis.h"
#include "rbtree.h"

void test_rbtree_insert() {
    rbtree tree;
    tree_init(&tree);

    rbt_insert(&tree, 8, "", 0);
    rbt_insert(&tree, 4, "", 0);
    rbt_insert(&tree, 12, "", 0);
    rbt_insert(&tree, 2, "", 0);
    rbt_insert(&tree, 6, "", 0);
    rbt_insert(&tree, 10, "", 0);
    rbt_insert(&tree, 14, "", 0);
    rbt_insert(&tree, 1, "", 0);
    rbt_insert(&tree, 3, "", 0);
    rbt_insert(&tree, 5, "", 0);
    rbt_insert(&tree, 7, "", 0);
    rbt_insert(&tree, 9, "", 0);
    rbt_insert(&tree, 11, "", 0);
    rbt_insert(&tree, 13, "", 0);
    rbt_insert(&tree, 15, "", 0);
    rbt_insert(&tree, 16, "", 0);

}

int main() {

    test_rbtree_insert();
    return 0;
}