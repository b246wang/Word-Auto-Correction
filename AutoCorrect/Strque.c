//
//  Strque.c
//  AutoCorrect
//
//  Created by Brody Wang on 2015-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "Strque.h"


/**
 Bincan Wang
 File: strqueue.c
 Implementation for strqueue.h
 */
struct llnode {
    char *item;
    struct llnode *next;
};

struct strqueue {
    int len;
    struct llnode *head;
    struct llnode *back;
};


StrQueue create_StrQueue(void) {
    StrQueue new = malloc(sizeof(struct strqueue));
    new->len = 0;
    new->head = NULL;
    new->back = NULL;
    return new;
}


void destroy_StrQueue(StrQueue sq) {
    struct llnode *cur = sq->head;
    struct llnode *trash;
    while (cur) {
        trash = cur->next;
        if (cur->item != NULL) free(cur->item);
        free(cur);
        cur = trash;
    }
    free(sq);
}


void sq_add_back(StrQueue sq, const char *str) {
    struct llnode *new = malloc(sizeof(struct llnode));
    new->item = malloc(strlen(str) * sizeof(char) + 1);
    strcpy(new->item, str);
    new->next = NULL;
    if (sq->head == NULL) {
        sq->head = new;
        sq->back = new;
    } else {
        sq->back->next = new;
        sq->back = new;
    }
    sq->len++;
}


char *sq_remove_front(StrQueue sq) {
    if (sq->head == NULL) return NULL;
    struct llnode *byebye = sq->head;
    sq->head = sq->head->next;
    if (sq->head == NULL) sq->back = NULL;
    char *s = byebye->item;
    free(byebye);
    sq->len--;
    return s;
}


int sq_length(StrQueue sq) {
    return sq->len;
}





