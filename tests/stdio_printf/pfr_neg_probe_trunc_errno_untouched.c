/* TC
 * id:    pfr_neg_probe_trunc_errno_untouched
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf _snwprintf
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf (truncation: count bytes written, -1 returned), C11 7.21.6.5 snprintf; contrast _snprintf_s _TRUNCATE (STRUNCATE)
 * note:  PROBE (errno_* cases only; return values/handler checks in the other cases are documented): plain truncation by the non-_s functions is not an error condition: errno stays at a preset sentinel (4242)
 *        and the invalid-parameter handler is not called. The legacy functions return -1 on truncation; on ucrtbase the C99
 *        snprintf returns the would-be length (8) and does not touch errno either.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#define SENT 4242
TC_CASE(snprintf_legacy)
{
    char b[16]; int r;
    tc_iph_count = 0; errno = SENT;
    r = _snprintf(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(snwprintf_legacy)
{
    wchar_t b[16]; int r;
    tc_iph_count = 0; errno = SENT;
    r = _snwprintf(b, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0);
}
#if TC_CRT_VER >= 140
TC_CASE(snprintf_c99_ucrt)
{
    char b[16]; int r;
    tc_iph_count = 0; errno = SENT;
    r = snprintf(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
#endif
TC_CASE(errno_legacy)
{
    char b[16]; wchar_t w[16];
    errno = SENT; (void)_snprintf(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(errno, SENT);
    errno = SENT; (void)_snwprintf(w, 4, L"%ls", L"abcdefgh");
    CHECK_EQ_I(errno, SENT);
}
#if TC_CRT_VER >= 140
TC_CASE(errno_c99_ucrt)
{
    char b[16];
    errno = SENT; (void)snprintf(b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(errno, SENT);
}
#endif
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(snprintf_legacy); TC_RUN(snwprintf_legacy); TC_RUN(errno_legacy);
#if TC_CRT_VER >= 140
    TC_RUN(snprintf_c99_ucrt); TC_RUN(errno_c99_ucrt);
#endif
    return tc_end();
}
