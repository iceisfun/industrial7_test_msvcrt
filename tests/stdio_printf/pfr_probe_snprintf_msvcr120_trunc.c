/* TC
 * id:    pfr_probe_snprintf_msvcr120_trunc
 * area:  stdio_printf
 * funcs:
 * procs: snprintf
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs snprintf (VS2013 C99 conformance), C11 7.21.6.5
 * note:  PROBING whether msvcr120 snprintf is C99 (returns would-be length 8, NUL-terminated "abc") or legacy (-1).
 *        Asserted C99 as the most likely behaviour.
 */
#include "tc.h"
#include <stdio.h>

typedef int (__cdecl *snprintf_t)(char *, size_t, const char *, ...);
static snprintf_t p_snprintf;

TC_CASE(trunc_c99)
{
    char b[8]; int r;
    if (!p_snprintf) TC_SKIP("snprintf not exported");
    b[0]=b[1]=b[2]=b[3]=b[4]='Q';
    r = p_snprintf(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_STR(b, "abc");
}
int main(void) { tc_begin(); p_snprintf = (snprintf_t)tc_crt_proc("snprintf"); TC_RUN(trunc_c99); return tc_end(); }
