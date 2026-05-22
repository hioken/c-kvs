#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "./kvs.h"

#define MAX_NODES 1000
#define TABLE_SIZE 1009
#define VALUE_SIZE 768
#define KEY_SIZE 256

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

static Node pool[MAX_NODES];
static Node* hash_table[TABLE_SIZE] = { NULL };
static Node* free_head;

static uint32_t hash_fnv1a(const char*);
static Node* scan_chain(const int, const char*);

int node_setup(void) {
    for(int i = 0; i < MAX_NODES - 1; i++) {
        pool[i] = (Node){0};
        pool[i].next = &pool[i+1];
    }
    pool[MAX_NODES - 1] = (Node){.next = NULL};
    free_head = &pool[0];

    assert(pool[0].next == &pool[1]);
    assert(pool[1].next == &pool[2]);
    assert(pool[MAX_NODES - 1].next == NULL);
    assert(free_head == &pool[0]);

    return 0;
}

static uint32_t hash_fnv1a(const char* key) { // 理解が薄い関数のため、メモ書きを残しておく
    uint32_t hash = 2166136261U; // FNVの32ビット用初期値
    const uint32_t fnv_prime = 16777619U; // FNVの32ビット用素数

    while (*key) {
        hash ^= (unsigned char)*key++; // 文字をXORで混ぜる
        hash *= fnv_prime;             // 素数を掛け算する
    }
    return hash;
}

static Node* scan_chain(const int idx, const char* input_key) {
    Node* node_p = hash_table[idx];
    while (node_p != NULL && node_p->key[0] != '\0') {
        if (strcmp(node_p->key, input_key) == 0) { return node_p; }
        node_p = node_p->next;
    }
    return NULL;
}

int kvs_string_get(Context* ctx_p) {
    int idx = hash_fnv1a(ctx_p->args[1]) % TABLE_SIZE;
    Node* matched_node_p = scan_chain(idx, ctx_p->args[1]);
    if (matched_node_p) {
        strncpy(ctx_p->result, matched_node_p->value.string_val, sizeof(ctx_p->result));
        ctx_p->status = 1;
        return 0;
    } else {
        snprintf(ctx_p->result, sizeof(ctx_p->result), "Undefined key: %s", ctx_p->args[1]);
        ctx_p->status = 2;
        return 1;
    }
}

int kvs_string_set(Context* ctx_p) {
    if (!(ctx_p->args[2])) {
        strcpy(ctx_p->result, "Value not provided")
        return 1;
    } else if (ctx_p->args[3]) {
        strcpy(ctx_p->result, "wrong number of arguments for 'set' command")
        return 1;
    }
    int idx = hash_fnv1a(ctx_p->args[1]) % TABLE_SIZE;
    Node* matched_node_p = scan_chain(idx, ctx_p->args[1]);
    if (matched_node_p) {
        strncpy(matched_node_p->value.string_val, ctx_p->args[2], sizeof(matched_node_p->value.string_val));
    } else {
        Node* new_node_p = free_head;
        strncpy(new_node_p->key, ctx_p->args[1], sizeof(new_node_p->key));
        strncpy(new_node_p->value.string_val, ctx_p->args[2], sizeof(new_node_p->value));
        new_node_p->type = TYPE_STRING;

        free_head = new_node_p->next;
        new_node_p->next = hash_table[idx]; //衝突時はnew_nodeをチェーンの先頭へ、非衝突時は意味ない処理
        hash_table[idx] = new_node_p;
    }
    ctx_p->status = 1;
    strcpy(ctx_p->result, "OK");
    return 0;
}