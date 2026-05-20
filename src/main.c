#include <stdio.h>
#include <string.h>
#include <kvs.h>

#define DEFAULT_BUF_SIZE 1024
#define 

typedef struct {
    char buf[DEFAULT_BUF_SIZE];
    char* args[32];
    char result[768];
} Context;

int input_line(Context*);
int parse_input(Context*);
int dispatch_cmd(Context*)

#ifndef TESTMODE
int main(void) {
    Context ctx;
    while (true) { 
        input_line(&ctx);
        parse_input(&ctx);
        dispatch_cmd(&ctx);
    }
    return 0;
}
#endif

int input_line(Context* ctx_p) {
    // 入力待ち
    // if sizeチェック エラー
        // buf_margin[4096]
    // if (parse_input(入力)) return 1;
}

int parse_input(Context* ctx_p) {
    // char input_cmd[], char** argsを作る

    // エラー処理
    return 0;
}

int dispatch_cmd(Context* ctx_p) {
    static const struct {
        const char *cmd;
        int (*kvs_func)(char**);
    } cmd_table[] = {
      {"GET", kvs_string_get},
      {"SET", kvs_string_set}
    };
    int table_size = sizeof(cmd_table) / sizeof(cmd_table[0]);

    for (int i = 0; i < table_size; i++) {
        if (コマンド cmd_table[0]) {
            if ( cmd_table[1].kvs_func(ctx_p) ) return 1;
            // resultを使う処理
            return 0;
        }
    }

}