#include <assert.h>

void run_all_tests(Context*);
void main_flow_for_test(Context*, char []);
void test_core_sequence(Context*);
void test_string_set_and_get(Context*);

// テストを走らせる処理
void run_all_tests(Context* ctx_p) {
    test_core_sequence(ctx_p);
    test_string_set_and_get(ctx_p);

    printf("--All Success--\n");
}

// テスト用関数
void main_flow_for_test(Context* ctx_p, char test_cmd[]) {
    reset_context(ctx_p);
    strcpy(ctx_p->buf, test_cmd);
    parse_input(ctx_p);
    dispatch_cmd(ctx_p);
}

// ここからテスト
void test_core_sequence(Context* ctx_p) {
    char test_set_1[] = "SET key1 apple";
    strncpy(ctx_p->buf, test_set_1, sizeof(ctx_p->buf));

    parse_input(ctx_p);
    assert(strcmp(ctx_p->args[0], "SET") == 0);
    assert(strcmp(ctx_p->args[1], "key1") == 0);
    assert(strcmp(ctx_p->args[2], "apple") == 0);
}

void test_string_set_and_get(Context* ctx_p) {
    // test_core_sequenceの続き SET key1 apple から
    dispatch_cmd(ctx_p);
    assert(strcmp(ctx_p->result, "OK") == 0);

    char test_get_1[] = "GET key1";
    main_flow_for_test(ctx_p, test_get_1);
    assert(strcmp(ctx_p->result, "apple") == 0);


    char test_set_2[] = "SET key2 banana";
    main_flow_for_test(ctx_p, test_set_2);
    assert(strcmp(ctx_p->result, "OK") == 0);

    char test_get_2[] = "GET key2";
    main_flow_for_test(ctx_p, test_get_2);
    assert(strcmp(ctx_p->result, "banana") == 0);


    char test_set_3[] = "SET key2 orange";
    main_flow_for_test(ctx_p, test_set_3);
    main_flow_for_test(ctx_p, test_get_2);
    assert(strcmp(ctx_p->result, "orange") == 0);

    char test_get_3[] = "GET key_not_found";
    main_flow_for_test(ctx_p, test_get_3);
    assert(strcmp(ctx_p->result, "Undefined key: key_not_found") == 0);
}