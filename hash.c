/**
 * hash.c - 哈希表实现（链地址法）
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
   第三部分：哈希表
   ============================================================ */

/* BKDR哈希函数 */
static unsigned int hash(const char *key, int cap)
{
    unsigned int h = 0;
    while (*key) {
        h = h * 131 + (*key++);
    }
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

/* 插入（头插法） */
int hash_insert(HashTable *H, Record *r)
{
    /* ① 空指针检查 */
    if (!H || !r) return 0;

    /* ② 生成键值：学号|课程编号（唯一标识） */
    char key[21];
    make_key(key, r->student_id, r->course_id);

    /* ③ 计算桶索引 */
    int idx = hash(key, H->capacity);

    /* ④ 检查是否已存在（防止重复插入） */
    HNode *p = H->buckets[idx];
    while (p) {
        char k[21];
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) return 0;  /* 已存在，插入失败 */
        p = p->next;
    }

    /* ⑤ 头插法：新节点插入链表头部 */
    HNode *n = (HNode *)malloc(sizeof(HNode));
    n->data = r;
    n->next = H->buckets[idx];    /* 新节点指向原链表头 */
    H->buckets[idx] = n;          /* 更新桶指针指向新节点 */
    H->size++;
    return 1;
}

/* 查找 */
Record* hash_find(HashTable *H, const char *id, const char *cid)
{
    /* ① 空指针检查 */
    if (!H) return NULL;

    /* ② 生成键值 */
    char key[21];
    make_key(key, id, cid);

    /* ③ 计算桶索引 */
    int idx = hash(key, H->capacity);

    /* ④ 遍历该桶的链表，逐个比较 */
    HNode *p = H->buckets[idx];
    while (p) {
        char k[21];
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) {
            return p->data;       /* 找到 */
        }
        p = p->next;              /* 继续下一个 */
    }
    return NULL;                  /* 未找到 */
}

/* 删除 */
int hash_delete(HashTable *H, const char *id, const char *cid)
{
    if (!H) return 0;

    char key[21];
    make_key(key, id, cid);
    int idx = hash(key, H->capacity);

    HNode *p = H->buckets[idx];
    HNode *prev = NULL;

    while (p) {
        char k[21];
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) {
            if (prev) {
                prev->next = p->next;
            } else {
                H->buckets[idx] = p->next;
            }
            free(p->data);
            free(p);
            H->size--;
            return 1;
        }
        prev = p;
        p = p->next;
    }
    return 0;
}

/* 遍历 */
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

/* 释放 */
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

int hash_size(HashTable *H)
{
    return H ? H->size : 0;
}
