/**
 * main.c - 主程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include "utils.h"

#define DATA_FILE "data_1000.csv"   // 加载1000条

/* ============================================================
   打印记录
   ============================================================ */
void print_record(Record *r)
{
    printf("%-12s %-8s %-20s %-8s %-16s %-4.1f %-8s %-12s %-4d\n",
           r->student_id, r->name, r->college, r->course_id,
           r->course_name, r->credit, r->semester, r->date, r->score);
}

void print_header(void)
{
    printf("\n");
    printf("%-12s %-8s %-20s %-8s %-16s %-4s %-8s %-12s %-4s\n",
           "学号","姓名","学院","课程号","课程名","学分","学期","日期","成绩");
    printf("----------------------------------------------------------------------------------------\n");
}

/* ============================================================
   功能：显示所有
   ============================================================ */
void show_all(void)
{
    if (get_size() == 0) { printf("暂无数据\n"); return; }
    print_header();
    traverse_records(print_record);
    printf("共 %d 条\n", get_size());
}

/* ============================================================
   功能：插入
   ============================================================ */
void do_insert(void)
{
    Record *r = (Record *)malloc(sizeof(Record));
    printf("学号(12位): "); scanf("%s", r->student_id);
    printf("姓名: "); scanf("%s", r->name);
    printf("学院: "); scanf("%s", r->college);
    printf("课程编号(8位): "); scanf("%s", r->course_id);
    printf("课程名: "); scanf("%s", r->course_name);
    printf("学分: "); scanf("%f", &r->credit);
    printf("学期(如2024-02): "); scanf("%s", r->semester);
    printf("日期(如2024-09-01): "); scanf("%s", r->date);
    printf("成绩(0-100): "); scanf("%d", &r->score);
    
    if (r->score < 0 || r->score > 100) {
        printf("成绩无效\n"); free(r); return;
    }
    if (insert_record(r)) printf("插入成功\n");
    else { printf("插入失败（已存在）\n"); free(r); }
}

/* ============================================================
   功能：查找
   ============================================================ */
void do_find(void)
{
    char id[13], cid[9];
    printf("学号: "); scanf("%s", id);
    printf("课程编号: "); scanf("%s", cid);
    Record *r = find_record(id, cid);
    if (r) { print_header(); print_record(r); }
    else printf("未找到\n");
}

/* ============================================================
   功能：按姓名查找（模糊匹配） 任务1 
   ============================================================ */
void do_find_by_name(void)
{
    char name[32];
    printf("请输入姓名（支持模糊匹配）: ");
    scanf("%s", name);

    int total = get_size();
    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) { printf("内存分配失败\n"); return; }

    int idx = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                if (strstr(p->data->name, name) != NULL) {
                    arr[idx++] = p->data;
                }
                p = p->next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            collect_avl_find_name(avl->root, arr, &idx, name);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    if (strstr(p->data->name, name) != NULL) {
                        arr[idx++] = p->data;
                    }
                    p = p->next;
                }
            }
            break;
        }
        default: break;
    }

    if (idx == 0) {
        printf("未找到\n");
        free(arr);
        return;
    }

    print_header();
    int i;
    for (i = 0; i < idx; i++) print_record(arr[i]);
    printf("共 %d 条\n", idx);
    free(arr);
}

/* ============================================================
   功能：按课程名称查找（模糊匹配） 任务1 
   ============================================================ */
void do_find_by_course(void)
{
    char cname[128];
    printf("请输入课程名称（支持模糊匹配）: ");
    scanf("%s", cname);

    int total = get_size();
    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) { printf("内存分配失败\n"); return; }

    int idx = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                if (strstr(p->data->course_name, cname) != NULL) {
                    arr[idx++] = p->data;
                }
                p = p->next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            collect_avl_find_course(avl->root, arr, &idx, cname);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    if (strstr(p->data->course_name, cname) != NULL) {
                        arr[idx++] = p->data;
                    }
                    p = p->next;
                }
            }
            break;
        }
        default: break;
    }

    if (idx == 0) {
        printf("未找到\n");
        free(arr);
        return;
    }

    print_header();
    int i;
    for (i = 0; i < idx; i++) print_record(arr[i]);
    printf("共 %d 条\n", idx);
    free(arr);
}

/* ============================================================
   功能：修改成绩
   ============================================================ */
void do_update(void)
{
    char id[13], cid[9];
    int score;
    printf("学号: "); scanf("%s", id);
    printf("课程编号: "); scanf("%s", cid);
    printf("新成绩(0-100): "); scanf("%d", &score);
    if (update_score(id, cid, score)) printf("修改成功\n");
    else printf("修改失败\n");
}

