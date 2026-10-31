/* TC
 * id:    pfr_neg_probe_vsnprintf_s_size0_errno
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vsnprintf_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _vsnprintf_s (parameter validation)
 * note:  PROBE: return -1, iph called once, and errno after _vsnprintf_s(buf, 0, 5, ...) fails; EINVAL guessed (size 0 is a parameter validation failure like sprintf_s), ERANGE is the alternative.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;

/* msvcr80's import library lacks the _s functions: resolve non-UCRT targets at run time (ucrtbase has them inline). */
#if TC_CRT_VER < 140
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
#define _vsnprintf_s tc_p__vsnprintf_s
#endif
static int vn(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = _vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }

TC_CASE(size_zero_errno)
{
    char b[10]; volatile size_t z = 0; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = vn(b, z, 5, "%s", "abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(_vsnprintf_s);
    if (!tc_p__vsnprintf_s) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 2; }
#endif
    TC_RUN(size_zero_errno);
    return tc_end();
}
