/* TC
 * id:    pfr_snprintf_s_truncate_fit
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_s, _vsnprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s: with _TRUNCATE, if output fits the string is written and its length returned
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));

/* ucrtbase does not export these (they are header inlines over __stdio_common_*); other CRTs are resolved from the DLL. */
#ifdef _UCRT
#define LOADP(n) (tc_p_##n = (tc_pt_##n)(n))
#else
#define LOADP(n) TC_LOAD_PROC(n)
#endif


static int cv(char *b, size_t n, size_t c, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = tc_p__vsnprintf_s(b, n, c, f, ap); va_end(ap);
    return r;
}

TC_CASE(exported) { CHECK(tc_p__snprintf_s != NULL); CHECK(tc_p__vsnprintf_s != NULL); }
TC_CASE(fits)
{
    char b[10]; volatile size_t n = 10; int r;
    r = tc_p__snprintf_s(b, n, _TRUNCATE, "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
}
TC_CASE(exactly_size_minus_1)
{
    char b[10]; volatile size_t n = 10; int r;
    r = tc_p__snprintf_s(b, n, _TRUNCATE, "%s", "123456789");
    CHECK_EQ_I(r, 9);
    CHECK_STR(b, "123456789");
}
TC_CASE(va_list_variant)
{
    char b[10]; volatile size_t n = 10; int r;
    r = cv(b, n, _TRUNCATE, "%d-%d", 1, 2);
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "1-2");
    r = cv(b, n, _TRUNCATE, "%s", "123456789");
    CHECK_EQ_I(r, 9);
    CHECK_STR(b, "123456789");
}

int main(void) { tc_begin(); LOADP(_snprintf_s); LOADP(_vsnprintf_s); TC_RUN(exported); TC_RUN(fits); TC_RUN(exactly_size_minus_1); TC_RUN(va_list_variant); return tc_end(); }