/* ============================================================
   功能：删除
   ============================================================ */
void do_delete(void)
{
    char id[13], cid[9];
    printf("学号: "); scanf("%s", id);
    printf("课程编号: "); scanf("%s", cid);
    if (delete_record(id, cid)) printf("删除成功\n");
    else printf("删除失败\n");
}

/* ============================================================
   功能：多条件筛选（课程名/学期/学院/成绩区间） 任务3：多条件筛选与多关键字排序
   ============================================================ */
void do_filter(void)
{
    char course_name[128] = {0};
    char semester[7] = {0};
    char college[64] = {0};
    int min = 0, max = 100;
    int has_filter = 0;
    int match_mode = 0;

    printf("\n========== 多条件筛选 ==========\n");
    printf("匹配方式: 0.模糊匹配  1.精确匹配\n");
    printf("请选择: ");
    scanf("%d", &match_mode);
    while (getchar() != '\n');

    printf("课程名称（直接回车跳过）: ");
    fgets(course_name, sizeof(course_name), stdin);
    course_name[strcspn(course_name, "\n")] = '\0';
    if (strlen(course_name) > 0) has_filter = 1;

    printf("选课学期（如2024-02，直接回车跳过）: ");
    fgets(semester, sizeof(semester), stdin);
    semester[strcspn(semester, "\n")] = '\0';
    if (strlen(semester) > 0) has_filter = 1;

    printf("学院（直接回车跳过）: ");
    fgets(college, sizeof(college), stdin);
    college[strcspn(college, "\n")] = '\0';
    if (strlen(college) > 0) has_filter = 1;

    printf("成绩区间(最小 最大，如 60 80，直接回车跳过): ");
    char score_input[100];
    fgets(score_input, sizeof(score_input), stdin);
    if (strlen(score_input) > 1) {
        sscanf(score_input, "%d %d", &min, &max);
        has_filter = 1;
    }

    if (!has_filter) {
        printf("未设置任何筛选条件\n");
        return;
    }

    int total = get_size();
    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) { printf("内存分配失败\n"); return; }

    int idx = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {       /* 代码中的匹配逻辑： 开始 */
                int match = 1;
                Record *r = p->data;
                if (strlen(course_name) > 0) {
                    if (match_mode == 0) {
                        if (strstr(r->course_name, course_name) == NULL) match = 0;
                    } else {
                        if (strcmp(r->course_name, course_name) != 0) match = 0;
                    }
                }
                if (strlen(semester) > 0 && strcmp(r->semester, semester) != 0) match = 0;
                if (strlen(college) > 0 && strcmp(r->college, college) != 0) match = 0;
                if (r->score < min || r->score > max) match = 0;
                if (match) arr[idx++] = r;  // 所有条件都满足才加入结果
                p = p->next;
            }   /* 代码中的匹配逻辑： 结束 */
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            collect_avl_filter_adv(avl->root, arr, &idx, course_name, semester, college, min, max, match_mode);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    int match = 1;
                    Record *r = p->data;
                    if (strlen(course_name) > 0) {
                        if (match_mode == 0) {
                            if (strstr(r->course_name, course_name) == NULL) match = 0;
                        } else {
                            if (strcmp(r->course_name, course_name) != 0) match = 0;
                        }
                    }
                    if (strlen(semester) > 0 && strcmp(r->semester, semester) != 0) match = 0;
                    if (strlen(college) > 0 && strcmp(r->college, college) != 0) match = 0;
                    if (r->score < min || r->score > max) match = 0;
                    if (match) arr[idx++] = r;
                    p = p->next;
                }
            }
            break;
        }
        default: break;
    }

    if (idx == 0) {
        printf("无符合条件记录\n");
        free(arr);
        return;
    }

    print_header();
    int i;
    for (i = 0; i < idx; i++) print_record(arr[i]);
    printf("共 %d 条\n", idx);

    printf("是否导出到文件？(y/n): ");
    char c;
    scanf(" %c", &c);
    if (c == 'y' || c == 'Y') {
        char filename[100];
        printf("请输入文件名（如 filter_result.csv）: ");
        scanf("%s", filename);
        FILE *fp = fopen(filename, "w");
        if (fp) {
            fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,学期,日期,成绩\n");
            for (i = 0; i < idx; i++) {
                fprintf(fp, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                        arr[i]->student_id, arr[i]->name, arr[i]->college,
                        arr[i]->course_id, arr[i]->course_name, arr[i]->credit,
                        arr[i]->semester, arr[i]->date, arr[i]->score);
            }
            fclose(fp);
            printf("导出 %d 条记录到 %s\n", idx, filename);
        } else {
            printf("无法创建文件\n");
        }
    }
    free(arr);
}

