#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/resource.h>
#include <unistd.h>
#include "kvs.h"

int info(char* file, int line, Context* ctx_p) {
    struct rusage usage;
    printf("== info[%s:%d] ==\n", file, line);

    int i = 0;
    long avphys_pages;
    long page_size;
    page_size = sysconf(_SC_PAGESIZE);

    if (page_size == -1) {
        perror("sysconf failed");
        return 1;
    }

    avphys_pages = get_avphys_pages();
    if (avphys_pages == -1) {
        perror("sysinfo.h error\n");
        return 1;
    } else {
        printf("Free Pages: %ld\n", avphys_pages);
    }

    if (getrusage(RUSAGE_SELF, &usage) != 0) {
        perror("resource.h error\n");
        return 1;
    } else {
        printf("Max Memory Usage Pages: %ld\n", usage.ru_maxrss * 1024 / page_size );
    }

    if (ctx_p == NULL) {
        printf("Context: Not provided (Skipped)\n");
    } else {
        printf("Context: \n");
        printf("  status: %d\n", ctx_p->status);
        printf("  result: %s\n", ctx_p->result);
        if (ctx_p->args[0] == NULL) {
            printf("  buf: %s\n", ctx_p->buf);
        } else {
            while(i < MAX_ARGS && ctx_p->args[i] != NULL && ctx_p->args[i][0] != '\0') {
                printf("  arg%d: %s\n", i, ctx_p->args[i]);
                i++;
            }
        }
    }
    return 0;
}