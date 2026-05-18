#include <stdio.h>
#include <sys/sysinfo.h>

int main() {
    struct sysinfo si;

    // システム情報を取得
    if (sysinfo(&si) != 0) {
        perror("sysinfoの取得に失敗しました");
        return 1;
    }

    // sysinfoの値は設定されたメモリ単位（mem_unit）を掛けることでバイト数になる
    unsigned long long total_bytes = (unsigned long long)si.totalram * si.mem_unit;
    unsigned long long free_bytes = (unsigned long long)si.freeram * si.mem_unit;
    unsigned long long buffer_bytes = (unsigned long long)si.bufferram * si.mem_unit;

    // 簡易的な利用可能メモリの計算（free + buffer/cacheに相当する分）
    // ※より厳密なavailable値は /proc/meminfo のパースが必要ですが、
    // sysinfo単体では（free + buffer）を空き領域として扱うのが一般的です。
    unsigned long long usable_free = free_bytes + buffer_bytes;
    unsigned long long used_bytes = total_bytes - usable_free;

    // パーセンテージの算出
    double usage_percent = ((double)used_bytes / total_bytes) * 100.0;

    printf("全メモリ容量: %llu bytes\n", total_bytes);
    printf("実質使用容量: %llu bytes\n", used_bytes);
    printf("メモリ使用率: %.2f%%\n", usage_percent);

    return 0;
}