/* TC
 * id:    loc_neg_wcstombs_s_small_buffer
 * area:  locale
 * funcs: wcstombs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs_s (ERANGE when buffer too small; _TRUNCATE gives STRUNCATE)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

#ifndef _TRUNCATE
#define _TRUNCATE ((size_t)-1)
#endif
#ifndef STRUNCATE
#define STRUNCATE 80
#endif
errno_t __cdecl wcstombs_s(size_t *, char *, size_t, const wchar_t *, size_t);

static wchar_t w_hello[] = L"hello";
static void fill(char *b) { int i; for (i = 0; i < 8; i++) b[i] = 'Z'; }

TC_CASE(erange)
{
    char b[8]; size_t n = 99; errno_t r; int c0 = tc_iph_count; fill(b);
    errno = 0; r = wcstombs_s(&n, b, 3, w_hello, 5);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(r, 34); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(truncate)
{
    char b[8]; size_t n = 99; errno_t r; fill(b);
    r = wcstombs_s(&n, b, 4, w_hello, _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE); CHECK_EQ_I(r, 80);
}
TC_CASE(truncated_content)
{
    char b[8]; size_t n = 99; errno_t r; fill(b);
    r = wcstombs_s(&n, b, 4, w_hello, _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE); CHECK_STR(b, "hel"); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(iph_counts)
{
    char b[8]; size_t n = 99; int c0 = tc_iph_count; fill(b);
    wcstombs_s(&n, b, 4, w_hello, _TRUNCATE);
    CHECK_EQ_I(tc_iph_count, c0);
    wcstombs_s(&n, b, 3, w_hello, 5);
    CHECK_EQ_I(tc_iph_count, c0 + 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(erange); TC_RUN(truncate); TC_RUN(truncated_content); TC_RUN(iph_counts);
    return tc_end();
}
