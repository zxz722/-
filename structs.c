/**
 * structs.c - 链表、AVL树、哈希表的操作函数
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

/* ============================================================
   工具函数：生成键值（学号|课程编号）
   ============================================================ */
static void make_key(char *key, const char *id, const char *cid)
{
    sprintf(key, "%s|%s", id, cid);
}

/* ============================================================
   第一部分：双向链表
   ============================================================ */

/* 创建空链表 */
LinkedList* list_create(void)
{
    LinkedList *L = (LinkedList *)malloc(sizeof(LinkedList));
    L->head = (DNode *)malloc(sizeof(DNode));
    L->head->data = NULL;
    L->head->prev = NULL;
    L->head->next = NULL;
    L->tail = L->head;
    L->size = 0;
    return L;
}

/* 在尾部插入 */
int list_insert(LinkedList *L, Record *r)
{
    if (!L || !r) return 0;
    DNode *n = (DNode *)malloc(sizeof(DNode));
    n->data = r;
    n->prev = L->tail;
    n->next = NULL;
    L->tail->next = n;
    L->tail = n;
    L->size++;
    return 1;
}

/* 根据学号+课程编号查找 */
Record* list_find(LinkedList *L, const char *id, const char *cid)
{
    if (!L) return NULL;
    char key[21], k[21];
    make_key(key, id, cid);
    DNode *p = L->head->next;
    while (p) {
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) return p->data;
        p = p->next;
    }
    return NULL;
}

/* 根据学号+课程编号删除 */
int list_delete(LinkedList *L, const char *id, const char *cid)
{
    if (!L) return 0;
    char key[21], k[21];
    make_key(key, id, cid);
    DNode *p = L->head->next;
    while (p) {
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) {
            p->prev->next = p->next;
            if (p->next) p->next->prev = p->prev;
            else L->tail = p->prev;
            free(p->data);
            free(p);
            L->size--;
            return 1;
        }
        p = p->next;
    }
    return 0;
}

/* 遍历 */
void list_traverse(LinkedList *L, void (*visit)(Record *))
{
    if (!L) return;
    DNode *p = L->head->next;
    while (p) { visit(p->data); p = p->next; }
}

/* 释放 */
void list_destroy(LinkedList *L)
{
    if (!L) return;
    DNode *p = L->head, *q;
    while (p) { q = p->next; free(p->data); free(p); p = q; }
    free(L);
}

int list_size(LinkedList *L) { return L ? L->size : 0; }


/* ============================================================
   第二部分：AVL树
   ============================================================ */

/* 高度 */
static int get_h(ANode *n) { return n ? n->height : 0; }

/* 更新高度 */
static void set_h(ANode *n) {
    int a = get_h(n->left), b = get_h(n->right);
    n->height = (a > b ? a : b) + 1;
}

/* 平衡因子 */
static int bf(ANode *n) { return n ? get_h(n->left) - get_h(n->right) : 0; }

/* 右旋 */
static ANode* R(ANode *y) {
    ANode *x = y->left, *T = x->right;
    x->right = y; y->left = T;
    set_h(y); set_h(x);
    return x;
}

/* 左旋 */
static ANode* L(ANode *x) {
    ANode *y = x->right, *T = y->left;
    y->left = x; x->right = T;
    set_h(x); set_h(y);
    return y;
}

/* 递归插入 */
static ANode* insert_node(ANode *n, Record *r)
{
    if (!n) {
        ANode *nn = (ANode *)malloc(sizeof(ANode));
        nn->data = r; nn->left = nn->right = NULL; nn->height = 1;
        return nn;
    }
    char key[21], k[21];
    make_key(key, r->student_id, r->course_id);
    make_key(k, n->data->student_id, n->data->course_id);
    int cmp = strcmp(key, k);
    
    if (cmp < 0) n->left = insert_node(n->left, r);
    else if (cmp > 0) n->right = insert_node(n->right, r);
    else { free(n->data); n->data = r; return n; }
    
    set_h(n);
    int bal = bf(n);
    if (bal > 1 && cmp < 0) return R(n);
    if (bal < -1 && cmp > 0) return L(n);
    if (bal > 1 && cmp > 0) { n->left = L(n->left); return R(n); }
    if (bal < -1 && cmp < 0) { n->right = R(n->right); return L(n); }
    return n;
}