/* ============================================================
   功能：多关键字排序 任务3 
   ============================================================ */
void do_sort(void)
{
    int total = get_size();
    if (total == 0) { printf("暂无数据\n"); return; }
    
    printf("\n========== 多关键字排序 ==========\n");
    printf("支持字段: 1.成绩 2.学号 3.姓名 4.课程名称\n");
    printf("输入格式: 字段1:升降序,字段2:升降序 (如 1:desc,2:asc)\n");
    printf("示例: 先按成绩降序，再按学号升序 -> 1:desc,2:asc\n");
    printf("请输入排序规则: ");
    
    char input[200];
    getchar();
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';
    
    /* 解析排序规则 */
    int fields[10];
    int orders[10];  /* 0=升序, 1=降序 */
    int count = 0;
    
    char *token = strtok(input, ",");
    while (token && count < 10) {
        int field, order;
        char order_str[10];
        if (sscanf(token, "%d:%s", &field, order_str) == 2) {
            fields[count] = field;
            if (strcmp(order_str, "desc") == 0 || strcmp(order_str, "DESC") == 0) {
                orders[count] = 1;
            } else {
                orders[count] = 0;
            }
            count++;
        }
        token = strtok(NULL, ",");
    }
    
    if (count == 0) {
        printf("无效输入，使用默认排序：按成绩降序\n");
        fields[0] = 1;
        orders[0] = 1;
        count = 1;
    }
    
    /* 收集所有数据 */
    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) { printf("内存分配失败\n"); return; }
    
    int idx = 0;
    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) { arr[idx++] = p->data; p = p->next; }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            collect_avl_all(avl->root, arr, &idx);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) { arr[idx++] = p->data; p = p->next; }
            }
            break;
        }
        default: break;
    }
    
    /* 多关键字排序（冒泡排序） */
    int i, j, k;
    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - i - 1; j++) {
            int cmp = 0;
            for (k = 0; k < count && cmp == 0; k++) {
                int f = fields[k];
                int order = orders[k];
                int compare_result = 0;
                switch (f) {
                    case 1: /* 成绩 */
                        compare_result = (arr[j]->score - arr[j+1]->score);
                        break;
                    case 2: /* 学号 */
                        compare_result = strcmp(arr[j]->student_id, arr[j+1]->student_id);
                        break;
                    case 3: /* 姓名 */
                        compare_result = strcmp(arr[j]->name, arr[j+1]->name);
                        break;
                    case 4: /* 课程名称 */
                        compare_result = strcmp(arr[j]->course_name, arr[j+1]->course_name);
                        break;
                    default:
                        compare_result = 0;
                }
                if (order == 1) compare_result = -compare_result;  /* 降序取反 */
                cmp = compare_result;
            }
            if (cmp > 0) {
                Record *t = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = t;
            }
        }
    }
    
    print_header();
    for (i = 0; i < total; i++) print_record(arr[i]);
    free(arr);
}

/* ============================================================
   功能：统计   任务4：数据统计分析
   ============================================================ */
void do_stats(void)
{
    int total = get_size();
    if (total == 0) {
        printf("暂无数据\n");
        return;
    }

    int sum = 0, max = 0, min = 100;
    int a = 0, b = 0, c = 0, d = 0, e = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                Record *r = p->data;
                sum += r->score;
                if (r->score > max) max = r->score;
                if (r->score < min) min = r->score;
                if (r->score >= 90) a++;
                else if (r->score >= 80) b++;
                else if (r->score >= 70) c++;
                else if (r->score >= 60) d++;
                else e++;
                p = p->next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            stats_avl(avl->root, &sum, &max, &min, &a, &b, &c, &d, &e);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    Record *r = p->data;
                    sum += r->score;
                    if (r->score > max) max = r->score;
                    if (r->score < min) min = r->score;
                    if (r->score >= 90) a++;
                    else if (r->score >= 80) b++;
                    else if (r->score >= 70) c++;
                    else if (r->score >= 60) d++;
                    else e++;
                    p = p->next;
                }
            }
            break;
        }
        default:
            break;
    }

    printf("\n===== 统计 =====\n");
    printf("记录数: %d\n", total);
    printf("平均分: %.2f\n", (float)sum / total);
    printf("最高分: %d  最低分: %d\n", max, min);
    printf("优秀(90+): %d  良好(80-89): %d\n", a, b);
    printf("中等(70-79): %d  及格(60-69): %d  不及格: %d\n", c, d, e);
}

