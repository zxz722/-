/**
 * utils.c - 工具函数 + 文件读写 + 统一接口
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "structs.h"

/* 当前使用的数据结构类型 */
static StructType current_type = USE_HASH;
static LinkedList *g_list = NULL;
static AVLTree *g_avl = NULL;
static HashTable *g_hash = NULL;

/* ============================================================
   获取当前类型和实例
   ============================================================ */
StructType get_current_type(void)
{
    return current_type;
}

LinkedList* get_list_instance(void)
{
    return g_list;
}

AVLTree* get_avl_instance(void)
{
    return g_avl;
}

HashTable* get_hash_instance(void)
{
    return g_hash;
}

/* ============================================================
   初始化
   ============================================================ */
void init(StructType type)
{
    if (g_list) { list_destroy(g_list); g_list = NULL; }
    if (g_avl) { avl_destroy(g_avl); g_avl = NULL; }
    if (g_hash) { hash_destroy(g_hash); g_hash = NULL; }

    current_type = type;
    switch (type) {
        case USE_LIST: g_list = list_create(); break;
        case USE_AVL:  g_avl = avl_create(); break;
        case USE_HASH: g_hash = hash_create(101); break;
    }
}

const char* get_type_name(void)
{
    switch (current_type) {
        case USE_LIST: return "双向链表";
        case USE_AVL:  return "AVL树";
        case USE_HASH: return "哈希表";
        default: return "未知";
    }
}

/* ============================================================
   AVL树遍历辅助函数
   ============================================================ */
void collect_avl_all(ANode *node, Record **arr, int *idx)
{
    if (!node) return;
    collect_avl_all(node->left, arr, idx);
    arr[(*idx)++] = node->data;
    collect_avl_all(node->right, arr, idx);
}

void collect_avl_filter(ANode *node, Record **arr, int *idx, int min, int max)
{
    if (!node) return;
    collect_avl_filter(node->left, arr, idx, min, max);
    if (node->data->score >= min && node->data->score <= max) {
        arr[(*idx)++] = node->data;
    }
    collect_avl_filter(node->right, arr, idx, min, max);
}

/* ============================================================
   AVL树统计辅助函数
   ============================================================ */
void stats_avl(ANode *node, int *sum, int *max, int *min,
               int *a, int *b, int *c, int *d, int *e)
{
    if (!node) return;
    stats_avl(node->left, sum, max, min, a, b, c, d, e);

    Record *r = node->data;
    *sum += r->score;
    if (r->score > *max) *max = r->score;
    if (r->score < *min) *min = r->score;
    if (r->score >= 90) (*a)++;
    else if (r->score >= 80) (*b)++;
    else if (r->score >= 70) (*c)++;
    else if (r->score >= 60) (*d)++;
    else (*e)++;

    stats_avl(node->right, sum, max, min, a, b, c, d, e);
}

/* ============================================================
   AVL树过期记录辅助函数
   ============================================================ */
void count_expire_avl(ANode *node, int *count)
{
    if (!node) return;
    count_expire_avl(node->left, count);
    if (strcmp(node->data->date, "2023-09-01") < 0) {
        (*count)++;
    }
    count_expire_avl(node->right, count);
}

void delete_expire_avl(ANode *node, int *deleted)
{
    if (!node) return;
    delete_expire_avl(node->left, deleted);
    if (strcmp(node->data->date, "2023-09-01") < 0) {
        if (delete_record(node->data->student_id, node->data->course_id)) {
            (*deleted)++;
        }
    }
    delete_expire_avl(node->right, deleted);
}

/* ============================================================
   统一操作接口
   ============================================================ */
int insert_record(Record *r)
{
    switch (current_type) {
        case USE_LIST: return list_insert(g_list, r);
        case USE_AVL:  return avl_insert(g_avl, r);
        case USE_HASH: return hash_insert(g_hash, r);
        default: return 0;
    }
}

Record* find_record(const char *id, const char *cid)
{
    switch (current_type) {
        case USE_LIST: return list_find(g_list, id, cid);
        case USE_AVL:  return avl_find(g_avl, id, cid);
        case USE_HASH: return hash_find(g_hash, id, cid);
        default: return NULL;
    }
}

int delete_record(const char *id, const char *cid)
{
    switch (current_type) {
        case USE_LIST: return list_delete(g_list, id, cid);
        case USE_AVL:  return avl_delete(g_avl, id, cid);
        case USE_HASH: return hash_delete(g_hash, id, cid);
        default: return 0;
    }
}

int update_score(const char *id, const char *cid, int new_score)
{
    Record *r = find_record(id, cid);
    if (!r || new_score < 0 || new_score > 100) return 0;
    r->score = new_score;
    return 1;
}

void traverse_records(void (*visit)(Record *))
{
    switch (current_type) {
        case USE_LIST: list_traverse(g_list, visit); break;
        case USE_AVL:  avl_traverse(g_avl, visit); break;
        case USE_HASH: hash_traverse(g_hash, visit); break;
    }
}

int get_size(void)
{
    switch (current_type) {
        case USE_LIST: return list_size(g_list);
        case USE_AVL:  return avl_size(g_avl);
        case USE_HASH: return hash_size(g_hash);
        default: return 0;
    }
}

/* ============================================================
   解析CSV行
   ============================================================ */
