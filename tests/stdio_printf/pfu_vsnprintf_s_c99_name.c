/* TC
 * id:    pfu_vsnprintf_s_c99_name
 * area:  stdio_printf
 * funcs:
 * procs: vsnprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _vsnprintf_s, _vsnprintf_s_l (vsnprintf_s is the same function without underscore)
 * note:  PROBE: that the DLL exports the C99-style name vsnprintf_s (header inline in mingw, so resolved at run time) and that it behaves as _vsnprintf_s. Narrowed to ucrtbase: only the UCRT headers document vsnprintf_s (VS2015+); older DLLs are not expected to export it.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
TC_DECL_PROC(int, vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
static int cv(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tc_p_vsnprintf_s(b, n, c, f, ap); va_end(ap); return r; }
TC_CASE(exported) { CHECK(tc_p_vsnprintf_s != NULL); }

TC_CASE(success)
{
    if (!tc_p_vsnprintf_s) TC_SKIP("not exported");
    char b[16]; volatile size_t n = 16, c = 16; int r;
    r = cv(b, n, c, "%d%s", 12, "ab");
    CHECK_EQ_I(r, 4); CHECK_STR(b, "12ab");
}
TC_CASE(truncate)
{
    if (!tc_p_vsnprintf_s) TC_SKIP("not exported");
    char b[16]; volatile size_t n = 4, c = _TRUNCATE; int r;
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_lt_len)
{
    if (!tc_p_vsnprintf_s) TC_SKIP("not exported");
    char b[16]; volatile size_t n = 16, c = 2; int r;
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(vsnprintf_s);
    TC_RUN(exported); TC_RUN(success); TC_RUN(truncate); TC_RUN(count_lt_len);
    return tc_end();
}
