/* TC
 * id:    pfx_fprintf_return_counts
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ftell
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1, 7.21.3; MS docs fprintf, fopen
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_return_counts_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(counts)
{
    volatile const char *s = "ab"; volatile int d = 42; volatile const char *l = "abcdef";
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abc"), 3);
    CHECK_EQ_I(fprintf(f, "%5d", d), 5);
    CHECK_EQ_I(fprintf(f, "%-5s", (const char *)s), 5);
    CHECK_EQ_I(fprintf(f, "%.3s", (const char *)l), 3);
    CHECK_EQ_I(fprintf(f, "%%"), 1);
    CHECK_EQ_I(ftell(f), 3 + 5 + 5 + 3 + 1);
    CHECK_EQ_I(fclose(f), 0);
    remove(FN);
}
TC_CASE(sum_equals_ftell)
{
    int sum = 0; volatile int d = 12345;
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    sum += fprintf(f, "x=%d;", d);
    sum += fprintf(f, "[%08.3f]", 3.14159);
    sum += fprintf(f, "%s", "tail");
    CHECK_EQ_I(sum, 8 + 10 + 4);
    CHECK_EQ_I(ftell(f), sum);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(counts);
    TC_RUN(sum_equals_ftell);
    return tc_end();
}