/* ============================================================
   功能：删除过期记录（早于2023-09-01）  任务5：批量删除过期记录 
   ============================================================ */
void do_expire(void)
{
    int count = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                if (strcmp(p->data->date, "2023-09-01") < 0) count++;
                p = p->next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            count_expire_avl(avl->root, &count);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    if (strcmp(p->data->date, "2023-09-01") < 0) count++;
                    p = p->next;
                }
            }
            break;
        }
        default:
            break;
    }

    if (count == 0) {
        printf("没有过期记录\n");
        return;
    }

    printf("删除 %d 条过期记录? (y/n): ", count);
    char c;
    scanf(" %c", &c);
    if (c != 'y' && c != 'Y') {
        printf("取消\n");
        return;
    }

    int deleted = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                DNode *next = p->next;
                if (strcmp(p->data->date, "2023-09-01") < 0) {
                    if (delete_record(p->data->student_id, p->data->course_id)) {
                        deleted++;
                    }
                }
                p = next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            delete_expire_avl(avl->root, &deleted);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    HNode *next = p->next;
                    if (strcmp(p->data->date, "2023-09-01") < 0) {
                        if (delete_record(p->data->student_id, p->data->course_id)) {
                            deleted++;
                        }
                    }
                    p = next;
                }
            }
            break;
        }
        default:
            break;
    }

    printf("删除 %d 条\n", deleted);
}

/* ============================================================
   功能：性能测试（任务6）
   ============================================================ */
void do_perf(void)
{
    printf("\n===== 性能测试 =====\n");
    printf("当前数据结构: %s\n", get_type_name());
    printf("记录数: %d\n", get_size());

    printf("\n请选择测试操作:\n");
    printf("1. 插入测试（插入100条）\n");
    printf("2. 查找测试（查找100次）\n");
    printf("3. 删除测试（删除100条）\n");
    printf("4. 遍历测试\n");
    printf("5. 全部测试\n");
    printf("请选择: ");

    int choice;
    scanf("%d", &choice);

    clock_t start, end;
    double cpu_time_used;
    int i;

    switch (choice) {
        case 1: {
            start = clock();
             /* 插入100条测试数据 */
            for (i = 0; i < 100; i++) {
                Record *r = (Record *)malloc(sizeof(Record));
                sprintf(r->student_id, "9999%06d", i);
                sprintf(r->name, "测试%d", i);
                sprintf(r->college, "测试学院");
                sprintf(r->course_id, "TE%06d", i);
                sprintf(r->course_name, "测试课程%d", i);
                r->credit = 3.0;
                sprintf(r->semester, "2024-02");
                sprintf(r->date, "2024-09-01");
                r->score = 80;
                insert_record(r);
            }
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("插入100条耗时: %.3f ms\n", cpu_time_used);
             /* 删除刚才插入的100条测试数据 */
		    for (i = 0; i < 100; i++) {
		        char id[13];
		        sprintf(id, "9999%06d", i);
		        delete_record(id, "TE000000");
		    }
            break;
        }
        case 2: {
            start = clock();
            char id[13], cid[9];
            for (i = 0; i < 100; i++) {
                sprintf(id, "2024%06d", i + 1);
                sprintf(cid, "CS300%d", (i % 10) + 1);
                find_record(id, cid);
            }
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("查找100次耗时: %.3f ms\n", cpu_time_used);
            break;
        }
        case 3: {
            start = clock();
            char id[13], cid[9];
            int deleted = 0;
            for (i = 0; i < 100 && i < get_size(); i++) {
                sprintf(id, "2024%06d", i + 1);
                sprintf(cid, "CS300%d", (i % 10) + 1);
                if (delete_record(id, cid)) deleted++;
            }
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("删除 %d 条耗时: %.3f ms\n", deleted, cpu_time_used);
            break;
        }
        case 4: {
            start = clock();
            traverse_records(print_record);
            end = clock();
            cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000;
            printf("遍历耗时: %.3f ms\n", cpu_time_used);
            break;
        }
        case 5: {
            clock_t s1, e1;
            s1 = clock();
            for (i = 0; i < 100; i++) {
                Record *r = (Record *)malloc(sizeof(Record));
                sprintf(r->student_id, "9999%06d", i);
                sprintf(r->name, "测试%d", i);
                sprintf(r->college, "测试学院");
                sprintf(r->course_id, "TE%06d", i);
                sprintf(r->course_name, "测试课程%d", i);
                r->credit = 3.0;
                sprintf(r->semester, "2024-02");
                sprintf(r->date, "2024-09-01");
                r->score = 80;
                insert_record(r);
            }
            e1 = clock();
            printf("插入100条耗时: %.3f ms\n", ((double)(e1 - s1)) / CLOCKS_PER_SEC * 1000);

            s1 = clock();
            for (i = 0; i < 100; i++) {
                char id[13], cid[9];
                sprintf(id, "2024%06d", i + 1);
                sprintf(cid, "CS300%d", (i % 10) + 1);
                find_record(id, cid);
            }
            e1 = clock();
            printf("查找100次耗时: %.3f ms\n", ((double)(e1 - s1)) / CLOCKS_PER_SEC * 1000);

            s1 = clock();
            traverse_records(print_record);
            e1 = clock();
            printf("遍历耗时: %.3f ms\n", ((double)(e1 - s1)) / CLOCKS_PER_SEC * 1000);
            break;
        }
        default:
            printf("无效选择\n");
    }
}