/* 递归删除 */
static ANode* delete_node(ANode *n, const char *id, const char *cid)
{
    if (!n) return NULL;
    char key[21], k[21];
    make_key(key, id, cid);
    make_key(k, n->data->student_id, n->data->course_id);
    int cmp = strcmp(key, k);
    
    if (cmp < 0) n->left = delete_node(n->left, id, cid);
    else if (cmp > 0) n->right = delete_node(n->right, id, cid);
    else {
        if (!n->left || !n->right) {
            ANode *t = n->left ? n->left : n->right;
            free(n->data); free(n);
            return t;
        } else {
            ANode *t = n->right;
            while (t->left) t = t->left;
            free(n->data);
            n->data = t->data;
            n->right = delete_node(n->right, t->data->student_id, t->data->course_id);
        }
    }
    if (!n) return NULL;
    set_h(n);
    int bal = bf(n);
    if (bal > 1 && bf(n->left) >= 0) return R(n);
    if (bal > 1 && bf(n->left) < 0) { n->left = L(n->left); return R(n); }
    if (bal < -1 && bf(n->right) <= 0) return L(n);
    if (bal < -1 && bf(n->right) > 0) { n->right = R(n->right); return L(n); }
    return n;
}

/* 创建AVL树（只保留这一个） */
AVLTree* avl_create(void)
{
    AVLTree *T = (AVLTree *)malloc(sizeof(AVLTree));
    T->root = NULL;
    T->size = 0;
    return T;
}

int avl_insert(AVLTree *T, Record *r)
{
    if (!T || !r) return 0;
    T->root = insert_node(T->root, r);
    T->size++;
    return 1;
}

Record* avl_find(AVLTree *T, const char *id, const char *cid)
{
    if (!T) return NULL;
    char key[21], k[21];
    make_key(key, id, cid);
    ANode *p = T->root;
    while (p) {
        make_key(k, p->data->student_id, p->data->course_id);
        int cmp = strcmp(key, k);
        if (cmp == 0) return p->data;
        if (cmp < 0) p = p->left;
        else p = p->right;
    }
    return NULL;
}

int avl_delete(AVLTree *T, const char *id, const char *cid)
{
    if (!T) return 0;
    T->root = delete_node(T->root, id, cid);
    T->size--;
    return 1;
}

static void inorder(ANode *n, void (*visit)(Record *))
{
    if (!n) return;
    inorder(n->left, visit);
    visit(n->data);
    inorder(n->right, visit);
}

void avl_traverse(AVLTree *T, void (*visit)(Record *))
{
    if (T) inorder(T->root, visit);
}

void avl_destroy(AVLTree *T)
{
    free(T);
}

int avl_size(AVLTree *T) { return T ? T->size : 0; }


/* ============================================================
   第三部分：哈希表
   ============================================================ */

/* BKDR哈希 */
static unsigned int hash(const char *key, int cap)
{
    unsigned int h = 0;
    while (*key) h = h * 131 + (*key++);
    return h % cap;
}

/* 创建哈希表 */
HashTable* hash_create(int cap)
{
    if (cap < 1) cap = 101;
    HashTable *H = (HashTable *)malloc(sizeof(HashTable));
    H->buckets = (HNode **)calloc(cap, sizeof(HNode *));
    H->capacity = cap;
    H->size = 0;
    return H;
}

int hash_insert(HashTable *H, Record *r)
{
    if (!H || !r) return 0;
    char key[21];
    make_key(key, r->student_id, r->course_id);
    int idx = hash(key, H->capacity);
    
    HNode *p = H->buckets[idx];
    while (p) {
        char k[21];
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) return 0;
        p = p->next;
    }
    
    HNode *n = (HNode *)malloc(sizeof(HNode));
    n->data = r;
    n->next = H->buckets[idx];
    H->buckets[idx] = n;
    H->size++;
    return 1;
}

Record* hash_find(HashTable *H, const char *id, const char *cid)
{
    if (!H) return NULL;
    char key[21];
    make_key(key, id, cid);
    int idx = hash(key, H->capacity);
    HNode *p = H->buckets[idx];
    while (p) {
        char k[21];
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) return p->data;
        p = p->next;
    }
    return NULL;
}

int hash_delete(HashTable *H, const char *id, const char *cid)
{
    if (!H) return 0;
    char key[21];
    make_key(key, id, cid);
    int idx = hash(key, H->capacity);
    HNode *p = H->buckets[idx], *prev = NULL;
    while (p) {
        char k[21];
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) {
            if (prev) prev->next = p->next;
            else H->buckets[idx] = p->next;
            free(p->data); free(p);
            H->size--;
            return 1;
        }
        prev = p;
        p = p->next;
    }
    return 0;
}

void hash_traverse(HashTable *H, void (*visit)(Record *))
{
    if (!H) return;
    int i;
    for (i = 0; i < H->capacity; i++) {
        HNode *p = H->buckets[i];
        while (p) {
            visit(p->data);
            p = p->next;
        }
    }
}

void hash_destroy(HashTable *H)
{
    if (!H) return;
    int i;
    for (i = 0; i < H->capacity; i++) {
        HNode *p = H->buckets[i];
        while (p) {
            HNode *tmp = p;
            p = p->next;
            free(tmp->data);
            free(tmp);
        }
    }
    free(H->buckets);
    free(H);
}

int hash_size(HashTable *H) { return H ? H->size : 0; }
