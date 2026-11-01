/* TC
 * id:    pfu_vsnprintf_s_c99_name
 * area:  stdio_printf
 * funcs:
 * procs: __stdio_common_vsnprintf_s _vsnprintf_s vsnprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _vsnprintf_s, _vsnprintf_s_l (vsnprintf_s is the C99-style header name; both inline in the UCRT headers)
 * note:  D (export alias): ucrtbase.dll exports neither vsnprintf_s nor _vsnprintf_s (GetProcAddress == NULL, observed on Win10 22H2 19045);
 *        both are header inlines over the exported __stdio_common_vsnprintf_s. The behaviour cases therefore call that export.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
TC_DECL_PROC(int, _vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
TC_DECL_PROC(int, __stdio_common_vsnprintf_s, (unsigned __int64, char *, size_t, size_t, const char *, void *, va_list));
TC_DECL_PROC(int, vsnprintf_s, (char *, size_t, size_t, const char *, va_list));
static int cv(char *b, size_t n, size_t c, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tc_p___stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
TC_CASE(exported) { CHECK(tc_p___stdio_common_vsnprintf_s != NULL); }
TC_CASE(c99_name_not_exported) { CHECK(tc_p_vsnprintf_s == NULL); CHECK(tc_p__vsnprintf_s == NULL); }

TC_CASE(success)
{
    char b[16]; volatile size_t n = 16, c = 16; int r;
    if (!tc_p___stdio_common_vsnprintf_s) TC_SKIP("not exported");
    r = cv(b, n, c, "%d%s", 12, "ab");
    CHECK_EQ_I(r, 4); CHECK_STR(b, "12ab");
}
TC_CASE(truncate)
{
    char b[16]; volatile size_t n = 4, c = _TRUNCATE; int r;
    if (!tc_p___stdio_common_vsnprintf_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_lt_len)
{
    char b[16]; volatile size_t n = 16, c = 2; int r;
    if (!tc_p___stdio_common_vsnprintf_s) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "ab"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_LOAD_PROC(_vsnprintf_s); TC_LOAD_PROC(vsnprintf_s); TC_LOAD_PROC(__stdio_common_vsnprintf_s);
    TC_RUN(exported); TC_RUN(c99_name_not_exported); TC_RUN(success); TC_RUN(truncate); TC_RUN(count_lt_len);
    return tc_end();
}
