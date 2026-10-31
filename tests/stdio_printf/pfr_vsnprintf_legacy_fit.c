/* TC
 * id:    pfr_vsnprintf_legacy_fit
 * area:  stdio_printf
 * funcs:
 * procs: _vsnprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf: len < count -> returns len and NUL-terminates; len == count -> returns count, no NUL appended
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

TC_DECL_PROC(int, _vsnprintf, (char *, size_t, const char *, va_list));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


static int cv(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = tc_p__vsnprintf(b, n, f, ap); va_end(ap);
    return r;
}

TC_CASE(exported) { CHECK(tc_p__vsnprintf != NULL); }
TC_CASE(fit)
{
    char b[16]; volatile size_t n = 16; int i, r;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = cv(b, n, "%s-%d", "abc", 12);
    CHECK_EQ_I(r, 6);
    CHECK_STR(b, "abc-12");
    CHECK_EQ_I(b[6], 0);
}
TC_CASE(exact_fill)
{
    char b[8]; volatile size_t n = 5; int i, r;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    r = cv(b, n, "%s", "abcde");
    CHECK_EQ_I(r, 5);
    CHECK_MEM(b, "abcde", 5);
    CHECK_EQ_I(b[5], 'Z');   /* no NUL written, sentinel intact */
}

int main(void) { tc_begin(); LOADP(_vsnprintf); TC_RUN(exported); TC_RUN(fit); TC_RUN(exact_fill); return tc_end(); }
