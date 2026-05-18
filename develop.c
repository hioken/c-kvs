#include <stdio.h>

int main(void) {
#ifdef __STDC_VERSION__
    printf("現在のC言語バージョン（規格）: %ld\n", __STDC_VERSION__);
#else
    printf("C89/C90（または古いコンパイラ）です。\n");
#endif
    return 0;
}