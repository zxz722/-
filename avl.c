/**
 * avl.c - AVL树实现
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
   第二部分：AVL树
   ============================================================ */

/* 高度 */
static int get_h(ANode *n)
{
    return n ? n->height : 0;
}

/* 更新高度 */
static void set_h(ANode *n)
{
    int a = get_h(n->left);
    int b = get_h(n->right);
    n->height = (a > b ? a : b) + 1;
}

/* 平衡因子 */
static int bf(ANode *n)
{
    return n ? get_h(n->left) - get_h(n->right) : 0;
}

/* 右旋（LL情况） */
static ANode* R(ANode *y)
{
    ANode *x = y->left;  //x是y的左孩子（要提上去） 
    ANode *T = x->right;  //T是x的右子树（要过继给y） 
    x->right = y;         //y变成x的右孩子 
    y->left = T;          //T变成y的左孩子 
    set_h(y);             //更新高度 
    set_h(x);
    return x;             //x成为新的根 
}

/* 左旋（RR情况） */
static ANode* L(ANode *x)
{
    ANode *y = x->right;
    ANode *T = y->left;
    y->left = x;
    x->right = T;
    set_h(x);
    set_h(y);
    return y;
}

/* 递归插入 */
static ANode* insert_node(ANode *n, Record *r)
{
    if (!n) {
        ANode *nn = (ANode *)malloc(sizeof(ANode));
        nn->data = r;
        nn->left = NULL;
        nn->right = NULL;
        nn->height = 1;
        return nn;
    }

    char key[21], k[21];
    make_key(key, r->student_id, r->course_id);
    make_key(k, n->data->student_id, n->data->course_id);
    int cmp = strcmp(key, k);

    if (cmp < 0) {
        n->left = insert_node(n->left, r);
    } else if (cmp > 0) {
        n->right = insert_node(n->right, r);
    } else {
        free(n->data);
        n->data = r;
        return n;
    }

    set_h(n);
    int bal = bf(n);

    /* LL：左孩子的左子树插入 → 右旋 */
    if (bal > 1 && cmp < 0) return R(n);

    /* RR：右孩子的右子树插入 → 左旋 */
    if (bal < -1 && cmp > 0) return L(n);

    /* LR：左孩子的右子树插入 → 先左旋再右旋 */
    if (bal > 1 && cmp > 0) {
        n->left = L(n->left);
        return R(n);
    }

    /* RL：右孩子的左子树插入 → 先右旋再左旋 */
    if (bal < -1 && cmp < 0) {
        n->right = R(n->right);
        return L(n);
    }

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

    if (cmp < 0) {
        n->left = delete_node(n->left, id, cid);
    } else if (cmp > 0) {
        n->right = delete_node(n->right, id, cid);
    } else {
        if (!n->left || !n->right) {
            ANode *t = n->left ? n->left : n->right;
            free(n->data);
            free(n);
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
    if (bal > 1 && bf(n->left) < 0) {
        n->left = L(n->left);
        return R(n);
    }
    if (bal < -1 && bf(n->right) <= 0) return L(n);
    if (bal < -1 && bf(n->right) > 0) {
        n->right = R(n->right);
        return L(n);
    }

    return n;
}

/* 创建AVL树 */
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

int avl_size(AVLTree *T)
{
    return T ? T->size : 0;
}
