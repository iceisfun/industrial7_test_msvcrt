/* TC
 * id:    pfm_1_probe_vsnprintf_msvcr120_trunc
 * area:  stdio_printf
 * funcs:
 * procs: vsnprintf
 * crt:   msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs vsnprintf (C99 semantics: returns the count that would have been written); VS2013 added C99 snprintf family
 * note:  PROBE: msvcr120.dll exports vsnprintf and it is C99-conforming (returns needed count, always NUL-terminates on truncation).
 *        Contrast with _vsnprintf which returns -1 on truncation.
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

typedef int (__cdecl *vsnprintf_t)(char *, size_t, const char *, va_list);
static vsnprintf_t p_vsnprintf;

static int vs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsnprintf(b, n, f, ap); va_end(ap); return r; }

TC_CASE(exported) { CHECK(p_vsnprintf != NULL); }
TC_CASE(truncation_returns_needed)
{
    char b[16]; int r; memset(b, 'Z', sizeof b);
    if (!p_vsnprintf) TC_SKIP("vsnprintf not exported");
    r = vs(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8); CHECK_STR(b, "abc"); CHECK(b[4] == 'Z');
}
TC_CASE(fit)
{
    char b[16]; int r; memset(b, 'Z', sizeof b);
    if (!p_vsnprintf) TC_SKIP("vsnprintf not exported");
    r = vs(b, 16, "%d-%s", 5, "xy");
    CHECK_EQ_I(r, 4); CHECK_STR(b, "5-xy");
}

int main(void)
{
    tc_begin();
    p_vsnprintf = (vsnprintf_t)tc_crt_proc("vsnprintf");
    TC_RUN(exported); TC_RUN(truncation_returns_needed); TC_RUN(fit);
    return tc_end();
}
