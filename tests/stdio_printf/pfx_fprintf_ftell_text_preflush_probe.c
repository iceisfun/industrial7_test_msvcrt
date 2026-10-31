/* TC
 * id:    pfx_fprintf_ftell_text_preflush_probe
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ftell
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs ftell
 * note:  Probes: ftell on a text-mode write stream with unflushed data counts the pending LF as CR-LF (6 for a-LF-b-LF).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_ftell_text_preflush_probe_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(preflush)
{
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a\nb\n"), 4);
    CHECK_EQ_I(ftell(f), 6);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(preflush);
    return tc_end();
}
