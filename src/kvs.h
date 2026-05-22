#ifndef KVS_H
#define KVS_H

#define DEFAULT_BUF_SIZE 1024
#define MAX_ARGS 32

int node_setup(void);
int kvs_string_get(Context*);
int kvs_string_set(Context*);

#endif