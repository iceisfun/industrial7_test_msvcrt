/* TC
 * id:    pfx_fprintf_ftell_append_probe
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ftell fflush
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs fopen "a", ftell
 * note:  Probes: after writing 3 bytes to an ab stream and fflush, ftell reports old size + 3 (file pointer at end of file).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_ftell_append_probe_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(append_ftell_after_flush)
{
    int n; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fprintf(f, "0123456789"); fclose(f);
    f = fopen(FN, "ab"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    n = fprintf(f, "abc");
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(ftell(f), 10 + n);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(append_ftell_after_flush);
    return tc_end();
}
