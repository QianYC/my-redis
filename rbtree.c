//
// Created by yc_qian on 20-9-12.
//

#include "rbtree.h"

void notnull(void *n) {
    if (!n) {
        perror("null pointer!\n");
        exit(-1);
    }
}

void check_color(node *n, color c) {
    notnull(n);
    if (n->c != c) {
        perror("inconsistent color!\n");
        exit(-1);
    }
}

node *root(node *me);

node *parent(node *me);

node *uncle(node *me);

node *grandpa(node *me);

bool is_left(node *me);

void adjust(node *me);

void tree_init(rbtree *tree) {
    notnull(tree);
    tree->root = NULL;
}

void tree_destroy(rbtree *tree) {
    notnull(tree);

}

void rbt_insert(rbtree *tree, int val, void *data, size_t size) {
    notnull(tree);

    if (!tree->root) {
        tree->root = calloc(1, sizeof(node));
        tree->root->c = BLACK;
        tree->root->val = val;
        tree->root->data = malloc(size);
        memcpy(tree->root->data, data, size);
    } else {
        node *son = calloc(1, sizeof(node));
        son->val = val;
        son->data = malloc(size);
        memcpy(son->data, data, size);

        node *p = NULL, *n = tree->root;
        while (n) {
            p = n;
            if (n->val > val) {
                n = n->left;
            } else {
                n = n->right;
            }
        }
        if (p->val > val) {
            p->left = son;
        } else {
            p->right = son;
        }
        son->parent = p;
        adjust(son);
        tree->root = root(tree->root);
    }
}


void *rbt_find(rbtree *tree, int val) {

}

bool rbt_exists(rbtree *tree, int val) {
    return rbt_find(tree, val) != NULL;
}

void adjust(node *me) {
    if (!me) {
        return;
    }
    if (!me->parent) {
        me->c = BLACK;
        return;
    }
    if (me->parent->c == BLACK) {
        return;
    }
    node *u = uncle(me);
    node *g = grandpa(me);
    node *p = parent(me);
    if (u) {
        if (u->c == RED) {
            check_color(u, RED);
            u->c = BLACK;
            p->c = BLACK;
            g->c = RED;
            adjust(g);
        } else {
            node *gp = parent(g);
            bool gleft = is_left(g);

            p->c = BLACK;
            g->c = RED;
            if (is_left(p)) {
                g->left = p->right;
                p->right->parent = g;
                g->parent = p;
                p->right = g;
            } else {
                g->right = p->left;
                p->left->parent = g;
                g->parent = p;
                p->left = g;
            }

            if (gp) {
                if (gleft) {
                    gp->left = p;
                } else {
                    gp->right = p;
                }
            }
            p->parent = gp;
        }
    } else {
        bool pleft = is_left(p);
        bool mleft = is_left(me);

        if (pleft) {
            if (mleft) {
                p->c = BLACK;
                g->c = RED;
                node *gp = parent(g);
                if (gp) {
                    if (is_left(g)) {
                        gp->left = p;
                    } else {
                        gp->right = p;
                    }
                }
                p->parent = gp;
                g->left = p->right;
                p->right = g;
                g->parent = p;
            } else {
                me->c = BLACK;
                g->c = RED;
                node *gp = parent(g);
                if (gp) {
                    if (is_left(g)) {
                        gp->left = me;
                    } else {
                        gp->right = me;
                    }
                }
                me->parent = gp;
                p->right = me->left;
                me->left = p;
                p->parent = me;
                g->left = me->right;
                me->right = g;
                g->parent = me;
            }
        } else {
            if (mleft) {
                me->c = BLACK;
                g->c = RED;
                node *gp = parent(g);
                if (gp) {
                    if (is_left(g)) {
                        gp->left = me;
                    } else {
                        gp->right = me;
                    }
                }
                me->parent = gp;
                g->right = me->left;
                me->left = g;
                g->parent = me;
                p->left = me->right;
                me->right = p;
                p->parent = me;
            } else {
                p->c = BLACK;
                g->c = RED;
                node *gp = parent(g);
                if (gp) {
                    if (is_left(g)) {
                        gp->left = p;
                    } else {
                        gp->right = p;
                    }
                }
                p->parent = gp;
                g->right = p->left;
                p->left = g;
                g->parent = p;
            }
        }
    }
}

node *root(node *me) {
    if (!me) {
        return NULL;
    }

    while (me->parent) {
        me = me->parent;
    }
    return me;
}

node *parent(node *me) {
    return me ? me->parent : NULL;
}

node *uncle(node *me) {
    node *p = parent(me);
    node *g = grandpa(me);
    if (p && g) {
        return is_left(p) ? g->right : g->left;
    }
    return NULL;
}

node *grandpa(node *me) {
    return me && me->parent ? me->parent->parent : NULL;
}

bool is_left(node *me) {
    return me && me->parent && me == me->parent->left;
}