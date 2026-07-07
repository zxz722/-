/**
 * list.c - 双向链表实现
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
    n->prev = L->tail;  //新节点的prev指向当前尾 
    n->next = NULL;     //新节点是尾，next为NULL 
    L->tail->next = n;  //原尾的next指向新节点 
    L->tail = n;        //更新尾指针 
    L->size++;
    return 1;
}

/* 根据学号+课程编号查找 */
Record* list_find(LinkedList *L, const char *id, const char *cid)
{
    if (!L) return NULL;
    char key[21], k[21];
    make_key(key, id, cid);  //生成要查找的键值 
    DNode *p = L->head->next;
    while(p){
    	make_key(k,p->data->student_id,p->data->course_id);
    	if(strcmp(k,key)==0){
    		p->prev->next=p->next;
    		p->next=p;
    		return 1;
		}
	}
    return NULL;   //遍历完都没找到 
}

/* 根据学号+课程编号删除 */
int list_delete(LinkedList *L, const char *id, const char *cid)
{
    if (!L) return 0;
    char key[21], k[21];
    make_key(key, id, cid);  //生成键值 
    DNode *p = L->head->next;
    while (p) {
        make_key(k, p->data->student_id, p->data->course_id);
        if (strcmp(k, key) == 0) {  //找到了要删除的节点P 
            p->prev->next = p->next;  //前驱跳过p 
            if (p->next) {        //如果p不是尾节点 
                p->next->prev = p->prev;   //后继跳过p 
            } else {
                L->tail = p->prev;  //如果p是尾节点，更新tail 
            }
            free(p->data);  //释放数据内存 
            free(p);        //释放节点内存 
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
    while (p) {
        visit(p->data);
        p = p->next;
    }
}

/* 释放 */
void list_destroy(LinkedList *L)
{
    if (!L) return;
    DNode *p = L->head, *q;
    while (p) {
        q = p->next;
        free(p->data);
        free(p);
        p = q;
    }
    free(L);
}

int list_size(LinkedList *L)
{
    return L ? L->size : 0;
}
