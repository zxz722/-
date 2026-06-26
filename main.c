/**
 * main.c - 主程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
   功能：筛选（按成绩区间）
   ============================================================ */
void do_filter(void)
{
    int min, max;
    printf("成绩区间(最小 最大): ");
    scanf("%d %d", &min, &max);

    int total = get_size();
    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) {
        printf("内存分配失败\n");
        return;
    }

    int idx = 0;

    switch (get_current_type()) {
        case USE_LIST: {
            LinkedList *L = get_list_instance();
            DNode *p = L->head->next;
            while (p) {
                if (p->data->score >= min && p->data->score <= max) {
                    arr[idx++] = p->data;
                }
                p = p->next;
            }
            break;
        }
        case USE_AVL: {
            AVLTree *avl = get_avl_instance();
            collect_avl_filter(avl->root, arr, &idx, min, max);
            break;
        }
        case USE_HASH: {
            HashTable *H = get_hash_instance();
            int i;
            for (i = 0; i < H->capacity; i++) {
                HNode *p = H->buckets[i];
                while (p) {
                    if (p->data->score >= min && p->data->score <= max) {
                        arr[idx++] = p->data;
                    }
                    p = p->next;
                }
            }
            break;
        }
        default:
            break;
    }

    if (idx == 0) {
        printf("无符合条件记录\n");
        free(arr);
        return;
    }

    print_header();
    int i;
    for (i = 0; i < idx; i++) {
        print_record(arr[i]);
    }
    printf("共 %d 条\n", idx);
    free(arr);
}

/* ============================================================
   功能：排序（按成绩降序）
   ============================================================ */
void do_sort(void)
{
    int total = get_size();
    if (total == 0) {
        printf("暂无数据\n");
        return;
    }

    Record **arr = (Record **)malloc(total * sizeof(Record *));
    if (!arr) {
        printf("内存分配失败\n");
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
            collect_avl_all(avl->root, arr, &idx);
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

    int i, j;
    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - i - 1; j++) {
            if (arr[j]->score < arr[j + 1]->score) {
                Record *t = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = t;
            }
        }
    }

    print_header();
    for (i = 0; i < total; i++) {
        print_record(arr[i]);
    }
    free(arr);
}

/* ============================================================
   功能：统计
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
   功能：删除过期记录（早于2023-09-01）
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
   功能：性能测试
   ============================================================ */
void do_perf(void)
{
    printf("\n===== 性能测试 =====\n");
    printf("数据结构: %s\n", get_type_name());
    printf("记录数: %d\n", get_size());
    printf("提示: 修改 init() 中的类型来对比不同结构\n");
}

/* ============================================================
   保存数据到CSV
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
    printf(" 3. 查找记录\n");
    printf(" 4. 修改成绩\n");
    printf(" 5. 删除记录\n");
    printf(" 6. 筛选(按成绩区间)\n");
    printf(" 7. 排序(按成绩降序)\n");
    printf(" 8. 统计分析\n");
    printf(" 9. 删除过期记录\n");
    printf("10. 保存数据\n");
    printf("11. 性能测试\n");
    printf(" 0. 退出\n");
    printf("请选择: ");
}

int main()
{
    printf("\n校园选课记录检索与大数据分析系统\n");
    printf("作者: 周仙芝  学号: B20250905116\n");
    
    init(USE_HASH);
    
     /* 如果数据文件不存在，自动生成100条测试数据 */
    FILE *test = fopen(DATA_FILE, "r");
    if (!test) {
    	 printf("数据文件不存在，正在自动生成100条测试数据...\n");
        generate_data(DATA_FILE, 100);
    } else {
        fclose(test);
    }
    
    load_csv(DATA_FILE);
    
    int choice;
    do {
        menu();
        scanf("%d", &choice);
        while (getchar() != '\n');
        
        switch (choice) {
            case 1: show_all(); break;
            case 2: do_insert(); break;
            case 3: do_find(); break;
            case 4: do_update(); break;
            case 5: do_delete(); break;
            case 6: do_filter(); break;
            case 7: do_sort(); break;
            case 8: do_stats(); break;
            case 9: do_expire(); break;
            case 10: save_data_to_csv(DATA_FILE); break;  /* ← 改了 */
            case 11: do_perf(); break;
            case 0: save_data_to_csv(DATA_FILE); printf("再见\n"); break;  /* ← 改了 */
            default: printf("无效\n");
        }
    } while (choice != 0);
    
    return 0;
}
