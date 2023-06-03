/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#pragma once

#include <stddef.h> // size_t

#define COMPARE_UNEQUAL 0
#define COMPARE_EQUAL 1

#define COMPARE_CASE_INSENSETIVE 0
#define COMPARE_CASE_SENSETIVE 1

int compare1(const char* user_str, size_t user_str_len, const char* templ_str, int case_sensetive);

typedef struct word_node {
    const char      * str;
    size_t            len;
    struct word_node* next;
} word_node;

typedef struct word_list {
    struct word_node* first;
    struct word_node* last;
    size_t            size;
} word_list;

typedef struct line_node {
    const char      * str;
    size_t            len;
    struct word_list* words;
    struct line_node* next;
} line_node;

typedef struct line_list {
    struct line_node* first;
    struct line_node* last;
    size_t            size;
} line_list;

// ===== Word Node ===== //
word_node* word_node_create1(const char* str, size_t len);
word_node* word_node_destroy1(word_node* n);
word_node* word_node_destroy1_rec(word_node* n);

// ===== Word List ===== //
word_list* word_list_create1();
word_list* word_list_destroy1(word_list* l);

void word_list_push1(word_list* l, word_node* n);
word_node* word_list_get1(word_list* l, size_t i);

// ===== Line Node ==== //
line_node* line_node_create1(const char* str, size_t len);
word_node* line_node_destroy1(line_node* n);
word_node* line_node_destroy1_rec(line_node* n);

// ===== Line List ===== //
line_list* line_list_create1();
line_list* line_list_destroy1(line_list* l);

void line_list_push1(line_list* l, line_node* n);
line_node* line_list_get1(line_list* l, size_t i);

// ===== Parser ===== //
word_list* parse_words1(const char* str, size_t len);
line_list* parse_lines1(const char* str, size_t len);