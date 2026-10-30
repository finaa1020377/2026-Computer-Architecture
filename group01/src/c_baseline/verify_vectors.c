/*
 * verify_vectors.c — SAD 測試向量驗證器（CP3 / 成員 3）
 *
 * 讀入 test_vectors.csv，對每一列做兩件事：
 *   1. 用 spec_sad()（遮罩式 (d ^ m) - m，等同 CP2 的 sad.S 資料路徑）重算，
 *      確認 CSV 的 expected 欄位符合成員 1 的 wrap 語意規格。
 *   2. 呼叫 sad.c 的 sad_accumulate()（二補數取負式），確認 C 模型與 CSV 一致。
 * 兩條獨立實作都必須等於 expected，才算 PASS。
 *
 * 另外依資料自動判定邊界類型，統計「至少 3 組邊界值」這項要求：
 *   B1 = rs1 == rs2（零差值）
 *   B4 = 真正數學差 |rs1 - rs2| 超出 int32 範圍（減法溢位）
 *   B2 = d == 0x80000000（絕對值溢位）
 *   B3 = rd_old + |d| > 0xFFFFFFFF（累加溢位）
 *   SE = 任一運算元等於 INT32_MIN 或 INT32_MAX（signed 邊界）
 *
 * 用法（在 tests/ 目錄下）：
 *   gcc -std=c11 -Wall -Wextra -Werror verify_vectors.c ../c_baseline/sad.c -o verify_vectors
 *   ./verify_vectors test_vectors.csv
 *   ./verify_vectors test_vectors.csv --md      # 輸出 Markdown 表格，供報告貼用
 *   ./verify_vectors < test_vectors.csv         # 不給檔名時從 stdin 讀
 *
 * 結束碼：全部通過且邊界數達標為 0，向量驗證失敗為 1，參數或開檔錯誤為 2。
 */

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../c_baseline/sad.h"

#define LINE_BUF 512
#define MAX_FIELDS 6

/* 邊界旗標 */
enum {
    F_ZERO_DIFF   = 1u << 0,   /* B1 */
    F_SUB_OVERFLOW = 1u << 1,  /* B4 */
    F_ABS_OVERFLOW = 1u << 2,  /* B2 */
    F_ACC_WRAP     = 1u << 3,  /* B3 */
    F_SIGNED_EDGE  = 1u << 4   /* SE */
};

/* 規格模型：與 sad.S 相同的無分支資料路徑（遮罩式二補數絕對值）。 */
static uint32_t spec_sad(uint32_t rd_old, int32_t rs1, int32_t rs2,
                         uint32_t *d_out, uint32_t *ad_out)
{
    uint32_t d = (uint32_t)rs1 - (uint32_t)rs2;          /* mod 2^32 */
    uint32_t mask = (d & UINT32_C(0x80000000)) ? UINT32_MAX : UINT32_C(0);
    uint32_t ad = (d ^ mask) - mask;                     /* |d|，0x80000000 不變 */

    if (d_out != NULL) {
        *d_out = d;
    }
    if (ad_out != NULL) {
        *ad_out = ad;
    }
    return rd_old + ad;                                  /* mod 2^32 */
}

/* 依輸入資料自動判定邊界類型。 */
static unsigned classify(uint32_t rd_old, int32_t rs1, int32_t rs2,
                         uint32_t d, uint32_t ad)
{
    unsigned flags = 0;
    int64_t exact = (int64_t)rs1 - (int64_t)rs2;

    if (rs1 == rs2) {
        flags |= F_ZERO_DIFF;
    }
    if (exact < (int64_t)INT32_MIN || exact > (int64_t)INT32_MAX) {
        flags |= F_SUB_OVERFLOW;
    }
    if (d == UINT32_C(0x80000000)) {
        flags |= F_ABS_OVERFLOW;      /* 0 − 0x80000000 仍為 0x80000000 */
    }
    if ((uint64_t)rd_old + (uint64_t)ad > (uint64_t)UINT32_MAX) {
        flags |= F_ACC_WRAP;
    }
    if (rs1 == INT32_MAX || rs1 == INT32_MIN ||
        rs2 == INT32_MAX || rs2 == INT32_MIN) {
        flags |= F_SIGNED_EDGE;
    }
    return flags;
}

static void flag_text(unsigned flags, char *out, size_t size)
{
    static const char *const names[] = { "B1", "B4", "B2", "B3", "SE" };
    size_t used = 0;
    int first = 1;
    size_t i;

    if (size == 0) {
        return;
    }
    out[0] = '\0';
    for (i = 0; i < sizeof names / sizeof names[0]; ++i) {
        int written;

        if ((flags & (1u << i)) == 0) {
            continue;
        }
        written = snprintf(out + used, size - used, "%s%s",
                           first ? "" : ",", names[i]);
        if (written < 0 || (size_t)written >= size - used) {
            return;
        }
        used += (size_t)written;
        first = 0;
    }
    if (first) {
        (void)snprintf(out, size, "-");
    }
}

static void strip_eol(char *s)
{
    size_t len = strlen(s);

    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[--len] = '\0';
    }
}

static size_t split_fields(char *line, char *fields[], size_t max_fields)
{
    size_t count = 0;
    char *cursor = line;

    while (count < max_fields) {
        fields[count++] = cursor;
        cursor = strchr(cursor, ',');
        if (cursor == NULL) {
            break;
        }
        *cursor++ = '\0';
    }
    return count;
}

