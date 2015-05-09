//
//  main.c
//  AutoCorrect
//
//  Created by Brody Wang on 2015-04-09.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "Dictionary.h"
#include "FileToStrque.h"

// this is an apostrophe (')
const char apostrophe = '\'';

// a simple void * wrapper for strcmp
int strcmp_wrapper(const void *a, const void *b) {
    assert(a);
    assert(b);
    const char *str_a = a;
    const char *str_b = b;
    return strcmp(str_a, str_b);
}

// Because the Dictionary ADT uses key/value pairs,
// and for the wordlist dictionary, we only need keys
// we can use the VALID pointer for every value
int valid = 1;
void *VALID = &valid;

// but we don't want our Dictionary ADT free-ing VALID,
// so we need a function that does not free anything
// (to pass to the create_Dictionary function)
void do_nothing(void *p) {
    return;
}

bool not_word(const char *a) {
    assert(a);
    int len = strlen(a);
    bool flag = false;
    for (int i = 0; i < len; ++i) {
        if (a[i] < 'A' || (a[i] > 'Z' && a[i] < 'a' - 1)||
            a[i] > 'z') {
            if (i < len - 1) {
                flag = true; break;
            }
            if (a[i] != ',' && a[i] != '.' && a[i] !='!' && a[i] != apostrophe) {
                printf("%d ", i);
                flag = true;
            }
        }
    }
        return flag;
}


bool in_list(Dictionary d, const char *b) {
    assert(b);
    int len = strlen(b);
    char *copy = malloc(sizeof(char) * len + 1);
    strcpy(copy, b);
    char last = b[len-1];
    //printf("%c\n", last);
    if (last == ',' || last == '.' || last == '!' || last == apostrophe) {
        copy[len-1] = 0;
        --len;
    }
    for (int i = 0; i < len; ++i) {
        if(copy[i] >= 'A' && copy[i] <= 'Z') copy[i] += 32;
    }
    //printf("%s\n", copy);
    bool flag = true;
    if (lookup(d, copy)) flag = true;
    else flag = false;
    free(copy);
    return flag;
}

int main(void) {
    // create the wordlist dictionary
    Dictionary wordlist = create_Dictionary(strcmp_wrapper, free, do_nothing);
    StrQueue sq = file2StrQueue("wordlist.txt");
    while (sq_length(sq)) {
        char *word = sq_remove_front(sq);
        insert(wordlist, word, VALID);
    }
    destroy_StrQueue(sq);
    sq = NULL;
    
    Dictionary autocor = create_Dictionary(strcmp_wrapper, free, free);
    sq = file2StrQueue("autocorrect.txt");
    while (sq_length(sq)) {
        char *word = sq_remove_front(sq);
        char *val = sq_remove_front(sq);
        //printf("%s %s\n", word, val);
        insert(autocor, word, val);
    }
    destroy_StrQueue(sq);
    sq = NULL;
    
    sq = file2StrQueue(NULL);
    int flag = 0;
    while (sq_length(sq)) {
        
        char *word = sq_remove_front(sq);
        char *in_cor = lookup(autocor, word);
        //printf("%s\n", in_cor);
        if (not_word(word)) {
            printf("%s(unrecgonized)", word);
        } else if (in_list(wordlist, word)) {
            printf("%s", word);
        } else if (in_cor) {
            printf("%s(corrected)", in_cor);
        } else {
            printf("%s[not_in_list]", word);
        }
        flag++;
        // add a new line between every ten words
        if (flag - 10 == 0) {
            printf("\n");
            flag = 0;
        } else if (sq_length(sq)) {
            printf(" ");
        } else printf("\n");
        
        free(word);
    }
    
    
    
    // free memory
    destroy_StrQueue(sq);
    destroy_Dictionary(autocor);
    destroy_Dictionary(wordlist);
}



