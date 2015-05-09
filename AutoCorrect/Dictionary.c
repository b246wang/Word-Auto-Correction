//
//  Dictionary.c
//  AutoCorrect
//
//  Created by Brody Wang on 2015-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "Dictionary.h"

/**
 Bincan Wang
 File: dictionary.c
 Implementatzion for dictionary.h
 */

struct bstnode {
    void *key;
    void *value;
    struct bstnode *left;
    struct bstnode *right;
};


struct dictionary {
    struct bstnode *root;
    CompareFunction f_comp;
    FreeFunction free_k;
    FreeFunction free_v;
};


//typedef int (*CompareFunction)(const void *, const void *);
//typedef void (*FreeFunction)(void *);

Dictionary create_Dictionary(CompareFunction comp_k, FreeFunction free_k, FreeFunction free_v) {
    Dictionary new_d = malloc(sizeof(struct dictionary));
    new_d->f_comp = comp_k;
    new_d->free_k = free_k;
    new_d->free_v = free_v;
    new_d->root = NULL;
    return new_d;
}


void destroy_help(struct bstnode *node, FreeFunction free_k,
                  FreeFunction free_v) {
    if (!node) return;
    destroy_help(node->left, free_k, free_v);
    destroy_help(node->right, free_k, free_v);
    free_k(node->key);
    free_v(node->value);
    free(node);
}


void destroy_Dictionary(Dictionary dict) {
    destroy_help(dict->root, dict->free_k, dict->free_v);
    free(dict);
}


struct bstnode *new_node(void *k, void *v) {
    struct bstnode *new = malloc(sizeof(struct bstnode));
    new->key = k;
    new->value = v;
    new->left = NULL;
    new->right = NULL;
    return new;
}


void insert(Dictionary dict, void *k, void *v) {
    struct bstnode *cur = dict->root;
    struct bstnode *prev = NULL;
    int res = 0;
    while (cur) {
        res = dict->f_comp(k, cur->key);
        if (res == 0) {
            dict->free_v(cur->value);
            cur->value = v;
            return;
        }
        
        prev = cur;
        if (res < 0) cur = cur->left;
        else cur = cur->right;
    }
    
    struct bstnode *new = new_node(k, v);
    if (prev == NULL) {
        dict->root = new;
    } else if (res < 0) {
        prev->left = new;
    } else prev->right = new;
}


void *lookup(Dictionary dict, void *k) {
    struct bstnode *cur = dict->root;
    while (cur) {
        int res = dict->f_comp(k, cur->key);
        if (res < 0) {
            cur = cur->left;
        } else if (res > 0) {
            cur = cur->right;
        } else {
            return cur->value;
        }
    }
    return NULL;
}




