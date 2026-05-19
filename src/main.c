#include <stdio.h>
#include <string.h>

int input_line(void);
int parse_input(char*);

int main(void) {
    // kvsのsetup呼び出し
    // ループ
      // if (input_line()) { エラー }
    return 0;
}

int input_line(void) {
    // 入力待ち
    // if sizeチェック エラー
    // if (parse_input(入力)) return 1;
}

int parse_input(char* line) {
    static const struct {
        const char *command;
        int (*kvs_func)(char**);
    } command_table[] = {
      {"GET", kvs_string_get},
      {"SET", kvs_string_set}
    };
    static const int table_size = sizeof(command_table) / sizeof(command_table[0]);
    // char** argsを作る

    for (int i = 0; i < table_size; i++) {
        if (コマンド command_table[0]) {
            command_table[1].kvs_func(args);
            return 0;
        }
    }
    // エラー処理
    return 1;
}