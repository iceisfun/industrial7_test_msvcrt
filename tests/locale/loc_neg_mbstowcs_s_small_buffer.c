/* TC
 * id:    loc_neg_mbstowcs_s_small_buffer
 * area:  locale
 * funcs: mbstowcs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs mbstowcs_s (ERANGE when buffer too small; _TRUNCATE gives STRUNCATE); errno.h STRUNCATE = 80
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
errno_t __cdecl mbstowcs_s(size_t *, wchar_t *, size_t, const char *, size_t);

static char hello[] = "hello";

static void fill(wchar_t *w) { int i; for (i = 0; i < 8; i++) w[i] = 0x5a5a; }

TC_CASE(too_small_erange)
{
    wchar_t w[8]; size_t n = 99; errno_t r; int c0 = tc_iph_count; fill(w);
    errno = 0; r = mbstowcs_s(&n, w, 3, hello, 5);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(r, 34); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(truncate_returns_strunc)
{
    wchar_t w[8]; size_t n = 99; errno_t r; fill(w);
    r = mbstowcs_s(&n, w, 4, hello, _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE); CHECK_EQ_I(r, 80);
}
TC_CASE(truncate_content_nul_terminated)
{
    wchar_t w[8]; size_t n = 99; errno_t r; fill(w);
    r = mbstowcs_s(&n, w, 4, hello, _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE); CHECK_WSTR(w, L"hel"); CHECK_EQ_I(w[3], 0);
    CHECK_EQ_I(w[4], 0x5a5a);   /* nothing written past sizeInWords */
}
TC_CASE(iph_count_only_for_range)
{
    wchar_t w[8]; size_t n = 99; int c0 = tc_iph_count; fill(w);
    mbstowcs_s(&n, w, 4, hello, _TRUNCATE);
    CHECK_EQ_I(tc_iph_count, c0);
    mbstowcs_s(&n, w, 3, hello, 5);
    CHECK_EQ_I(tc_iph_count, c0 + 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(too_small_erange); TC_RUN(truncate_returns_strunc);
    TC_RUN(truncate_content_nul_terminated); TC_RUN(iph_count_only_for_range);
    return tc_end();
}
