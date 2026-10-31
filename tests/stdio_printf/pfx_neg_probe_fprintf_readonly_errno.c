/* TC
 * id:    pfx_neg_probe_fprintf_readonly_errno
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   _flsbuf on a stream not open for writing
 * note:  PROBE: errno is EBADF after fprintf to a stream opened "rb". The negative return itself is asserted in pfx_neg_fprintf_readonly_stream.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>
#define F "tc_pfx_neg_probe_fprintf_readonly_errno_" TCV ".tmp"
TC_CASE(errno_ebadf)
{
    FILE *w = fopen(F, "wb"), *r;
    CHECK(w != NULL); if (!w) return;
    fputs("data", w); fclose(w);
    r = fopen(F, "rb");
    CHECK(r != NULL); if (!r) return;
    errno = 0;
    fprintf(r, "x%d", 1);
    CHECK_EQ_I(errno, EBADF);
    fclose(r);
}
int main(void)
{
    tc_begin(); remove(F); tc_iph_install();
    TC_RUN(errno_ebadf);
    remove(F); return tc_end();
}
