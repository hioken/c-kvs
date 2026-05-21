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
static Node* hash_table[TABLE_SIZE] = { NULL };
STATIC Node* free_head;

static uint32_t hash_fnv1a(const char*);
static Node* scan_chain(const int, const char*);

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

STATIC Node* scan_chain(const int idx, const char* input_key) {
    Node* node_p = hash_table[idx];
    while (node_p != NULL && node_p->key[0] != '\0') {
        if (strcmp(node_p->key, input_key) == 0) { return hash_table[idx]; }
        node_p = node_p->next;
    }
    return NULL;
};

int kvs_string_get(Context* ctx_p) {
    int idx = hash_fnv1a(ctx_p->args[1]) / TABLE_SIZE;
    Node* matched_node_p = scan_chain(idx, ctx_p->args[1])
    if (matched_node_p*) {
        ctx_p->result = matched_node_p->value;
        ctx_p->status = 400;
        return 0;
    } else {
        snprintf(ctx_p->result, sizeof(ctx_p->result), "Undefined key: '%s'", args[1]);
        return 1;
    }
}

int kvs_string_set(Context* ctx_p) {
    int idx = hash_fnv1a(ctx_p->args[1]) / TABLE_SIZE;
    Node* matched_node_p = scan_chain(idx, ctx_p->args[1])
    if (matched_node_p) {
        strncpy(matched_node_p->value, ctx_p->args[2], sizeof(matched_node_p->value));
        return 0;
    } else {
        Node* new_node_p = free_head;
        free_head = new_node_p->next;
        new_node_p->next = hash_table[idx];
        new_node_p->value = ctx_p->args[2];
        return 0;
    }
}