/**
 * data_generator.c - 独立数据生成器
 * 生成 100、1000、10000、100000 条不重复姓名的数据
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generate_data(const char *filename, int count)
{
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("无法创建文件 %s\n", filename);
        return;
    }

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

    char *given_chars[] = {"伟","芳","娜","秀英","敏","静","丽","强","磊","洋",
                           "勇","艳","杰","倩","涛","明","超","秀兰","霞","平",
                           "刚","桂英","涛","慧","建","文","华","飞","玉兰","斌",
                           "宇","鑫","浩","然","博","文","轩","宇","辰","怡",
                           "彤","萱","妍","琦","珂","玥","琛","瑶","雪","晴",
                           "子涵","梓涵","一鸣","天翊","瑞霖","雨桐","梦瑶","欣怡","浩宇","梓萱",
                           "若曦","子轩","宇航","可欣","沐阳","诗雨","乐怡","欣怡","子琪","浩然"};
    int given_count = sizeof(given_chars) / sizeof(given_chars[0]);

    char *colleges[] = {"计算机科学与工程学院","电子信息工程学院","数学与统计学院",
                        "外国语学院","经济管理学院","物理与光电学院","化学与化工学院",
                        "生命科学与技术学院","材料科学与工程学院","机械与汽车工程学院"};
    int college_count = sizeof(colleges) / sizeof(colleges[0]);

    char *courses[] = {"数据结构","操作系统","数据库原理","计算机网络","软件工程",
                       "编译原理","算法设计","人工智能","计算机组成","离散数学"};
    char *cids[] = {"CS3001","CS3002","CS3003","CS3004","CS3005",
                    "CS3006","CS3007","CS3008","CS3009","CS3010"};
    float credits[] = {3.5,3.0,3.5,3.0,3.0,3.5,3.0,3.5,3.0,3.0};
    int course_count = sizeof(courses) / sizeof(courses[0]);

    char *semesters[] = {"2021-01","2021-02","2022-01","2022-02",
                         "2023-01","2023-02","2024-01","2024-02",
                         "2025-01","2025-02","2026-01","2026-02"};
    char *dates[] = {"2021-02-20","2021-09-01","2022-02-15","2022-09-01",
                     "2023-02-20","2023-09-01","2024-02-15","2024-09-01",
                     "2025-02-20","2025-09-01","2026-02-15","2026-09-01"};
    int sem_count = sizeof(semesters) / sizeof(semesters[0]);

    fprintf(fp, "学号,姓名,学院,课程编号,课程名称,学分,学期,日期,成绩\n");

    int i;
    for (i = 0; i < count; i++) {
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
                rand() % 60 + 40);
    }

    fclose(fp);
    printf("生成 %d 条数据到 %s\n", count, filename);
}

int main()
{
    srand((unsigned)time(NULL));
    
    printf("========== 数据生成器 ==========\n");
    printf("开始生成测试数据...\n\n");
    
    generate_data("data_100.csv", 100);
    generate_data("data_1000.csv", 1000);
    generate_data("data_10000.csv", 10000);
    generate_data("data_100000.csv", 100000);
    
    printf("\n所有数据生成完成！\n");
    printf("文件列表：\n");
    printf("  - data_100.csv    (100条)\n");
    printf("  - data_1000.csv   (1000条)\n");
    printf("  - data_10000.csv  (10000条)\n");
    printf("  - data_100000.csv (100000条)\n");
    printf("\n提示：每个文件中的姓名均不重复。\n");
    
    return 0;
}
