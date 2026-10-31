/* TC
 * id:    pfx_fprintf_neg_readonly_errno_probe
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs errno / _doserrno
 * note:  Probes: errno is EBADF after fprintf on a stream opened read-only.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_neg_readonly_errno_probe_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(ebadf)
{
    int r; FILE *f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) return;
    fprintf(f, "x"); fclose(f);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    (void)tc_iph_install();   /* keep a possible invalid-parameter call non-fatal; not asserted */
    errno = 0;
    r = fprintf(f, "y");
    CHECK(r < 0);
    CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(ebadf);
    return tc_end();
}
