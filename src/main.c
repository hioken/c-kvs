#include <stdio.h>
#include <string.h>
#include <kvs.h>

#define DEFAULT_BUF_SIZE 1024
#define MAX_ARGS 32

typedef struct {
    char buf[DEFAULT_BUF_SIZE];
    char* args[MAX_ARGS];
    char result[768]; // 200の時は結果, 400の時はエラー文
    int status;
} Context;

static int input_line(Context*);
int parse_input(Context*);
int dispatch_cmd(Context*);
void to_uppercase_bit(char [])

#ifndef TESTMODE
int main(void) {
    Context ctx = {0};
    ctx.buf[DEFAULT_BUF_SIZE - 2] = '\x07';
    ctx.status = 200;
    setup();

    while (true) { 
        if (input_line(&ctx) == 2) {
            printf("Good Night\n");
            return 0;
        }
        parse_input(&ctx);
        dispatch_cmd(&ctx);
        // feat: resultを扱う処理
    }

    return 0;
}
#endif

int input_line(Context* ctx_p) {
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

int parse_input(Context* ctx_p) {
    int i = 0;
    char* token = strtok(ctx_p->buf, " ");
    ctx_p->args[i++] = token;
    if (!token) {
        // feat: コマンドが空の時の例外処理
        printf("Error: Command is empty. (at parse_input)\n");
        return 1;
    }

    // 2個めの条件は保険
    while (token != NULL && i < MAX_ARGS + 1) {
        token = strtok(NULL, " ");
        args[i++] = token;
    }

    if (i > MAX_ARGS) {
        // feat: 動的リサイズなり、エラー処理なり
        printf("Error: Argument count exceeds limit.\n");
        return 1;
    }

    return 0;
}

int dispatch_cmd(Context* ctx_p) {
    static const struct {
        const char *cmd;
        int (*kvs_func)(Context*);
    } cmd_table[] = {
      {"GET", kvs_string_get},
      {"SET", kvs_string_set}
    };
    int table_size = sizeof(cmd_table) / sizeof(cmd_table[0]);
    to_uppercase_bit(args[0]);

    for (int i = 0; i < table_size; i++) {
        if (strcmp(ctx_p->args[0], cmd_table[i])) {
            if ( cmd_table[1].kvs_func(ctx_p) ) return 1;
            return 0;
        }
    }
    printf("Error: Command not found.\n");
    return 1;
}

void to_uppercase_bit(char str[]) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') *str &= ~0x20; 
        str++;
    }
}