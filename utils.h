/**
 * utils.h - 工具函数
 */

#ifndef UTILS_H
#define UTILS_H

#include "structs.h"

/* 当前使用的数据结构类型 */
typedef enum { USE_LIST, USE_AVL, USE_HASH } StructType;

/* 初始化/切换数据结构 */
void init(StructType type);
const char* get_type_name(void);
StructType get_current_type(void);

/* 统一操作接口 */
int insert_record(Record *r);
Record* find_record(const char *id, const char *cid);
int delete_record(const char *id, const char *cid);
int update_score(const char *id, const char *cid, int new_score);
void traverse_records(void (*visit)(Record *));
int get_size(void);

/* 获取实例 */
LinkedList* get_list_instance(void);
AVLTree* get_avl_instance(void);
HashTable* get_hash_instance(void);

/* AVL树遍历辅助 */
void collect_avl_all(ANode *node, Record **arr, int *idx);
void collect_avl_filter(ANode *node, Record **arr, int *idx, int min, int max);

/* 统计和过期 */
void stats_avl(ANode *node, int *sum, int *max, int *min,
               int *a, int *b, int *c, int *d, int *e);
void count_expire_avl(ANode *node, int *count);
void delete_expire_avl(ANode *node, int *deleted);

/* 文件读写 */
int load_csv(const char *filename);
int save_csv(const char *filename);
int generate_data(const char *filename, int count);

void stats_avl(ANode *node, int *sum, int *max, int *min,
               int *a, int *b, int *c, int *d, int *e);
 void count_expire_avl(ANode *node, int *count);
void delete_expire_avl(ANode *node, int *deleted);

#endif
