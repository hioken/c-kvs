#ifndef KVS_H
#define KVS_H

#define DEFAULT_BUF_SIZE 1024
#define MAX_ARGS 32

typedef struct {
    char buf[DEFAULT_BUF_SIZE];
    char* args[MAX_ARGS];
    char result[768]; // 0の時はエラー, 1の時は取得結果 2の時は取得できなかった時の結果
    int status;
} Context;

int node_setup(void);
int kvs_string_get(Context*);
int kvs_string_set(Context*);

#endif