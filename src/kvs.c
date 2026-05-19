#include <stdio.h>
#include <stdint.h>
// #include <stdlib.h>

#define MAX_NODES 100
#define TABLE_SIZE 1009
#define KEY_SIZE 256
#define VALUE_SIZE 768
#ifdef NOSTATIC
    #define STATIC
#else
    #define STATIC static
#endif


typedef

// STATIC Node pool[MAX_NODES]

uint32_t hash_fnv1a(const char*);


uint32_t hash_fnv1a(const char *str) {
    uint32_t hash = 2166136261U; // FNVの32ビット用初期値
    const uint32_t fnv_prime = 16777619U; // FNVの32ビット用素数

    while (*str) {
        hash ^= (unsigned char)*str++; // 文字をXORで混ぜる
        hash *= fnv_prime;             // 素数を掛け算する
    }
    return hash;
}