/* ============================================================
   保存数据到CSV 任务2 
   ============================================================ */
void save_data_to_csv(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("无法创建文件\n");
        return;
    }

    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,学期,日期,成绩\n");

    int total = get_size();
    if (total == 0) {
        fclose(fp);
        printf("没有数据可保存\n");
        return;
    }

    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) {
        fclose(fp);
        return;
    }

    int idx = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                arr[idx++] = p->data;
                p = p->next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            collect_avl_all(avl->root, arr, &idx);  /* ← 改成了 collect_avl_all */
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    arr[idx++] = p->data;
                    p = p->next;
                }
            }
            break;
        }
        default:
            break;
    }

    int i;
    for (i = 0; i < total; i++) {
        fprintf(fp, "%s,%s,%s,%s,%s,%.1f,%s,%s,%d\n",
                arr[i]->student_id,
                arr[i]->name,
                arr[i]->college,
                arr[i]->course_id,
                arr[i]->course_name,
                arr[i]->credit,
                arr[i]->semester,
                arr[i]->date,
                arr[i]->score);
    }

    free(arr);
    fclose(fp);
    printf("保存 %d 条记录到 %s\n", total, filename);
}

/* ============================================================
   主菜单
   ============================================================ */
void menu(void)
{
    printf("\n========================================\n");
    printf(" 校园选课记录检索系统\n");
    printf(" 数据结构: %s  记录数: %d\n", get_type_name(), get_size());
    printf("========================================\n");
    printf(" 1. 显示所有\n");
    printf(" 2. 插入记录\n");
    printf(" 3. 查找记录(按学号+课程编号)\n");
    printf(" 4. 按姓名查找\n");
    printf(" 5. 按课程名称查找\n");
    printf(" 6. 修改成绩\n");
    printf(" 7. 删除记录\n");
    printf(" 8. 多条件筛选\n");
    printf(" 9. 多关键字排序\n");
    printf("10. 统计分析\n");
    printf("11. 删除过期记录\n");
    printf("12. 保存数据\n");
    printf("13. 性能测试\n");
    printf(" 0. 退出\n");
    printf("请选择: ");
}

/* ============================================================
   退出时自动保存 任务2：数据持久化 
   ============================================================ */
void save_on_exit(void)
{
    printf("\n正在自动保存数据...\n");
    save_data_to_csv(DATA_FILE);   //保存数据 
}

int main()
{
    printf("\n校园选课记录检索与大数据分析系统\n");
    printf("作者: 周仙芝  学号: B20250905116\n");
    
    /* 注册退出时自动保存 */
    atexit(save_on_exit);
    
    init(USE_LIST);
    
    /* 如果数据文件不存在，自动生成100条测试数据 */
    // FILE *test = fopen(DATA_FILE, "r");
    //if (!test) {
    //    printf("数据文件不存在，正在自动生成100条测试数据...\n");
    //    generate_data(DATA_FILE, 100);
    //} else {
    //    fclose(test);
   // }
    
    load_csv(DATA_FILE);  //加载数据 
    
    int choice;
    do {
        menu();
        scanf("%d", &choice);
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: show_all(); break;
            case 2: do_insert(); break;
            case 3: do_find(); break;
            case 4: do_find_by_name(); break;
            case 5: do_find_by_course(); break;
            case 6: do_update(); break;
            case 7: do_delete(); break;
            case 8: do_filter(); break;
            case 9: do_sort(); break;
            case 10: do_stats(); break;
            case 11: do_expire(); break;
            case 12: save_data_to_csv(DATA_FILE); break;
            case 13: do_perf(); break;
            case 0: printf("再见\n"); break;
            default: printf("无效\n");
        }
    } while (choice != 0);
    
    return 0;
}
