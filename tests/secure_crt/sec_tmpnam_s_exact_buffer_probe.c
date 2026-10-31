/* TC
 * id:    sec_tmpnam_s_exact_buffer_probe
 * area:  secure_crt
 * funcs:
 * procs: tmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs tmpnam_s (L_tmpnam_s)
 * note:  Probes: tmpnam_s succeeds (0) with an 18-char buffer, i.e. sizeof("\\")+16, my recollection of MS's L_tmpnam_s
 *        (the mingw header value is smaller, so it is not used), and the result fits (NUL within the buffer).
 */
#include "tc.h"
#include <stddef.h>

typedef int (__cdecl *tmpnam_s_t)(char *, size_t);
static tmpnam_s_t p_tmpnam_s;

TC_CASE(buffer_18_ok)
{
    char b[18]; int r, i, nul = 0;
    r = p_tmpnam_s(b, sizeof b); CHECK_EQ_I(r, 0);
    for (i = 0; i < 18; i++) if (!b[i]) nul = 1;
    CHECK(nul);
}
int main(void)
{
    tc_begin();
    p_tmpnam_s = (tmpnam_s_t)tc_crt_proc("tmpnam_s");
    CHECK(p_tmpnam_s); if (!p_tmpnam_s) return tc_end();
    TC_RUN(buffer_18_ok);
    return tc_end();
}
