/**
 * structs.h - 数据结构定义
 * 定义选课记录、链表、AVL树、哈希表
 */

//任务1：数据结构设计与基本操作

#ifndef STRUCTS_H
#define STRUCTS_H

/* ========== 1. 选课记录结构体 ========== */
typedef struct {
    char student_id[13];    // 学号
    char name[32];          // 姓名
    char college[64];       // 学院
    char course_id[9];      // 课程编号
    char course_name[128];  // 课程名称
    float credit;           // 学分
    char semester[7];       // 学期
    char date[11];          // 日期
    int score;              // 成绩
} Record;

/* ========== 2. 双向链表节点 ========== */
typedef struct DNode {
    Record *data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/* ========== 3. AVL树节点 ========== */
typedef struct ANode {
    Record *data;
    struct ANode *left;
    struct ANode *right;
    int height;
} ANode;

/* ========== 4. 哈希表节点 ========== */
typedef struct HNode {
    Record *data;
    struct HNode *next;
} HNode;

/* ========== 5. 三种数据结构 ========== */
typedef struct {
    DNode *head;   // 链表头
    DNode *tail;   // 链表尾
    int size;      // 大小
} LinkedList;

typedef struct {
    ANode *root;   // 树根
    int size;      // 大小
} AVLTree;

typedef struct {
    HNode **buckets;  // 桶数组
    int capacity;     // 桶数量
    int size;         // 大小
} HashTable;

LinkedList* list_create(void);

#endif
