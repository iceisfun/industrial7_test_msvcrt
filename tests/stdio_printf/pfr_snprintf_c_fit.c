/* TC
 * id:    pfr_snprintf_c_fit
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_c
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   none (_snprintf_c is not in the public MS docs; CRT-internal helper that is exported)
 * note:  PROBE: (1) the DLL export _snprintf_c exists in msvcr80..msvcr120, (2) when output fits it returns the length
 *        and NUL-terminates. Only the fits case is asserted; truncation behaviour is deliberately not asserted.
 */
#include "tc.h"

typedef int (__cdecl *snprintf_c_t)(char *, size_t, const char *, ...);
static snprintf_c_t p;

TC_CASE(exported) { CHECK(p != NULL); }
TC_CASE(fits)
{
    char b[16]; volatile size_t n = 16; int i, r;
    if (!p) TC_SKIP("not exported");
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = p(b, n, "%s%d", "abc", 7);
    CHECK_EQ_I(r, 4);
    CHECK_STR(b, "abc7");
}
TC_CASE(empty)
{
    char b[4]; volatile size_t n = 4; int r;
    if (!p) TC_SKIP("not exported");
    b[0] = 'Z';
    r = p(b, n, "%s", "");
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(b[0], 0);
}

int main(void) { tc_begin(); p = (snprintf_c_t)tc_crt_proc("_snprintf_c"); TC_RUN(exported); TC_RUN(fits); TC_RUN(empty); return tc_end(); }
