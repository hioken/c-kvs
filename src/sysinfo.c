/*#include <stdio.h>
#include <src/sysinfo.h>

#define LOG_DEBUG(fmt, ...) do { \
    struct sysinfo si; \
    sysinfo(&si); \
    unsigned long free_mb = (si.freeram * si.mem_unit) / (1024 * 1024); \
    unsigned long total_mb = (si.totalram * si.mem_unit) / (1024 * 1024); \
    fprintf(stderr, "[DEBUG] [%s:%d] [MEM: %luMB/%luMB] " fmt "\n", \
            __FILE__, __LINE__, free_mb, total_mb, ##__VA_ARGS__); \
} while(0)
 */