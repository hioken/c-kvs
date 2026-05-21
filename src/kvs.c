#include <stdio.h>
#include <stdint.h>
#include "./kvs.h"

#define MAX_NODES 1000
#define TABLE_SIZE 1009
#define VALUE_SIZE 768
#define KEY_SIZE 256
#ifdef TESTMODE
    #define STATIC
#else
    #define STATIC static
#endif

typedef enum {
    TYPE_STRING = 1,
    TYPE_LIST,
    TYPE_HASH
} DataType;

typedef struct Node Node;
struct Node {
    char key[KEY_SIZE];
    DataType type;
    Node* next;
    union {
        char string_val[VALUE_SIZE];
        Node* sub_node;
    } value;
};

STATIC Node pool[MAX_NODES];
STATIC Node* hash_table[TABLE_SIZE] = { NULL };
STATIC Node* free_head;

STATIC uint32_t hash_fnv1a(const char*);
STATIC Node* scan_chain(const char*);

int setup(void) {
    for(int i = 0; i < MAX_NODES - 1; i++) {
        pool[i] = (Node){0};
        pool[i].next = &pool[i+1];
    }
    pool[MAX_NODES - 1] = (Node){.next = NULL};
    free_head = &pool[0];

    return 0;
}

STATIC uint32_t hash_fnv1a(const char* key) {
    uint32_t hash = 2166136261U; // FNVの32ビット用初期値
    const uint32_t fnv_prime = 16777619U; // FNVの32ビット用素数

    while (*key) {
        hash ^= (unsigned char)*key++; // 文字をXORで混ぜる
        hash *= fnv_prime;             // 素数を掛け算する
    }
    return hash;
}

STATIC Node* scan_chain(const char* key) {
    while () {

    }
    if (1) {
        return node_p;
    } else {
        return NULL;
    }
};

int kvs_string_get(Context*) {
    if (1) {
        printf("value");
        return 0;
    } else {
        // 例外処理
        return 1;
    }
}

int kvs_string_set(Context*) {
    if (1) {
        printf("success");
        return 0;
    } else {
        // 例外処理
        return 1;
    }
}