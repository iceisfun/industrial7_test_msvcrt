/* TC
 * id:    pfx_fprintf_ftell_binary
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ftell
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.9.4; MS docs ftell
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_ftell_binary_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(ftell_zero)
{
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ftell(f), 0);
    fclose(f); remove(FN);
}
TC_CASE(ftell_buffered)
{
    FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "hello"), 5);
    CHECK_EQ_I(ftell(f), 5);
    fclose(f); remove(FN);
}
TC_CASE(ftell_sum)
{
    int a, b; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    a = fprintf(f, "%d", 123456); b = fprintf(f, "\n%s", "xy");
    CHECK_EQ_I(a, 6); CHECK_EQ_I(b, 3);
    CHECK_EQ_I(ftell(f), a + b);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(ftell_zero);
    TC_RUN(ftell_buffered);
    TC_RUN(ftell_sum);
    return tc_end();
}
