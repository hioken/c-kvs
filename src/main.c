#include <stdio.h>
#include <string.h>
#include "kvs.h"

static int input_line(Context*);
static int parse_input(Context*);
static int dispatch_cmd(Context*);
static void reset_context(Context*);
static void to_uppercase_bit(char []);

#ifdef TESTMODE
#include "../test/test_main.c"
#endif

int main(void) {
    Context ctx;
    node_setup();

#ifndef TESTMODE
    while (1) { 
        reset_context(&ctx);
        if (input_line(&ctx) == 2) {
            printf("Good Night\n");
            return 0;
        }
        parse_input(&ctx);
        dispatch_cmd(&ctx);

        // feat: resultを扱う処理
        if (ctx.status) {
            printf("Success\nResult: '%s'\n", ctx.result);
        } else {
            printf("Error: %s.\n", ctx.result);
        }
    }
    
#else
    run_all_tests(&ctx);
#endif

    return 0;
}

static int input_line(Context* ctx_p) {
    printf("\nPlease enter a command: ");
    fgets(ctx_p->buf, sizeof(ctx_p->buf), stdin);
    if (ctx_p->buf[DEFAULT_BUF_SIZE - 2] != '\x07') {
        // feat: バッファオーバーフローの例外処理
        printf("Error: Command length limit exceeded (max: %d bytes).\n", DEFAULT_BUF_SIZE - 2);
        return 1;
    }
    size_t len = strlen(ctx_p->buf);
    if (len > 0 && ctx_p->buf[len - 1] == '\n') { ctx_p->buf[len - 1] = '\0';}

    if (!(ctx_p->buf[0])) {
        // feat: コマンドが空の時の例外処理
        printf("Error: Command is empty. (at input_line)\n");
        return 1;
    } else if (strcmp(ctx_p->buf, "exit") == 0) {
        return 2;
    }
    return 0;
}

static int parse_input(Context* ctx_p) {
    int i = 0;
    char* token = strtok(ctx_p->buf, " ");
    ctx_p->args[i++] = token;
    if (!token) {
        // feat: コマンドが空の時の例外処理
        printf("Error: Command is empty. (at parse_input)\n");
        return 1;
    }

    while (token != NULL && i < MAX_ARGS) { // 2個めの条件は保険
        token = strtok(NULL, " ");
        ctx_p->args[i++] = token;
    }

    if (i > MAX_ARGS) {
        // feat: 動的リサイズなり、エラー処理なり
        printf("Error: Argument count exceeds limit.\n");
        return 1;
    }

    return 0;
}

static int dispatch_cmd(Context* ctx_p) {
    static const struct {
        const char *cmd;
        int (*kvs_func)(Context*);
    } cmd_table[] = {
      {"GET", kvs_string_get},
      {"SET", kvs_string_set}
    };
    int table_size = sizeof(cmd_table) / sizeof(cmd_table[0]);
    to_uppercase_bit(ctx_p->args[0]);

    for (int i = 0; i < table_size; i++) {
        if (strcmp(ctx_p->args[0], cmd_table[i].cmd) == 0) {
            if ( cmd_table[i].kvs_func(ctx_p) ) return 1;
            return 0;
        }
    }
    printf("Error: Command not found.\n");
    return 1;
}

static void reset_context(Context* ctx_p) {
    memset(ctx_p, 0, sizeof(Context));
    ctx_p->buf[DEFAULT_BUF_SIZE - 2] = '\x07';
}

static void to_uppercase_bit(char str[]) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') *str &= ~0x20; 
        str++;
    }
}