static int parse_u32(const char *text, uint32_t *out)
{
    char *end = NULL;
    unsigned long long value;

    if (*text == '\0') {
        return 0;
    }
    value = strtoull(text, &end, 0);
    if (end == text || *end != '\0' || value > UINT32_MAX) {
        return 0;
    }
    *out = (uint32_t)value;
    return 1;
}

static int parse_i32(const char *text, int32_t *out)
{
    char *end = NULL;
    long long value;

    if (*text == '\0') {
        return 0;
    }
    value = strtoll(text, &end, 0);
    if (end == text || *end != '\0' ||
        value < INT32_MIN || value > INT32_MAX) {
        return 0;
    }
    *out = (int32_t)value;
    return 1;
}

int main(int argc, char **argv)
{
    const char *path = NULL;
    int markdown = 0;
    int arg;
    FILE *input = stdin;
    char line[LINE_BUF];
    char *fields[MAX_FIELDS];
    char flags_text[16];
    size_t total = 0;
    size_t passed = 0;
    size_t boundary = 0;
    size_t malformed = 0;
    int line_no = 0;

    for (arg = 1; arg < argc; ++arg) {
        if (strcmp(argv[arg], "--md") == 0) {
            markdown = 1;
        } else if (path == NULL) {
            path = argv[arg];
        } else {
            fprintf(stderr, "未知參數：%s\n", argv[arg]);
            return 2;
        }
    }

    if (path != NULL) {
        input = fopen(path, "r");
        if (input == NULL) {
            fprintf(stderr, "無法開啟 %s\n", path);
            return 2;
        }
    }

    if (markdown) {
        printf("| # | `rd_old` | `rs1` | `rs2` | `d` | `ad` | `rd_new` | 類型 |\n");
        printf("|---:|---:|---:|---:|---|---:|---:|---|\n");
    } else {
        printf("%-3s %-11s %-12s %-12s %-11s %-11s %-8s %-9s %s\n",
               "#", "rd_old", "rs1", "rs2", "d", "expected", "actual",
               "boundary", "result");
    }

    while (fgets(line, sizeof line, input) != NULL) {
        uint32_t rd_old;
        uint32_t expected;
        uint32_t actual;
        uint32_t model;
        uint32_t d;
        uint32_t ad;
        int32_t rs1;
        int32_t rs2;
        unsigned flags;
        size_t count;
        int ok;

        strip_eol(line);
        ++line_no;
        if (line[0] == '\0' || line[0] == '#') {
            continue;
        }
        count = split_fields(line, fields, MAX_FIELDS);
        if (count < 5) {
            fprintf(stderr, "第 %d 列：欄位不足（需要 5 欄以上）\n", line_no);
            ++malformed;
            continue;
        }
        if (strcmp(fields[0], "case") == 0) {
            continue;                                  /* 表頭 */
        }
        if (!parse_u32(fields[1], &rd_old) || !parse_i32(fields[2], &rs1) ||
            !parse_i32(fields[3], &rs2) || !parse_u32(fields[4], &expected)) {
            fprintf(stderr, "第 %d 列：數值格式錯誤\n", line_no);
            ++malformed;
            continue;
        }

        model = spec_sad(rd_old, rs1, rs2, &d, &ad);
        actual = sad_accumulate(rd_old, rs1, rs2);
        flags = classify(rd_old, rs1, rs2, d, ad);
        ok = (model == expected) && (actual == expected);
        flag_text(flags, flags_text, sizeof flags_text);

        ++total;
        if (ok) {
            ++passed;
        }
        if (flags != 0) {
            ++boundary;
        }

        if (markdown) {
            printf("| %zu | %" PRIu32 " | %" PRId32 " | %" PRId32
                   " | `0x%08" PRIX32 "` | `0x%08" PRIX32
                   "` | %" PRIu32 " | %s |\n",
                   total, rd_old, rs1, rs2, d, ad, expected, flags_text);
        } else {
            printf("%-3zu %-11" PRIu32 " %-12" PRId32 " %-12" PRId32
                   " 0x%08" PRIX32 "  0x%08" PRIX32 "  0x%08" PRIX32
                   " %-9s %s\n",
                   total, rd_old, rs1, rs2, d, expected, actual,
                   flags_text, ok ? "PASS" : "FAIL");
        }
    }

    if (input != stdin) {
        (void)fclose(input);
    }

    if (markdown) {
        fprintf(stderr, "%zu 組向量，%zu 組通過，%zu 組失敗，邊界 %zu 組\n",
                total, passed, total - passed, boundary);
        return (malformed == 0 && passed == total && boundary >= 3) ? 0 : 1;
    }

    printf("\n%zu 組向量，%zu 組通過，%zu 組失敗\n",
           total, passed, total - passed);
    printf("邊界向量：%zu 組（要求至少 3 組）%s\n",
           boundary, boundary >= 3 ? "OK" : "FAIL");
    printf("向量總數：%zu 組（要求至少 10 組）%s\n",
           total, total >= 10 ? "OK" : "FAIL");

    if (malformed != 0) {
        printf("格式錯誤：%zu 列\n", malformed);
    }

    return (malformed == 0 && passed == total && boundary >= 3 &&
            total >= 10) ? 0 : 1;
}
