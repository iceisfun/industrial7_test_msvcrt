/* TC
 * id:    pfr_probe_snprintf_msvcr120_null0
 * area:  stdio_printf
 * funcs:
 * procs: snprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs snprintf, C11 7.21.6.5 (n==0: nothing written, s may be null)
 * note:  PROBING whether msvcr120 snprintf(NULL,0,"hello") allows a sizing query and returns 5.
 */
#include "tc.h"
#include <stdio.h>

typedef int (__cdecl *snprintf_t)(char *, size_t, const char *, ...);
static snprintf_t p_snprintf;

TC_CASE(null0)
{
    char *volatile nb = NULL; int r;
    if (!p_snprintf) TC_SKIP("snprintf not exported");
    r = p_snprintf(nb, 0, "%s", "hello");
    CHECK_EQ_I(r, 5);
}
int main(void) { tc_begin(); p_snprintf = (snprintf_t)tc_crt_proc("snprintf"); tc_iph_install(); TC_RUN(null0); return tc_end(); }