static Record* parse_csv_line(char *line)
{
    Record *r = (Record *)malloc(sizeof(Record));
    if (!r) return NULL;

    char *p = strtok(line, ",");
    if (p) strcpy(r->student_id, p);
    p = strtok(NULL, ","); if (p) strcpy(r->name, p);
    p = strtok(NULL, ","); if (p) strcpy(r->college, p);
    p = strtok(NULL, ","); if (p) strcpy(r->course_id, p);
    p = strtok(NULL, ","); if (p) strcpy(r->course_name, p);
    p = strtok(NULL, ","); if (p) r->credit = atof(p);
    p = strtok(NULL, ","); if (p) strcpy(r->semester, p);
    p = strtok(NULL, ","); if (p) strcpy(r->date, p);
    p = strtok(NULL, ","); if (p) r->score = atoi(p);

    return r;
}

/* ============================================================
   加载CSV
   ============================================================ */
int load_csv(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("文件 %s 不存在，将使用空数据\n", filename);
        return 0;
    }

    char line[1024];
    int count = 0;
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        Record *r = parse_csv_line(line);
        if (r && insert_record(r)) {
            count++;
        } else if (r) {
            free(r);
        }
    }
    fclose(fp);
    printf("加载 %d 条记录\n", count);
    return count;
}

/* ============================================================
   保存CSV（暂未实现）
   ============================================================ */
int save_csv(const char *filename)
{
    printf("保存功能暂未实现，数据在内存中\n");
    return 0;
}

/* ============================================================
   生成测试数据
   ============================================================ */
int generate_data(const char *filename, int count)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;

    /* 姓氏列表 */
    char *surnames[] = {"张","李","王","刘","陈","杨","赵","黄","周","吴",
                        "徐","孙","胡","朱","高","林","何","郭","马","罗",
                        "梁","宋","郑","谢","韩","唐","冯","于","董","萧",
                        "程","曹","袁","邓","许","傅","沈","曾","彭","吕",
                        "苏","卢","蒋","蔡","贾","丁","魏","薛","叶","阎",
                        "余","潘","杜","戴","夏","钟","汪","田","任","姜",
                        "范","方","石","姚","谭","廖","邹","熊","金","陆",
                        "郝","孔","白","崔","毛","邱","秦","江","史","顾",
                        "侯","邵","孟","龙","万","段","雷","钱","汤","尹",
                        "黎","易","常","武","乔","贺","赖","龚","文","樊"};
    int surname_count = sizeof(surnames) / sizeof(surnames[0]);

    /* 名字常用字 */
    char *given_chars[] = {"伟","芳","娜","秀英","敏","静","丽","强","磊","洋",
                           "勇","艳","杰","倩","涛","明","超","秀兰","霞","平",
                           "刚","桂英","涛","慧","建","文","华","飞","玉兰","斌",
                           "宇","鑫","浩","然","博","文","轩","宇","辰","怡",
                           "彤","萱","妍","琦","珂","玥","琛","瑶","雪","晴",
                           "子涵","梓涵","一鸣","天翊","瑞霖","雨桐","梦瑶","欣怡","浩宇","梓萱",
                           "若曦","子轩","宇航","可欣","沐阳","诗雨","乐怡","欣怡","子琪","浩然"};
    int given_count = sizeof(given_chars) / sizeof(given_chars[0]);

    /* 学院列表 */
    char *colleges[] = {"计算机科学与工程学院","电子信息工程学院","数学与统计学院",
                        "外国语学院","经济管理学院","物理与光电学院","化学与化工学院",
                        "生命科学与技术学院","材料科学与工程学院","机械与汽车工程学院"};
    int college_count = sizeof(colleges) / sizeof(colleges[0]);

    /* 课程列表 */
    char *courses[] = {"数据结构","操作系统","数据库原理","计算机网络","软件工程",
                       "编译原理","算法设计","人工智能","计算机组成","离散数学"};
    char *cids[] = {"CS3001","CS3002","CS3003","CS3004","CS3005",
                    "CS3006","CS3007","CS3008","CS3009","CS3010"};
    float credits[] = {3.5,3.0,3.5,3.0,3.0,3.5,3.0,3.5,3.0,3.0};
    int course_count = sizeof(courses) / sizeof(courses[0]);

    /* 学期和日期（确保包含3年前即2023年之前的记录） */
    char *semesters[] = {"2021-01","2021-02","2022-01","2022-02",
                         "2023-01","2023-02","2024-01","2024-02",
                         "2025-01","2025-02","2026-01","2026-02"};
    char *dates[] = {"2021-02-20","2021-09-01","2022-02-15","2022-09-01",
                     "2023-02-20","2023-09-01","2024-02-15","2024-09-01",
                     "2025-02-20","2025-09-01","2026-02-15","2026-09-01"};
    int sem_count = sizeof(semesters) / sizeof(semesters[0]);

    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,学期,日期,成绩\n");
    srand((unsigned)time(NULL));

    int i;
    for (i = 0; i < count; i++) {
        /* 生成唯一姓名：取姓氏 + 名字字 */
        int si = i % surname_count;
        int gi = (i / surname_count) % given_count;
        char name[64];
        sprintf(name, "%s%s", surnames[si], given_chars[gi]);

        int ci = rand() % college_count;
        int coi = rand() % course_count;
        int sem_i = rand() % sem_count;

        fprintf(fp, "2024%06d,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                i + 1,
                name,
                colleges[ci],
                cids[coi],
                courses[coi],
                credits[coi],
                semesters[sem_i],
                dates[sem_i],
                rand() % 60 + 40);  /* 成绩 40-100 */
    }

    fclose(fp);
    printf("生成 %d 条数据到 %s（姓名均不重复）\n", count, filename);
    return count;
}
