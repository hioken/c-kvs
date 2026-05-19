#include <stdio.h>
#include <stdint.h>
// #include <stdlib.h>

#define MAX_NODES 1000
#define TABLE_SIZE 1009
#define KEY_SIZE 256
#define VALUE_SIZE 768
#ifdef NOSTATIC
    #define STATIC
#else
    #define STATIC static
#endif

typedef enum {
    TYPE_STRING,
    TYPE_LIST,
    TYPE_HASH
} DataType;

typedef struct Node Node;
struct Node {
    char key[KEY_SIZE];
    DataType: type;
    Node* next;
    union {
        char string_val[VALUE_SIZE];
        Node* sub_node;
    } value;
};

int setup(void);
uint32_t hash_fnv1a(const char*);

STATIC Node pool[MAX_NODES];
STATIC Node* hash_table[TABLE_SIZE] = { NULL };
STATIC Node* free_head = &pool[0];

// 以下の関数はgeminiの丸写し
uint32_t hash_fnv1a(const char *str) {
    uint32_t hash = 2166136261U; // FNVの32ビット用初期値
    const uint32_t fnv_prime = 16777619U; // FNVの32ビット用素数

    while (*str) {
        hash ^= (unsigned char)*str++; // 文字をXORで混ぜる
        hash *= fnv_prime;             // 素数を掛け算する
    }
    return hash;
}