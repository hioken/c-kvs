#include "./main.c"
#include "./kvs.c"
#include <assert.h>

/* 一旦凍結
#include <stdbool.h>
typedef struct {
    int failure_cnt;
    int skipped_test_cnt;
    bool skipping;
} TestContext;

    if (t_ctx_p->skipping) {
        (t_ctx_p->skipped_test_cnt)++;
        return 0;
    }
*/


int main_loop(Context*, char[]);
int run_all_tests(void);

// ここからテスト
int test_core_sequence(Context* ctx_p) {
    char test_set_1[] = "SET key1 apple";
    strncpy(ctx_p->buf, test_set_1, sizeof(test_set_1));

    parse_input(ctx_p);
    assert(strcmp(ctx_p->args[0], "SET") == 0);
    assert(strcmp(ctx_p->args[1], "key1") == 0);
    assert(strcmp(ctx_p->args[2], "apple") == 0);
};

int test_string_set_and_get(Context* ctx_p) {
    // test_core_sequenceの続き SET key1 apple から
    dispatch_cmd(ctx_p);
    assert(strcmp(ctx_p->result, "OK") == 0);

    char test_get_1[] = "GET key1";
    main_loop(ctx_p, test_get_1);
    assert(strcmp(ctx_p->result, "apple") == 0);


    char test_set_2[] = "SET key2 banana";
    main_loop(ctx_p, test_set_2);
    assert(strcmp(ctx_p->result, "OK") == 0);

    char test_get_2[] = "GET key2";
    main_loop(ctx_p, test_get_2);
    assert(strcmp(ctx_p->result, "banana") == 0);


    char test_set_3[] = "SET key2 orange";
    main_loop(ctx_p, test_set_3);
    main_loop(ctx_p, test_get_2);
    assert(strcmp(ctx_p->result, "orange") == 0);

    char test_get_3[] = "GET key_not_found";
    assert(strcmp(ctx_p->result, "nil") == 0);

    return 0;
}

// テストを走らせる処理
int main(void) {
    run_all_tests();

    printf("--All Success--\n");
    return 0;
}

int run_all_tests(void) {
    Context ctx;
    test_core_sequence(&ctx);
    test_string_set_and_get(&ctx);

    return 0;
}

// テスト用関数
int main_loop(Context* ctx_p, char test_cmd[]) {
    strcpy(ctx_p->buf, test_cmd);
    parse_input(ctx_p);
    dispatch_cmd(ctx_p);

    return 0;
}