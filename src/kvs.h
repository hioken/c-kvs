#ifndef KVS_H   // もし MY_HEADER_H が未定義なら（1回目の読み込みなら）
#define KVS_H   // MY_HEADER_H を定義する

int setup(void);
int kvs_string_get(Context*);
int kvs_string_set(Context*);

#endif